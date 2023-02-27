/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/resultsAnalysis.h"

// This class contains all the functions used to analyse the reconstruction output and to compare these results with the MC truth.

ResultsAnalysis::ResultsAnalysis(ProgramConfig * givenConf){
    //Import configuration file
    conf = givenConf;
    
    //ROOT TFile for writing data analysis results
    outAnalysisFile = new TFile(conf->outAnalysisRootFileName.c_str(),"RECREATE");  
    gROOT->cd();

}



ResultsAnalysis::~ResultsAnalysis(){

    outRecoFile->Close();
    outAnalysisFile->Close();
    delete outRecoFile;
    delete outAnalysisFile;

}


// This function manages all the analysis process 
void ResultsAnalysis::CompareResults(TFile * simFile, int nTTreeVersions){

    simOutputFile = simFile;    
    nVersions = nTTreeVersions;
    //ROOT file where reconstruction output has been saved
    outRecoFile = new TFile(conf->outRecoRootFileName.c_str(),"READ");
    recoTree = (TTree*)outRecoFile->Get("T");  // output ttree from reconstruction
    Double_t lowestZtrue, highestZtrue;
    Int_t lowestMult, highestMult; 

    branchRecoVert = recoTree->GetBranch("reconstructedVertex");
    // Functions used to calculate the minimum and maximum values of Ztrue and Multiplicity from the data. This values will be used to initialize histo limits
    GetTreeMinMaxZ(lowestZtrue, highestZtrue, sigmaPrimaryVertex);
    
    GetTreeMinMaxMult(lowestMult, highestMult);
    // Function used to calculate down and up limits for Multiplicity and Ztrue ranges that will be considered during the analysis
    ConfigureMultZtrueRanges(lowestMult,highestMult);  
    
    zTrueNum = zTrueLeftLimit.size();
    multNum = multLeftLimit.size();
   

    EfficiencyAnalysis(lowestZtrue,highestZtrue,lowestMult,highestMult);  // makes efficiency analysis
    ResolutionAnalysis(lowestZtrue,highestZtrue,lowestMult,highestMult);   // makes resolution analysis
    outAnalysisFile->Write();
    
}

// It fills 4 arrays with up and down limits of the Ztrue and Multiplicity ranges, that will be considered during the analysis
void ResultsAnalysis::ConfigureMultZtrueRanges(int lowestMul,int highestMul){
    int temp, highestMult;
    if(lowestMul>(conf->minMult)){
        temp = lowestMul;
    }
    else{
        temp = conf->minMult;
    }
    if(highestMul<(conf->maxMult)){
        highestMult = conf->maxMult;
    }
    else{
        highestMult = conf->maxMult;
    }
    while(temp<highestMult){
        if(temp<10){
            multLeftLimit.push_back(temp-0.5);
            multRightLimit.push_back(temp+0.5);
            temp = temp + 1;
        }
        else if(10<=temp && temp<25){
            multLeftLimit.push_back(temp-0.5);
            multRightLimit.push_back(temp+0.5);
            temp = temp + 3;
        }
        else if(25<=temp && temp<60){
            multLeftLimit.push_back(temp-1.5);
            multRightLimit.push_back(temp+1.5);
            temp = temp + 6;
        }
        else {
            multLeftLimit.push_back(temp-2.5);
            multRightLimit.push_back(temp+2.5);
            temp = temp + 10;
        }
    }
    double maximumZtrue = sigmaPrimaryVertex*(conf->sigmasNumber);
    double sigma = sigmaPrimaryVertex;
    double aux = -1. * maximumZtrue;
    while(aux<=0){
        if(aux<(-sigma)){
            zTrueLeftLimit.push_back(aux-sigma/3);
            zTrueRightLimit.push_back(aux+sigma/3);
            aux = aux + 2*sigma/3;
        }
        else{
            zTrueLeftLimit.push_back(aux-sigma/6);
            zTrueRightLimit.push_back(aux+sigma/6);
            aux = aux + sigma/3;
        }
    }
    for(int j=(zTrueLeftLimit.size()-1);j>=0;j--){
        zTrueLeftLimit.push_back(-zTrueRightLimit[j]);
        zTrueRightLimit.push_back(-zTrueLeftLimit[j]);
    }  
}





