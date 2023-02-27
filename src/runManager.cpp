/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/runManager.h"

RunManager::RunManager()
{
    
}

RunManager::RunManager(ProgramConfig * configuration, TFile * simulationCurrentFile)
{   
    //Initialize branches of the current RunManager istance, since it inherits from TTree
    this->Branch("PrimaryVertex", &vert.X, "X/D:Y/D:Z/D:mult/I:eventID/I");
    this->Branch("DetectorHits", &dhit.X, "X/D:Y/D:Z/D:eventID/l:particleID/l:detectorID/l");

    //Import the simulation configuration
    conf = configuration;
    if(!conf->IsInit())
    {
        std::cerr << "\nError: input configuration not properly initialized.\n";
        return;
    }

    //Initialize the random engine to be used for this run
    rndEngine = new RndEngine();
    rndEngine->SetSeed(conf->rndSeed);

    //Open the ROOT working file on HDD and set it as the working directory
    simCurrentFile = simulationCurrentFile;
    simCurrentFile->mkdir("events/");
    simCurrentFile->mkdir("tracks/");
    simCurrentFile->mkdir("hits/");

    //Initialize the ExperimentSimulation class istance that will propagate the primary tracks across the entire detector
    experimentSimulation = new ExperimentSimulation();
    experimentSimulation->SetConfiguration(conf);
    experimentSimulation->SetHitTree(this);
    TransportEngine::SetRandomEngine(rndEngine);

    //Build the detector geometry
    experimentSimulation->BuildGeometry();

    //Initialize the ParticleGun class istance that will generate the primary tracks from a vertex inside a given event
    particleGun = new ParticleGun(rndEngine, conf);

    //Initialize the DetectorEffects class istance that will simulate the noise effects due to soft particles and silicon pixel dark counts
    //Load the configuration inside the detectorEffects object
    detectorEffects = new DetectorEffects(rndEngine);
    detectorEffects->LoadConfiguration(conf);

    std::cerr << "\nInitialization completed.";
}

RunManager::~RunManager()
{
    //Save buffered data on TFile and close TFile
    FlushMemory();
    delete particleGun;
    delete experimentSimulation;
    delete rndEngine;
}

void RunManager::FlushMemory()
{
    //Save the data still buffered in memory on the TFile
    simCurrentFile->cd(); 
    this->Write("PixelTracker", kOverwrite, 0);
    simCurrentFile->cd();
    simCurrentFile->Flush();
}

void RunManager::Debug()
{
    Track * tr = new Track;
    tr->Write();
}

void RunManager::BeamOn()
{
    //Start the simulation backend and launch stopwatch for performance evaluation
    std::cerr << "\nRun started. \n";
    TStopwatch * tsw = new TStopwatch();

    SimulationBackend();
    tsw->Stop();

    //Print elapsed time
    std::cerr << "\nRun completed. " << conf->eventNumber << " events in ";
    tsw->Print();
    delete tsw;
}

void RunManager::SimulationBackend()
{
    //Check if single event persistence is enabled in the persistence file
    bool persist = conf->singleEventPersistenceEnabled;

    //Retrieve the number of events from the configuration file
    unsigned long int eventNum = conf->eventNumber;

    simCurrentFile->cd();
    this->SetAutoFlush(100000);
     
    //Loop over all the events
    for (unsigned long int i = 0; i < eventNum; ++i)
    {
        //Generate the current event, specify if it will be persistent (for memory allocation optimization)
        EventManager * currentEvent = new EventManager();
        currentEvent->SetPersist(persist);
        currentEvent->runManager = this;

        //Set this event as active for the particle gun used in this run
        particleGun->SetCurrentEvent(currentEvent);
        
        //Some collisions! If a single collision per event is required
        if (conf->singleCollisionInEvent)
        {
            particleGun->GenerateCollision();
        }
        else
        {
            //If we need to simulate some pile-up, TH1D with the distribution of no. of collisions per events
            int ncoll = conf->collisionPerEventDistribution->GetRandom(rndEngine);
            for (int k = 0; k < ncoll; ++k)
                particleGun->GenerateCollision();
        }

        

        //Pass the current event to the ExperimentSimulation class istance that will compute particle transport and detector hits
        experimentSimulation->ProcessEvent(currentEvent);
        //std::cerr << "\nEVENT = " << currentEvent->GetEventID();

        //Pass the current event to the DetectorEffects class istance that will simulate soft particles and noise
        if (conf->enableSoftParticlesNoise) detectorEffects->SoftParticlePixelNoise(currentEvent);


        //If single event persistence is enabled, store the event, otherwise cleanup
        if(persist)
        {

            //Save event data
            events.push_back(currentEvent);
            simCurrentFile->cd("events/");
            currentEvent->Write();

            //Save the tracks
            for (unsigned long int i = 0; i < currentEvent->tracks.size(); ++i)
            {
                simCurrentFile->cd("tracks/");
                currentEvent->tracks[i]->Write();
            }

            //Save the hits
            for (unsigned long int i = 0; i < currentEvent->hits.size(); ++i)
            {
                simCurrentFile->cd("hits/");
                currentEvent->hits[i]->Write();
            }

        }
        else
        {
            delete currentEvent;
        }

        if (i % conf->reportEvery == 0)
            std::cerr << "\nEvent " << i << " completed.  ";

        if ((i % 50000 == 0) && (i != 0))
        {
            this->FlushBaskets();
            this->FlushMemory();
            simCurrentFile->Flush();
            std::cerr << "  -> Flushing tree baskets to the storage";
        }


        if ((i % 200000 == 0) && (i != 0))   // after debug 200000
        {
            this->FlushBaskets();
            this->FlushMemory();
            simCurrentFile->Flush();
            this->AutoSave();
            this->Write("PixelTracker");
            this->Reset();
            this->FlushMemory();
            std::cerr << "  -> Writing objects";
        }
    }

    //Save the sensitive detector hit (FAST2 sim data) recorded in the TTree
    this->StartViewer();
    simCurrentFile->cd("/");
    this->FlushBaskets();
    this->FlushMemory();
    simCurrentFile->Flush();

    
    //Save a copy of the configuration in the output TFile
}
