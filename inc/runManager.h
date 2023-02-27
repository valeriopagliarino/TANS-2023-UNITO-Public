#ifndef RUNMANAGER_H
#define RUNMANAGER_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<vector>
#include<string>
#include<iostream>

#include<TSystem.h>
#include<TNamed.h>
#include<TClonesArray.h>
#include<TTree.h>
#include<TBranch.h>
#include<TFile.h>
#include<TStopwatch.h>

#include "../inc/eventManager.h"
#include "../inc/particleGun.h"
#include "../inc/experimentSimulation.h"
#include "../inc/conf.h"
#include "../inc/detectorEffects.h"


/// @brief This class contains the settings, output and data analysis of single run
class RunManager : public TTree
{
    public:
        RunManager();
        RunManager(ProgramConfig * configuration, TFile * simulationCurrentFile);
        ~RunManager();

        //Total entries on the detectors for debug purposes
        int gDet1Counts = 0;
        int gDet2Counts = 0;

        void BeamOn();
        void FlushMemory();
        void Debug(); //[DEBUG]
        ExperimentSimulation * GetExperimentSimulation() {return experimentSimulation;}
        EventManager * GetEvent(unsigned long int index) {return events[index];}

    private:
        TFile * simCurrentFile;
        ProgramConfig * conf;

        static RndEngine * rndEngine;
        ExperimentSimulation * experimentSimulation;
        ParticleGun * particleGun;
        DetectorEffects * detectorEffects;
        std::vector<EventManager *> events;
        MemInfo_t memInfo;

        void SimulationBackend();

};

//Definition of static data members
RndEngine * RunManager::rndEngine;

typedef struct{
    Double_t X;
    Double_t Y;
    Double_t Z;
    Int_t mult;
    Int_t eventID;
    } Vertex;

typedef struct{
    Double_t X;
    Double_t Y;
    Double_t Z;
    ULong64_t eventID;
    ULong64_t particleID;
    ULong64_t detectorID;
    } DetHit;


//These are globals used to access the TTrees
Vertex vert;
DetHit dhit;

#endif