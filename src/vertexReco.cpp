/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/vertexReco.h"
#include "../inc/cli.h"
#include <Riostream.h>

//This class manages all the process of reconstruction and analysis
VertexReco::VertexReco(ProgramConfig * cnf){
     conf = cnf;
     simOutputFile = new TFile(conf->simRootFileName.c_str());
     
      
     hitsAnalysis = new HitsAnalysis(cnf); // class that manages the reconstruction process
     resultsAnalysis = new ResultsAnalysis(conf); // class that manages the analysis process
}


VertexReco::~VertexReco(){
     
     simOutputFile->Close();
     delete simOutputFile;
     delete hitsAnalysis;
     delete resultsAnalysis;
}

void VertexReco::MakeReconstruction(){

     int count = 0;
     TKey *key;
     TIter nextkey(simOutputFile->GetListOfKeys());
     while((key = (TKey*)nextkey())){                   // It counts how many versions of the same TTree are present in the output .root file from simulation
         if (strcmp(key->GetName(),"PixelTracker")==0){
         count++;
         }
     }
     if((conf->enableDeltaPhiMaxCalculation) == true){  // If the function is enabled by user, it calculetes deltaPhiMax through the MC truth
          CalculateDeltaPhiMax * calculateDeltaPhiMax = new CalculateDeltaPhiMax();   //This class calculetes deltaPhiMax
          deltaPhiMax = calculateDeltaPhiMax->ObtainDeltaPhiMax(simOutputFile);
          delete calculateDeltaPhiMax;
     }
     else{  // otherwise deltaPhiMax is set to 20 mrad
          deltaPhiMax = 0.020;
     }
     hitsAnalysis->CalculateZrec(simOutputFile,count,deltaPhiMax);  //  member function of hitsAnalysis that manages the reconstruction
 
     resultsAnalysis->CompareResults(simOutputFile,count);   // member function of resultsAnalysis that manages the analysis of the results and compares them with the MC truth
     
}