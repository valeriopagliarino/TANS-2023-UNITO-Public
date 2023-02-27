#ifndef VERTEXRECO_H
#define VERTEXRECO_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TNamed.h>
#include<TFile.h>
#include <TKey.h>

#include "../inc/conf.h"
#include "../inc/calculateDeltaPhiMax.h"
#include "../inc/hitsAnalysis.h"
#include "../inc/runManager.h"
#include "../inc/resultsAnalysis.h"

/// @brief This class manages the vertex reconstruction
class VertexReco : public TNamed
{
    public:
        VertexReco();
        VertexReco(ProgramConfig * cnf);
        ~VertexReco();
        void MakeReconstruction();

        
    private:
        
        ProgramConfig * conf;
        CalculateDeltaPhiMax * calculateDeltaPhiMax;
        HitsAnalysis * hitsAnalysis;
        ResultsAnalysis * resultsAnalysis;
        double deltaPhiMax;

};
static TFile * simOutputFile; //Input file from simulation
#endif