// Function for efficiency analysis
void ResultsAnalysis::EfficiencyAnalysis(double minZtrue, double maxZtrue,int minMult, int maxMult){
    
    double minZt = (double)minZtrue;
    double maxZt = (double)maxZtrue;
    //2D histogram (x axis: vertex Ztrue, y axis: vertex Multiplicity, z axis: counts). Filled for all the primary vertices
    TH2D * multZtrueAll = new TH2D("multZtrueAll","Ztrue and Multiplicity for all primary vertexes",2000,minZt,maxZt,200,minMult,maxMult);
    //2D histogram (x axis: vertex Ztrue, y axis: vertex Multiplicity, z axis: counts). Filled only for primary vertices that have been reconstructed
    TH2D * multZtrueReco = new TH2D("multZtrueReco","Ztrue and Multiplicity for reconstructed primary vertexes",2000,minZt,maxZt,200,minMult,maxMult);
    
    int vR = 0;
    branchRecoVert->SetAddress(&recVertex.Zr);
    
    // The loop fills the 2D histograms
    char ttreeName[60];
    for(int l=1;l<=nVersions;l++){                                                     
        sprintf(ttreeName,"PixelTracker;%d",l);
        tree = (RunManager*)simOutputFile->Get(ttreeName);
        branchPrimaryVert = tree->GetBranch("PrimaryVertex");
        branchPrimaryVert->SetAddress(&vert.X);
        int numPrimaryVertex = branchPrimaryVert->GetEntries();
        for (int vP = 0; vP<numPrimaryVertex;vP++){
            branchPrimaryVert->GetEvent(vP);
            auto mult = vert.mult;
            auto zP = vert.Z;
            auto eventP = vert.eventID;
            branchRecoVert->GetEvent(vR);
            auto zR = recVertex.Zr;
            auto eventR = recVertex.eventID;
            double maximumZtrue = (sigmaPrimaryVertex)*(conf->sigmasNumber);
            if(abs(zP)<=maximumZtrue){  // Only if the vertex Ztrue is in a certain range selected by the user. (ex: 1sigma, 2sigma, 3sigma...)
                if(eventP==eventR){
                    multZtrueAll->Fill(zP,mult);
                    multZtrueReco->Fill(zP,mult);
                    double res = zP - zR;
                    if(abs(res)>maxResidual){
                        maxResidual = abs(res);
                    }
                    vR = vR + 1;
                }
                else{
                    multZtrueAll->Fill(zP,mult);
                }
            } 
            else{
                if(eventP==eventR){
                    vR = vR + 1;
                }
            }          
        }  
    }

    // Plot of the efficiency as a function of the vertex Ztrue
    TGraphErrors *efficiencyZtrue = new TGraphErrors(zTrueNum);
    TAxis *xaxis = multZtrueAll->GetXaxis();
    TH1D * zTrueAll;  //TH1D of "slices" of the first TH2D (which contains all the primary vertices)
    TH1D * zTrueReco;   //TH1D of "slices" of the first TH2D (which contains only the primary vertices that have benn reconstructed)
    for(long unsigned int u = 0;u<zTrueNum;u++){        //loop over all the ranges of Ztrue considered 
       int binStart = xaxis->FindBin(zTrueLeftLimit[u]);        //If finds the bin corresponding to the left limit of the Ztrue range
       int binStop = xaxis->FindBin(zTrueRightLimit[u]);          //If finds the bin corresponding to the right limit of the Ztrue range
       char name1[60];
       sprintf(name1,"zTrueAll %lx",u);
       zTrueAll= new TH1D(name1,"zTrueAll",200,minMult,maxMult);
                                                   
       zTrueAll= multZtrueAll->ProjectionY("zTrueAll",binStart,binStop);  //projection on the multiplicity axis for a certain range of the Ztrue axis (distribuzione marginale)
       int numAll = zTrueAll->GetEntries();
       char name2[60];
       sprintf(name2,"zTrueReco %lx",u);
       zTrueReco = new TH1D(name2,"zTrueReco",200,minMult,maxMult); 
       zTrueReco = multZtrueReco->ProjectionY("zTrueReco",binStart,binStop);  //projection on the multiplicity axis for a certain range of the Ztrue axis (distribuzione marginale)
       int numReco = zTrueReco->GetEntries();
       // The efficiency and its error are calculated and the plot is filled
       if(numAll!=0){
            double efficiency = double(numReco)/double(numAll);
            double zTrue = (zTrueRightLimit[u]+zTrueLeftLimit[u])/2;
            double sEfficiency = TMath::Sqrt(efficiency*(1-efficiency)/double(numAll));
            double sZtrue = (zTrueRightLimit[u]-zTrueLeftLimit[u])/2;
            efficiencyZtrue->SetPoint(u,zTrue,efficiency);
            efficiencyZtrue->SetPointError(u,sZtrue,sEfficiency);    
       }
       else{
            double zTrue = (zTrueRightLimit[u]+zTrueLeftLimit[u])/2;
            double sZtrue = (zTrueRightLimit[u]-zTrueLeftLimit[u])/2;
            efficiencyZtrue->SetPoint(u,zTrue,0.);
            efficiencyZtrue->SetPointError(u,sZtrue,0.); 
       }
       delete zTrueAll;
       delete zTrueReco;
    }
    efficiencyZtrue->SetMarkerSize(0.8);
    efficiencyZtrue->SetMarkerStyle(21);
    efficiencyZtrue->SetTitle("efficiency(Z_{true})");
    efficiencyZtrue->GetXaxis()->SetTitle("Z_{true} (m)");
    efficiencyZtrue->GetYaxis()->SetTitle("efficiency");
    outAnalysisFile->cd();
    efficiencyZtrue->SetName("efficiencyZtrue"); efficiencyZtrue->Write();    //The plot is saved on the analysis output file
    delete efficiencyZtrue;
    gROOT->cd();

// Plot of the efficiency as a function of the vertex Multiplicity
    TGraphErrors *efficiencyMultiplicity = new TGraphErrors(multNum);
    TAxis *yaxis = multZtrueAll->GetYaxis();
    TH1D * multAll;   //TH1D of "slices" of the second TH2D (which contains all the primary vertices)
    TH1D * multReco;   //TH1D of "slices" of the second TH2D (which contains only the primary vertices that have benn reconstructed)
    for(long unsigned int u = 0;u<multNum;u++){      //loop over all the ranges of Multiplicity considered 
       int binStart = yaxis->FindBin(multLeftLimit[u]);     //If finds the bin corresponding to the left limit of the Multiplicity range
       int binStop = yaxis->FindBin(multRightLimit[u]);      //If finds the bin corresponding to the right limit of the Multiplicity range
       if (binStart == binStop){
        std::cerr<<"\nSomething went wrong. Please run the reconstruction/analysis again"<<std::endl;
       }
       char name1[60];
       sprintf(name1,"multAll %lx",u);
       multAll= new TH1D(name1,"multAll",2000,minZtrue,maxZtrue);
       multAll = multZtrueAll->ProjectionX("multAll",binStart,binStop);    //projection on the Ztrue axis for a certain range of the Multiplicity axis (distribuzione marginale)
       int numAll = multAll->GetEntries();
       char name2[60];
       sprintf(name2,"multReco %lx",u);
       multReco= new TH1D("multReco","multReco",2000,minZtrue,maxZtrue);
       multReco = multZtrueReco->ProjectionX("multReco",binStart,binStop);     //projection on the Ztrue axis for a certain range of the Multiplicity axis (distribuzione marginale)
       int numReco = multReco->GetEntries();
       // The efficiency and its error are calculated and the plot is filled
       if(numAll != 0){
            double efficiency = double(numReco)/double(numAll);
            double mult = (multRightLimit[u]+multLeftLimit[u])/2;
            double sEfficiency = TMath::Sqrt(efficiency*(1-efficiency)/double(numAll));
            double sMult = (multRightLimit[u]-multLeftLimit[u])/2;
            efficiencyMultiplicity->SetPoint(u,mult,efficiency);
            efficiencyMultiplicity->SetPointError(u,sMult,sEfficiency);
        }
        else{
            double mult = (multRightLimit[u]+multLeftLimit[u])/2;
            double sMult = (multRightLimit[u]-multLeftLimit[u])/2;
            efficiencyMultiplicity->SetPoint(u,mult,0.);
            efficiencyMultiplicity->SetPointError(u,sMult,0.); 
       }
       delete multAll;
       delete multReco;
    }
    efficiencyMultiplicity->SetMarkerSize(0.8);
    efficiencyMultiplicity->SetMarkerStyle(21);
    efficiencyMultiplicity->SetTitle("efficiency(multiplicity)");
    efficiencyMultiplicity->GetXaxis()->SetTitle("multiplicity");
    efficiencyMultiplicity->GetYaxis()->SetTitle("efficiency");
    outAnalysisFile->cd();
    efficiencyMultiplicity->SetName("efficiencyMult"); efficiencyMultiplicity->Write();        //The plot is saved on the analysis output file

    gROOT->cd();
    delete efficiencyMultiplicity;
    delete multZtrueAll;
    delete multZtrueReco;
}





