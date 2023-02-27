/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<iostream>
#include<string>
#include<stdlib.h>
#include<stdio.h>

#include "../inc/cli.h"

using namespace std;

Cli::Cli()
{
    
}

Cli::~Cli()
{
    
}

void Cli::Start(TString params, TString configurationFilePath)
{
    //Parsing arguments....
    if(params.Contains("sim"))
    {
        if(params.Contains("persist"))
            Cli::Simulation(configurationFilePath, true);
        else
            Cli::Simulation(configurationFilePath, false);
        return;
    }
        
    if(params.Contains("rec"))
    {
        Cli::Reconstruction(configurationFilePath);
        return;
    }
    
    std::cerr << "\nPlease call simulation or reconstruction function.\n\n";
}

RunManager * Cli::Simulation(TString configurationFilePath = "nn", bool persist = false)
{
    std::cerr << "\n\n\033[1mSimulation mode initialized.\033[0m\n";

    //Setup configuration

    if (!configAllocated)
    {
        conf = new ProgramConfig();

        if (gSystem->AccessPathName(configurationFilePath))
        {
            conf->LoadDebugData();//[DEBUG] 
        }
        else
        {
            conf->LoadDebugData();//[DEBUG] 
            conf->SetFilename(std::string(configurationFilePath.Data()));
            conf->ReadConfigurationFile();
            if (persist) conf->singleEventPersistenceEnabled = persist;
        }
        configAllocated = true;
    }
    else
    {
        std::cerr << "\nWarning! Multiple runs inside a single istance of this program";
        std::cerr << "\nis allowed only if they have the same experimental setup, described";
        std::cerr << "\nin the configuration file. If you need to change number of events";
        std::cerr << "\nor other parameters, please use Cli::GetConfiguration()->...";
    }
    

    if(currentRunAllocated) delete currentRun;

    //Setup a TFile
    auto * simCurrentFile = new TFile(conf->simRootFileName.c_str(),"recreate");
    //simCurrentFile->SetCompressionSettings(50.);
    //simCurrentFile->SetBufferSize(1000000);
    simCurrentFile->cd();

    //Create a new run
    currentRun = new RunManager(conf,simCurrentFile);

    //Beam on!
    currentRun->BeamOn();

    simCurrentFile->Flush();
    simCurrentFile->Close();
    std::cerr << "\n\n";
    std::cerr << "\ngDet2Counts=" << currentRun->gDet2Counts;
    std::cerr << "\ngDet1Counts=" << currentRun->gDet1Counts;
    return currentRun;
}

VertexReco * Cli::Reconstruction(TString configurationFilePath = "nn")
{
    if (!configAllocated)
    {
        conf = new ProgramConfig();

        if (gSystem->AccessPathName(configurationFilePath))
        {
            conf->LoadDebugData();
        }
        else
        {
            conf->LoadDebugData();
            conf->SetFilename(std::string(configurationFilePath.Data()));
            conf->ReadConfigurationFile();
        }
        configAllocated = true;
    }
    
    VertexReco * vrtReco = new VertexReco(conf);
    vrtReco->MakeReconstruction();
    return vrtReco;
}

EventDisplay * Cli::DrawEvent(RunManager * rm, unsigned long int eventID)
{
    EventDisplay *  ed = new EventDisplay();
    ed->LoadExperimentGeometry(rm->GetExperimentSimulation());
    ed->LoadEvent(rm->GetEvent(eventID -1 ));
    ed->DrawEvent();
    return ed;
}

EventDisplay * Cli::DrawEvent(unsigned long int eventID)
{
    if (!conf->singleEventPersistenceEnabled)
    {
        std::cerr << "\n\nError: the event display is supported only if the track persistence is enabled.";
        return nullptr;
    }
    EventDisplay *  ed = new EventDisplay();
    ed->LoadExperimentGeometry(currentRun->GetExperimentSimulation());
    ed->LoadEvent(currentRun->GetEvent(eventID -1 ));
    ed->DrawEvent();
    return ed;
}
