#ifndef RESULTSANALYSIS_H
#define RESULTSANALYSIS_H

/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

/// @brief This class analyses the results obtained with the reconstruction and makes a comparison with the MC truth

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TF1.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TH2D.h>
#include <TAxis.h>
#include <TProfile.h>

#include "../inc/conf.h"
#include "../inc/runManager.h"
#include "../inc/hitsAnalysis.h"

class ResultsAnalysis : public TObject{

    public:
        ResultsAnalysis(ProgramConfig * givenConf);
        ~ResultsAnalysis();
        void CompareResults(TFile * simFile, int nTTreeVersions);
        void ImportConfiguration(ProgramConfig * gConf);
        void GetTreeMinMaxZ(Double_t &min, Double_t &max, Double_t &sigmaPV);
        void GetTreeMinMaxMult(Int_t &min, Int_t &max);


    private:
        ProgramConfig * conf;
        TFile * outRecoFile;
        TFile * outAnalysisFile;
        TFile * simOutputFile;
        TTree * recoTree;
        RunManager * tree;
        TBranch * branchPrimaryVert;
        TBranch * branchRecoVert;
        std::vector<double> zTrueLeftLimit, zTrueRightLimit;               // 2 vettori con i limiti superiori e inferiori dei range di Ztrue che saranno considerati nell'analisi
        std::vector<double> multLeftLimit, multRightLimit;                  // 2 vettori con i limiti superiori e inferiori dei range di mult che saranno considerati nell'analisi
        long unsigned int zTrueNum, multNum;                             //numero di ranges di zTrue e di mult considerati nell'analisi
        double sigmaPrimaryVertex;                                      //sigma del vertice perimario
        int nVersions;
        double maxResidual = 0;

        void EfficiencyAnalysis(double minZtrue, double maxZtrue,int minMult, int maxMult);
        void ConfigureMultZtrueRanges(int lowestMul,int highestMul);
        void FindHistogramsRanges(double zTrue,double multiplicity,double residual);
        void ResolutionAnalysis(double minZtrue, double maxZtrue,int minMult, int maxMult);




};

#endif