// Function for resolution analysis
void ResultsAnalysis::ResolutionAnalysis(double minZtrue, double maxZtrue,int minMult, int maxMult){
    double minZt = (double)minZtrue;
    double maxZt = (double)maxZtrue; 
    //2D histogram (x axis: vertex Ztrue, y axis: residuals, z axis: counts).
    TH2D * zTrueResidual = new TH2D("zTrueResidual","Ztrue and Residuals",100,minZt,maxZt,30000,-maxResidual,maxResidual);
    //2D histogram (x axis: vertex Multiplicity, y axis: residuals, z axis: counts).
    TH2D * multResidual = new TH2D("multResidual","Multiplicity and Residuals",200,minMult,maxMult,20000,-maxResidual,maxResidual);
    zTrueResidual->GetYaxis()->SetTitle("Residuals (m)");
    zTrueResidual->GetXaxis()->SetTitle("Z_{true} (m)");
    multResidual->GetYaxis()->SetTitle("Residuals (m)");
    multResidual->GetXaxis()->SetTitle("multiplicity");  

    int vR = 0;  
    branchRecoVert->SetAddress(&recVertex.Zr);
    char ttreeName[60];
    //The first 2D histograms are filled
    for(int l=1;l<=nVersions;l++){                                           
        sprintf(ttreeName,"PixelTracker;%d",l);
        tree = (RunManager*)simOutputFile->Get(ttreeName);
        branchPrimaryVert = tree->GetBranch("PrimaryVertex");
        branchPrimaryVert->SetAddress(&vert.X);
        int numPrimaryVertex = branchPrimaryVert->GetEntries();
        for (int vP = 0; vP<numPrimaryVertex;vP++){
            branchPrimaryVert->GetEvent(vP);
            auto mult = vert.mult;
            auto zP = vert.Z;
            auto eventP = vert.eventID;
            branchRecoVert->GetEvent(vR);
            auto zR = recVertex.Zr;
            auto eventR = recVertex.eventID;
            double maximumZtrue = (sigmaPrimaryVertex)*(conf->sigmasNumber);
            if(abs(zP)<=maximumZtrue){     // only if the Ztrue is in a specific range selected by the user
                if(eventP==eventR){
                    double res = zP - zR;
                    zTrueResidual->Fill(zP,res);
                    multResidual->Fill(mult,res);
                    vR = vR + 1;
                }
            }   
            else{
                if(eventP==eventR){
                    vR = vR + 1;
                }
            }
        }  
    }
    outAnalysisFile->cd();
    //The 2D histograms are saved on the analysis output file
    zTrueResidual->Write();    
    multResidual->Write();
    gROOT->cd();

// Plot of the resolution as a function of the Ztrue
    TGraphErrors *resolutionZtrue = new TGraphErrors(zTrueNum);
    TAxis *xaxis = zTrueResidual->GetXaxis();
    TCanvas * Canvas;
    TF1 *gaussian;  
    for(long unsigned int u = 0;u<zTrueNum;u++){          // loop over all the Ztrue ranges
       int binStart = xaxis->FindBin(zTrueLeftLimit[u]);    //number of the bin corresponding to the left limit of the Ztrue range considered
       int binStop = xaxis->FindBin(zTrueRightLimit[u]);     // number of the bin corresponding to the right limit of the Ztrue range considered
       Canvas = new TCanvas();
       Canvas->cd();
       // Starting from the 2D histogram a projection (TH1D) on the residuals axis is made considering only the Ztrue in a specific range (distribuzione marginale)
       TH1D * histogramResidualZtrue = zTrueResidual->ProjectionY("histogramResidualZtrue",binStart,binStop);   // TH1D of the "slices" of the first TH2D
       double sigma = histogramResidualZtrue->GetRMS();
       if(sigma>0.001){
          sigma = 0.001;
       }
       histogramResidualZtrue->SetAxisRange(-3*sigma, 3*sigma,"X");
       gaussian = new TF1("gaussianZtrue","gaus");
       gaussian->SetParameter(1,0.);
       gaussian->SetParameter(2,0.0005);
       histogramResidualZtrue->Fit(gaussian,"Q");     // The 1D histogram is fitted with a gaussian and the standard deviation gives the resolution
       double resolution = gaussian->GetParameter(2);
       double zTrue = (zTrueRightLimit[u]+zTrueLeftLimit[u])/2;
       double sResolution = gaussian->GetParError(2);
       double sZtrue = (zTrueRightLimit[u]-zTrueLeftLimit[u])/2;
       //The plot is filled
       resolutionZtrue->SetPoint(u,zTrue,resolution);
       resolutionZtrue->SetPointError(u,sZtrue,sResolution);
       delete histogramResidualZtrue;
       delete gaussian;
       delete Canvas;
    }
    resolutionZtrue->SetMarkerSize(0.8);
    resolutionZtrue->SetMarkerStyle(21);
    resolutionZtrue->SetTitle("resolution(Z_{true})");
    resolutionZtrue->GetXaxis()->SetTitle("Z_{true} (m)");
    resolutionZtrue->GetYaxis()->SetTitle("resolution (m)");
    outAnalysisFile->cd();
    resolutionZtrue->SetName("resolutionZtrue"); resolutionZtrue->Write();     // The plot is saved on the analysis output file
    gROOT->cd();
    delete resolutionZtrue;

// Plot of the resolution as a function of the Multiplicity
    TGraphErrors *resolutionMult = new TGraphErrors(multNum);
    TAxis *x2axis = multResidual->GetXaxis();
    TCanvas *canvas;  
    // Starting from the second 2D histogram a projection (TH1D) on the residuals axis is made considering only the Multiplicity in a specific range (distribuzione marginale)
    TF1 *gaussianMult;  
    for(long unsigned int u = 0;u<multNum;u++){          // loop over the multiplicity ranges
       int binStart = x2axis->FindBin(multLeftLimit[u]);   //number of the bin corresponding to the left limit of the Multiplicity range considered
       int binStop = x2axis->FindBin(multRightLimit[u]);    //number of the bin corresponding to the right limit of the Multiplicity range considered
       char histoTitle[60];
       sprintf(histoTitle,"Residuals: %f < multiplicity < %f",multLeftLimit[u],multRightLimit[u]);
       canvas = new TCanvas();
       canvas->cd();       
       TH1D * histogramResidualMult= multResidual->ProjectionY("histogramResidualMult",binStart,binStop);
       histogramResidualMult->SetTitle(histoTitle);
       double sigma = histogramResidualMult->GetRMS();
       if(sigma>0.001){
          sigma = 0.001;
       }
       histogramResidualMult->SetAxisRange(-3*sigma, 3*sigma,"X");
       histogramResidualMult->SetOption("pe");
       histogramResidualMult->SetMarkerStyle(21);
       histogramResidualMult->SetMarkerSize(0.5);
       gaussianMult = new TF1("gaussianMult","gaus");
       gaussianMult->SetParameter(1,0.);
       gaussianMult->SetParameter(2,0.0005);
       histogramResidualMult->Fit(gaussianMult,"Q");   // The 1D histogram is fitted with a gaussian and the standard deviation gives the resolution
       gStyle->SetOptFit(1);
       histogramResidualMult->Draw();
       double resolution = gaussianMult->GetParameter(2);
       double mult = (multRightLimit[u]+multLeftLimit[u])/2;
       double sResolution = gaussianMult->GetParError(2);
       double sMult = (multRightLimit[u]-multLeftLimit[u])/2;
       //The plot is filled 
       resolutionMult->SetPoint(u,mult,resolution);
       resolutionMult->SetPointError(u,sMult,sResolution);
       outAnalysisFile->cd();
       histogramResidualMult->Write();    // The 1D histogram of residuals in a specific range of multiplicity is saved on the analysis output file
       gaussianMult->Write();
       gROOT->cd();
       delete histogramResidualMult;
       delete gaussianMult;
       delete canvas;
    }
    resolutionMult->SetMarkerSize(0.8);
    resolutionMult->SetMarkerStyle(21);
    resolutionMult->SetTitle("resolution(mult)");
    resolutionMult->GetXaxis()->SetTitle("mult");
    resolutionMult->GetYaxis()->SetTitle("resolution (m)");
    outAnalysisFile->cd();
    resolutionMult->SetName("resolutionMult"); resolutionMult->Write();
    gROOT->cd();

// The histogram of residual considering all the possible values of multiplicity is created and saved on the analysis output file
    TCanvas *canvasAll = new TCanvas();
    canvasAll->cd();
    int binStart = x2axis->FindBin(minMult);
    int binStop = x2axis->FindBin(maxMult);
    TH1D * histogramResidualAllMult = multResidual->ProjectionY("histogramResidualMult",binStart,binStop);
    histogramResidualAllMult->SetTitle("Residual: all multiplicities");
    double sigma = histogramResidualAllMult->GetRMS();
    if(sigma>0.002){
       sigma = 0.002;
    }
    histogramResidualAllMult->SetAxisRange(-3*sigma, 3*sigma,"X");
    histogramResidualAllMult->SetOption("pe");
    histogramResidualAllMult->SetMarkerStyle(21);
    histogramResidualAllMult->SetMarkerSize(0.5);
    outAnalysisFile->cd();
    histogramResidualAllMult->Write();
    gROOT->cd();
    delete resolutionMult;
    delete canvasAll;
    delete histogramResidualAllMult;   
    delete zTrueResidual;
    delete multResidual;
}






