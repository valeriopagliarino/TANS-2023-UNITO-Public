/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/particleGun.h"

#if !defined(__CLING__)
//ClassImp(ParticleGun);
#endif

ParticleGun::ParticleGun()
{
   particleIDGenerator = 0;
}

ParticleGun::ParticleGun(ProgramConfig * conf)
{
    ImportConfig(conf);
    particleIDGenerator = 0;
}

ParticleGun::ParticleGun(RndEngine * rnde, ProgramConfig * conf)
{
    ImportConfig(conf);
    SetRandomEngine(rnde);
    particleIDGenerator = 0;
}

ParticleGun::ParticleGun(RndEngine * rnde)
{
    SetRandomEngine(rnde);
    particleIDGenerator = 0;
}

ParticleGun::ParticleGun(RndEngine * rnde, EventManager * currentEventObject)
{
    SetRandomEngine(rnde);
    SetCurrentEvent(currentEventObject);
    particleIDGenerator = 0;
}

ParticleGun::ParticleGun(RndEngine * rnde, EventManager * currentEventObject, ProgramConfig * conf)
{
    SetRandomEngine(rnde);
    SetCurrentEvent(currentEventObject);
    ImportConfig(conf);
    particleIDGenerator = 0;
}

ParticleGun::~ParticleGun()
{
    
}

void ParticleGun::ImportConfig(ProgramConfig * conf)
{
    //Import the pointers to the input distributions from the configuration file
    disableKin                  = conf->disableKin;
    momentumDistribution        = conf->momentumDistribution;
    etaDistribution             = conf->etaDistribution;         
    multiplicityDistribution    = conf->multiplicityDistribution;
    phiDistribution             = conf->phiDistribution;         
    bunchCrossingX              = conf->bunchCrossingX;          
    bunchCrossingY              = conf->bunchCrossingY;         
    zPosDistribution            = conf->zPosDistribution; 
    charge                      = conf->charge;
    mass                        = conf->mass;
}

void ParticleGun::SetRandomEngine(RndEngine * rnde)
{
    rndEngine = rnde;
}

void ParticleGun::SetCurrentEvent(EventManager * currentEventObject)
{
    currentEvent = currentEventObject;
}

void ParticleGun::GenerateCollision()
{
    //Generate multiplicity from distribution and vertex position

    unsigned int mult = (unsigned int)multiplicityDistribution->GetRandom(rndEngine);
    double vrtX = bunchCrossingX->GetRandom(rndEngine);
    double vrtY = bunchCrossingY->GetRandom(rndEngine);
    double vrtZ = zPosDistribution->GetRandom(rndEngine);

    //Fill the TTree
    RunManager * ttree = currentEvent->GetRun();
    currentEvent->SetVertex(vrtX, vrtY, vrtZ);
    ttree->GetBranch("PrimaryVertex")->SetAddress(&vert.X);
    vert.X = vrtX;
    vert.Y = vrtY;
    vert.Z = vrtZ;
    vert.eventID = currentEvent->GetEventID();
    ULong64_t evento = currentEvent->GetEventID();
    vert.mult = mult;
    ttree->GetBranch("PrimaryVertex")->Fill();
    //std::cerr << "\nPGUN -> injection, mult = " << mult;

    //Iterate over tracks
    for (unsigned int i = 0; i < mult; ++i)
    {
        //Generate track parameters from the input distributions
        double momentum;
        if (disableKin)
            momentum = 1e-19;
        else
            momentum = momentumDistribution->GetRandom(rndEngine);
        
        double eta = etaDistribution->GetRandom(rndEngine);
        double phi = phiDistribution->GetRandom(rndEngine);

        //Add the track to the event
        GeneratePrimaryTrack(vrtZ, vrtX, vrtY, eta, phi, momentum, mass, charge);
    }

    //Add to the caller runManager TTree all the information
    RunManager * currentRun = currentEvent->runManager;
}

void ParticleGun::GeneratePrimaryTrack(Double_t zpos, Double_t xpos, Double_t ypos, Double_t eta, Double_t azimuth, Double_t momentum, Double_t mass, Double_t charge)
{
    Double_t px, py, pz;
    Double_t theta = 2 * TMath::ATan(TMath::Exp(-1. * eta));

    px = momentum * TMath::Sin(theta) * TMath::Cos(azimuth); 
    py = momentum * TMath::Sin(theta) * TMath::Sin(azimuth);
    pz = momentum * TMath::Cos(theta);


    Track * tr = new Track(GetParticleID());
    tr->SetMomentum(px, py, pz);
    tr->SetElectricalCharge(charge);
    tr->SetMass(mass);
    tr->SetTrackStart(xpos, ypos, zpos);

    //A track is flagged as "active" if it has not "stop point", therefore it has not yet been processed by the ExperimentSimulation class
    tr->SetActiveTrack(true);
    tr->SetEvent(currentEvent, currentEvent->GetEventID());

    //sstd::cerr << "\nParticle gun created track " << tr << " ParticleID=" << tr->GetParticleID();
    currentEvent->tracks.push_back(tr);
}

unsigned long int ParticleGun::GetParticleID()
{
    particleIDGenerator++;
    return particleIDGenerator;
}