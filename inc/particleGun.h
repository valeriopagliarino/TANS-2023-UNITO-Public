#ifndef PARTICLEGUN_H
#define PARTICLEGUN_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TNamed.h>

#include "../inc/rndEngine.h"
#include "../inc/eventManager.h"
#include "../inc/conf.h"
#include "../inc/track.h"

/// @brief This class is intended to be used as a "singleton" that represents the engine generating collision using Monte Carlo distributions
class ParticleGun : public TNamed
{
    public:
        ParticleGun();
        ParticleGun(ProgramConfig * conf);
        ParticleGun(RndEngine * rnde);
        ParticleGun(RndEngine * rnde, ProgramConfig * conf);
        ParticleGun(RndEngine * rnde, EventManager * currentEventObject);
        ParticleGun(RndEngine * rnde, EventManager * currentEventObject, ProgramConfig * conf);
        ~ParticleGun();

        void UpdateConfig(ProgramConfig * conf);
        void SetRandomEngine(RndEngine * rnde);
        void SetCurrentEvent(EventManager * currentEventObject);
        void GenerateCollision();
        unsigned long int GetParticleID();
        void ImportConfig(ProgramConfig * conf);

    private:
        EventManager * currentEvent;
        static RndEngine * rndEngine;
        void GeneratePrimaryTrack(Double_t zpos = 0, Double_t xpos = 0, Double_t ypos = 0, Double_t eta = 0, Double_t azimuth = 0, Double_t momentum = 0, Double_t mass = 0, Double_t charge = 0);

        TH1D * momentumDistribution;
        TH1D * etaDistribution;
        TH1D * multiplicityDistribution;
        TH1D * phiDistribution;
        TH1D * bunchCrossingX;
        TH1D * bunchCrossingY;
        TH1D * zPosDistribution;

        double mass;
        double charge;
        bool disableKin;

        static unsigned long int particleIDGenerator;

    #if !defined(__CLING__)
    //ClassDef(ParticleGun, 1);
    #endif
};

//Definition of static data members
RndEngine * ParticleGun::rndEngine;
unsigned long int ParticleGun::particleIDGenerator;

#endif