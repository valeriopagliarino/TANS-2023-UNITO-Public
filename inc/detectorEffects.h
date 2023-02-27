#ifndef DETECTOREFFECTS_H
#define DETECTOREFFECTS_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TNamed.h>
#include<TMath.h>
#include<TH1D.h> 
#include<TH2D.h>
#include<TF1.h>

#include "../inc/rndEngine.h"
#include "../inc/hit.h"
#include "../inc/conf.h"
#include "../inc/runManager.h"

/// @brief Class to simulate the effects of soft particles, detector noise etc...
class DetectorEffects : public TNamed
{
    public:
        DetectorEffects();
        DetectorEffects(RndEngine * rndE);
        ~DetectorEffects();

        void SetRndEngine(RndEngine * rndE);
        void LoadConfiguration(ProgramConfig * config);

        /// @brief This function generates the soft particle noise, according to the distributions given in the configuration file
        /// @param event Event where the additional hits will be added
        void SoftParticlePixelNoise(EventManager * event);

    private:
        static RndEngine * rndEngine;
        ProgramConfig * conf;
        

    //ClassDef(DetectorEffects, 1);
};

//Definition of static data members
RndEngine * DetectorEffects::rndEngine;

#endif
