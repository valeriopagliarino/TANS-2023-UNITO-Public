#ifndef CLI_H
#define CLI_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<string>

#include<TObject.h>
#include<TFile.h>
#include<TString.h>

#include "../inc/conf.h"
#include "../inc/runManager.h"
#include "../inc/eventDisplay.h"
#include "../inc/vertexReco.h"

class Cli
{
    public: 
        Cli();
        ~Cli();

        static void Start(TString params, TString configurationFilePath);
        static VertexReco * Reconstruction(TString configurationFilePath);
        static RunManager * Simulation(TString configurationFilePath, bool persist);
        static EventDisplay * DrawEvent(RunManager * rm, unsigned long int eventID);
        static EventDisplay * DrawEvent(unsigned long int eventID);

        static ProgramConfig * conf;
        static RunManager * currentRun;

    private:
        static bool currentRunAllocated;
        static bool configAllocated;
};

//Definition of static data members
ProgramConfig * Cli::conf;
RunManager * Cli::currentRun;
bool Cli::currentRunAllocated;
bool Cli::configAllocated;

#endif