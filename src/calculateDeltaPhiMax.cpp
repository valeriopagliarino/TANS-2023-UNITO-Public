/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include <TMath.h>

#include "../inc/calculateDeltaPhiMax.h"

//This class calculates an estimate of the deltaPhi between the two detectors for 2 hits due to the same particle using MC truth

CalculateDeltaPhiMax::CalculateDeltaPhiMax(){

      phiHist = new TH1D("DeltaPhiHist","Delta Phi Histogram",4000,-1,1);
     
}

CalculateDeltaPhiMax::~CalculateDeltaPhiMax(){
        delete phiHist;

}

double CalculateDeltaPhiMax::ObtainDeltaPhiMax(TFile * simFile){
      tree = (RunManager*)simFile->Get("PixelTracker;1");
      branchHits = tree->GetBranch("DetectorHits");
      branchHits->SetAddress(&dhit.X);
      std::vector<double> vecPhi1,vecPhi2;
      vecPhi1.reserve(20);
      vecPhi2.reserve(20);
      int i = 0;
      int j = 0;
      int entries = branchHits->GetEntries();
      branchHits->GetEvent(entries -2 );
      int init = dhit.eventID;    //init is the number of events considered for this analysis. It is not necessary to use all the events, but only a number that allows to have enough statistics
      if(init >= 200){
            init = 200;
      }
      branchHits->GetEvent(j);
      auto currentEvent = dhit.eventID;
      while(i<init){
            while(dhit.eventID == currentEvent){
                  if (dhit.particleID != 0){
                        double X1, X2, Y1, Y2;
                        if(dhit.detectorID == 1){
                              X1=dhit.X;
                              Y1=dhit.Y;
                              double phi1;
                              phi1=TMath::ATan(X1/Y1);
                              if(X1<0) phi1=phi1+TMath::Pi();
                              else if (X1>0 && Y1<0) phi1=phi1 + 2*TMath::Pi();
                              vecPhi1.push_back(phi1);
                        }
                        else if (dhit.detectorID == 2){
                              X2=dhit.X;
                              Y2=dhit.Y;
                              double phi2;
                              phi2=TMath::ATan(X2/Y2);
                              if(X2<0) phi2=phi2+TMath::Pi();
                              else if (X2>0 && Y2<0) phi2=phi2 + 2*TMath::Pi();  
                              vecPhi2.push_back(phi2);                            
                        }                
                  }
                  
                  j=j+1;
                  branchHits->GetEvent(j);
            }
            if(vecPhi1.size()==vecPhi2.size()){
                  HistogramFiller(vecPhi1,vecPhi2);      //The values of deltaPhi calculeted are used to fill an histogram. 
            }
            vecPhi1.clear();
            vecPhi2.clear();
            currentEvent = dhit.eventID;
            i=i+1;

      }
      double deltaPhiMax = phiHist->GetRMS();
      deltaPhiMax=3*deltaPhiMax;             // The value of DeltaPhiMax is estimated as 3 times the standard deviation of the data in the histogram
      return deltaPhiMax;
}

void CalculateDeltaPhiMax::HistogramFiller(std::vector<double> v1,std::vector<double> v2){
      int q = v1.size();
      double deltaPhi;
      for(int k=0;k<q;k++){
            deltaPhi=v2[k]-v1[k];
            phiHist->Fill(deltaPhi);
      }
}