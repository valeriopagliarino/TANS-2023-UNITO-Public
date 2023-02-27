#ifndef CALCULATEPHIMAX_H
#define CALCULATEPHIMAX_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/
/// @brief This class calculates the value of phi max, that will be used to reconstruct tracklets, starting from simulated data 

#include <TNamed.h>
#include <TH1D.h>
#include <TBranch.h>
#include "TNamed.h"

#include "../inc/runManager.h"

class CalculateDeltaPhiMax : public TNamed
{
    public:
        CalculateDeltaPhiMax();
        ~CalculateDeltaPhiMax();
        
        double ObtainDeltaPhiMax(TFile * simFile);

    private:
        TH1D * phiHist;
        TBranch * branchHits;
        RunManager * tree;

        void HistogramFiller(std::vector<double> v1,std::vector<double> v2);

};


#endif