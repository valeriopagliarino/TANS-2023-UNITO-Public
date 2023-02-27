/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/
#include <TMath.h>
#include <algorithm>   // for sorting std vector
 
#include "../inc/hitsAnalysis.h"

//This class manages the reconstruction

HitsAnalysis::HitsAnalysis(ProgramConfig * config){
  conf = config;
  outTree = new TTree("T","TTree con 1 branch");   // TTree that will contain the output results for reconstruction
  outRecoFile = new TFile(conf->outRecoRootFileName.c_str(),"RECREATE");  // .root file where the results from reconstruction will be saved
  outTree->SetDirectory(outRecoFile);
  outTree->Branch("reconstructedVertex",&recVertex.Zr,"Zr/D:eventID/I");
  gROOT->cd();
  // Parameters for reconstruction read from the configuration file
  runningW = conf->runningWindowSize;
  auto runningWStepPerc = conf->runningWPercentStep;
  runningWStep = runningWStepPerc * runningW;
  limitPercentMaxNumVert = conf->limitPercMaxNumVert; // limit percentual of numMax allowed for an other window in order to be confident of vertex reconstruction.
  minNumVertInWindow = conf->minVertNumber; // min number of vertexes in a window to be chosen for vertex reconstruction 

}


HitsAnalysis::~HitsAnalysis(){
    
    delete outTree;
    delete outRecoFile;
    delete branchHits;
    delete conf;

}

void HitsAnalysis::ImportConfiguration(ProgramConfig * config)
{
    conf = config;
}

void HitsAnalysis::CalculateZrec(TFile * simFile, int nTTreeVersions, double dPhiMax){

 
      deltaPhiMax=dPhiMax;
      std::vector<double> vecX1, vecX2, vecY1, vecY2, vecZ1, vecZ2;   // Vectors where the hits on the 2 detectors will be saved
      vecX1.reserve(40);
      vecX2.reserve(40);
      vecY1.reserve(40);
      vecY2.reserve(40);
      vecZ1.reserve(40);
      vecZ2.reserve(40);
      vecZtracklets.reserve(30);
      numZinWindow.reserve(500);
      char ttreeName[60];
      
      
      for(int l = 1;l<=nTTreeVersions;l++){
        sprintf(ttreeName,"PixelTracker;%d",l);
        tree = (RunManager*)simFile->Get(ttreeName);
        auto cacheSize=10000000U;
        tree->SetCacheSize(cacheSize);
        branchHits = tree->GetBranch("DetectorHits");
        branchHits->SetAddress(&dhit.X);
        int q=branchHits->GetEntries();
        int j=0;
        branchHits->GetEvent(0); 
        while(j<q){ // loop over events
            currentEvent = dhit.eventID;
            while(dhit.eventID == currentEvent && j<q){  // loop on the single event. The vectors of coordinates are filled
                double X1, X2, Y1, Y2, Z1, Z2;
                if(dhit.detectorID == 1){
                    X1=dhit.X;
                    Y1=dhit.Y;
                    Z1=dhit.Z;
                    vecX1.push_back(X1);
                    vecY1.push_back(Y1);
                    vecZ1.push_back(Z1);
                }
                else if (dhit.detectorID == 2){
                    X2=dhit.X;
                    Y2=dhit.Y;
                    Z2=dhit.Z;  
                    vecX2.push_back(X2);
                    vecY2.push_back(Y2);
                    vecZ2.push_back(Z2);                                         
                }   
                j=j+1;
                branchHits->GetEvent(j);   
            }
            GetIntersections(vecX1,vecX2,vecY1,vecY2,vecZ1,vecZ2);   // When an event has been processed this function is called
            vecX1.clear();
            vecX2.clear();
            vecY1.clear();
            vecY2.clear();
            vecZ1.clear();
            vecZ2.clear();
        }
        /*static ProcInfo_t info;
        const float toMB = 1.f/1024.f;
        gSystem->GetProcInfo(&info);
        std::cerr<<"\n res  memory = Mbytes  " <<info.fMemResident*toMB;
        std::cerr<<" vir  memory = Mbytes  " <<info.fMemVirtual*toMB;*/
      }
      outRecoFile->cd();
      outRecoFile->Write();
      outRecoFile->Close();
      delete outRecoFile;
}

