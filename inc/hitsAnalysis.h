#ifndef HITSANALYSIS_H
#define HITSANALYSIS_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/


/// @brief This class analyzes the hits obtained from the simulation and reconstructs vertex position

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TROOT.h>

#include "TNamed.h"
#include "../inc/runManager.h"
#include "../inc/conf.h"

class HitsAnalysis : public TNamed
{
    public:
        HitsAnalysis(ProgramConfig * config);
        ~HitsAnalysis();
        void ImportConfiguration(ProgramConfig * config);
        void CalculateZrec(TFile * simFile, int nTTreeVersions,double dPhiMax);
        

    private:
        RunManager *tree;
        TBranch * branchHits;
        double deltaPhiMax;
        Double_t runningW;
        Double_t runningWStep;
        double limitPercentMaxNumVert;
        int minNumVertInWindow;
        ULong64_t currentEvent;
        std::vector<double> vecZtracklets;
        std::vector<int> numZinWindow;
        Double_t zRec;
        ProgramConfig * conf;
        TFile * outRecoFile;
        TTree * outTree;

        void GetIntersections(std::vector<double> vX1,std::vector<double> vX2,std::vector<double> vY1,std::vector<double> vY2,std::vector<double> vZ1,std::vector<double> vZ2);
        bool ReconstructZ();
        
};
//Definition of static Data Member
  typedef struct {
    Double_t Zr;
    Int_t eventID;} VTXR;
  static VTXR recVertex;


#endif