// It calculates the min and max value for vertex Ztrue, from the data generated during simulation process. This function also extimates the standard deviation
// for the vertex Ztrue using getRMS: this value will be used during the analysis process
void ResultsAnalysis::GetTreeMinMaxZ(Double_t &min, Double_t &max, Double_t &sigmaPV){
    TH1D *his = new TH1D("histPrimaryVertex","histPrimaryVertex",5000,-1.,1.);
    char ttreeName[60];
    for(int l=1;l<=nVersions;l++){                                                                                  
        sprintf(ttreeName,"PixelTracker;%d",l);
        tree = (RunManager*)simOutputFile->Get(ttreeName);
        branchPrimaryVert = tree->GetBranch("PrimaryVertex");
        branchPrimaryVert->SetAddress(&vert.X);
        for (long long int i = 0; i < branchPrimaryVert->GetEntries(); ++i)
        {
            branchPrimaryVert->GetEvent(i);
            Double_t value = vert.Z;
            if (l == 1 && i == 0)
            {
                min = value;
                max = value;
            }
            else
            {
                if (value < min) min = value;
                if (value > max) max = value;
            }
            his->Fill(value);
        }
    }
    sigmaPV = his->GetRMS();
    delete his;
}
// It calculates the min and max value for vertex Multiplicity, from the data generated during simulation process.
void ResultsAnalysis::GetTreeMinMaxMult(Int_t &min, Int_t &max){
    char ttreeName[60];
    for(int l=1;l<=nVersions;l++){                                                           
        sprintf(ttreeName,"PixelTracker;%d",l);
        tree = (RunManager*)simOutputFile->Get(ttreeName);
        branchPrimaryVert = tree->GetBranch("PrimaryVertex");
        branchPrimaryVert->SetAddress(&vert.X);
        for (long long int i = 0; i < branchPrimaryVert->GetEntries(); ++i)
        {
            branchPrimaryVert->GetEvent(i);
            Int_t value = vert.mult;
            if(value<2){
            }
            if (l == 1 && i == 0)
            {
                min = value;
                max = value;
            }
            else
            {
                if (value < min) min = value;
                if (value > max) max = value;
            }
        } 
    }
}