// This function builds tracklets and finds the intersection with Z axis
void HitsAnalysis::GetIntersections(std::vector<double> vX1,std::vector<double> vX2,std::vector<double> vY1,std::vector<double> vY2,std::vector<double> vZ1,std::vector<double> vZ2){
    
    double phi1, phi2, deltaPhi, zRecTracklets;
    for(long unsigned int i=0;i<vX1.size();i++){ // 2 loops: on the first and second detector. All the hits on the 2 detectors are considered
        for(long unsigned int j=0;j<vX2.size();j++){
            phi1=atan(vX1[i]/vY1[i]);
            if(vX1[i]<0) phi1=phi1+TMath::Pi();
            else if (vX1[i]>0 && vY1[i]<0) phi1=phi1 + 2*TMath::Pi();

            phi2=atan(vX2[j]/vY2[j]);
            if(vX2[j]<0) phi2=phi2+TMath::Pi();
            else if (vX2[j]>0 && vY2[j]<0) phi2=phi2 + 2*TMath::Pi();

            deltaPhi=abs(phi2-phi1);
            if(deltaPhi<=deltaPhiMax){   // If deltaPhi between the hits on the 2 detectors is lower than DeltaPhiMax, the intersection with z axis is calculated
                double p0x = vX1[i];
                double p0y = vY1[i];
                double p0z = vZ1[i];
                double p1x = vX2[j]-vX1[i];
                double p1y = vY2[j]-vY1[i];
                double p1z = vZ2[j]-vZ1[i];
                double aa = p1x*p1x +p1y*p1y;
                double bb = 2*(p0x*p1x + p0y*p1y);
                double tt = -bb/(2*aa);

                zRecTracklets = p0z+tt*p1z;
                vecZtracklets.push_back(zRecTracklets);                //array that contains the coordinates of candidate vertices
            }
        }
    }
    
    bool checkReconstruction;
    checkReconstruction = ReconstructZ();      // Function that, if it is possible, recostruct the vertex starting from the candidate vertices
    if(checkReconstruction == true){   // If the vertex has been reconstructed, it is added to the TTree
        recVertex.Zr = zRec;
        recVertex.eventID = Int_t(currentEvent);
        outTree->Fill();
    }
    vecZtracklets.clear();
}
      
// This function implements the running window, from the candidate vertices calculates the reconstructed vertex.
// It returns a bool: true if the vertex has beeen reconstructed, false otherwise
bool HitsAnalysis::ReconstructZ(){
    
    // find the window that maximizes the number of candidate vertexes 
    double length = vecZtracklets.size();
    std::sort(vecZtracklets.begin(), vecZtracklets.end());
    double min = vecZtracklets[0];
    min = min - runningWStep/2;
    double max = vecZtracklets[length-1];
    int stepLimit = (int) (runningW/runningWStep);
    int i = 0;
    double iterate = min;
    while(iterate < max){    
        i++;    
        iterate = min + i*runningWStep;
        
    }
    const int iMax = i;      //iMax: number of windows
    for(int p = 0; p<(iMax+2*stepLimit);p++){
        numZinWindow.push_back(0);        // for each window this vector will contain the number of candidate vertices in that window
    }
    int counter, numMax, indexMax;     //numMax: maximum number of candidate vertices in a window, indexMax: index of the window that contains the max number of vertices
    counter = numMax = indexMax = 0;

    for (int j=0;j<iMax;j++){
        for(int k = 0; k<length; k++){
            if(vecZtracklets[k] < (min+j*runningWStep+runningW) && vecZtracklets[k] > (min+j*runningWStep)){
                counter = counter +1;
            }
        }
        numZinWindow[j+stepLimit] = counter;
        if(counter>numMax){
            numMax = counter;
            indexMax = j;
        }
        counter = 0;
    }
    indexMax = indexMax + stepLimit;

    // check if the window is selected with a certain degree of confidence. If yes, reconstruct the vertex, otherwise don't reconstruct vertex
    double nLimit = limitPercentMaxNumVert * numMax;       // limit of candidates vertices contained in the other windows (not the window with the max number of candidate vertices)
    int numLimit = TMath::Nint(nLimit);
    int leftMax, rightMax;
    leftMax = rightMax = 0;
    for (int u = 0;u<(indexMax-stepLimit);u++){
        if(numZinWindow[u]>leftMax){
            leftMax = numZinWindow[u];      //max number of candidate vertices contained in a window for the windows on the left of the window that contains the maximum number of candidate vertices
        }
    }
    for(int w = (indexMax + stepLimit);w<(iMax+2*stepLimit);w++){
        if(numZinWindow[w]>rightMax){
            rightMax = numZinWindow[w];     //max number of candidate vertices contained in a window for the windows on the right of the window that contains the maximum number of candidate vertices
        }
    }
    numZinWindow.clear();
    int start = 0;
    Double_t zzRec = 0.;
    if(numMax>=minNumVertInWindow && leftMax<numLimit && rightMax<numLimit){     // conditions that have to be satisfied to reconstruct the vertex
        
        Double_t aux = vecZtracklets[0];
        int r;
        for(r=0;r<length;r++){
            if(vecZtracklets[r]>(min + (indexMax-stepLimit)*runningWStep)){
            break;
            }
        }
        start=r;
        for(int d=start;d<(start+numMax);d++){
            zzRec = zzRec + vecZtracklets[d];
        }
        zRec = zzRec/numMax;
        return true;
    }
    else{
        return false;
    }
    
}
