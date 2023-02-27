/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/detectorEffects.h"

#if !defined(__CLING__)
//ClassImp(DetectorEffects);
#endif

DetectorEffects::DetectorEffects()
{

}

DetectorEffects::DetectorEffects(RndEngine * rndE)
{
    SetRndEngine(rndE);
}

DetectorEffects::~DetectorEffects()
{

}

void DetectorEffects::SetRndEngine(RndEngine * rndE)
{
    rndEngine = rndE;
}

void DetectorEffects::LoadConfiguration(ProgramConfig * config)
{
    conf = config;
}

void DetectorEffects::SoftParticlePixelNoise(EventManager * event)
{
    Double_t phi_inner, phi_outer, z_inner, z_outer;
    RunManager * currentRun = event->GetRun();
    Double_t nInner, nOuter;

    nInner = conf->innerCountsDistribution->GetRandom(rndEngine);
    nOuter = conf->outerCountsDistribution->GetRandom(rndEngine);

    Double_t rInner = conf->innerSiliconRadius;
    Double_t rOuter = conf->outerSiliconRadius;

    for (unsigned int i = 0; i < nInner; ++i)
    {
        conf->innerSiliconNoise->GetRandom2(phi_inner, z_inner, rndEngine);

        //Fill the TTree!
        dhit.X = rInner * TMath::Cos(phi_inner);
        dhit.Y = rInner * TMath::Sin(phi_inner);
        dhit.Z = z_inner;
        dhit.eventID = event->GetEventID();
        dhit.particleID = 0;
        dhit.detectorID = 1;
        event->GetRun()->GetBranch("DetectorHits")->Fill();
    }

    for (unsigned int i = 0; i < nOuter; ++i)
    {
        conf->outerSiliconNoise->GetRandom2(phi_outer, z_outer, rndEngine);

        //Fill the TTree!
        dhit.X = rOuter * TMath::Cos(phi_outer);
        dhit.Y = rOuter * TMath::Sin(phi_outer);
        dhit.Z = z_outer;
        dhit.eventID = event->GetEventID();
        dhit.particleID = 0;
        dhit.detectorID = 2;
        event->GetRun()->GetBranch("DetectorHits")->Fill();
    }
    
}


