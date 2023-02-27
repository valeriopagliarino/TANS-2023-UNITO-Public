#ifndef TRANSPORTENGINE_H
#define TRANSPORTENGINE_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TObject.h>
#include<TVector3.h>
#include<TMath.h>
#include<TH1D.h>
#include<TF1.h>

#include "../inc/track.h"
#include "../inc/rndEngine.h"

/// @brief This static class implements particle transport through the materials of the detector
class TransportEngine : public TObject
{
    public:
        TransportEngine();
        ~TransportEngine();

        //Set RndEngine
        static void SetRandomEngine(RndEngine * rndE);

        //Multiple scattering process
        /// @brief This functions is used to model the multiple scattering effect between high energy charged particles and thin layers of materials. It uses the Highland formula to compute the average theta scattering angle and it performs coordinate transformations. (https://doi.org/10.1016/0168-583X(91)95671-Y)
        /// @param incomingTrack Pointer to the incoming track. After interacting with the layer of material, the track "Active" attribute will be turned to false and the tracking is stopped.
        /// @param outgoingTrack Pointer to the outgoing active track.
        /// @param interactionPoint Hit object that represents the point of interaction between the track and the material.
        /// @param zMat Z of the material
        /// @param x Tickness of the material
        /// @param xr Radiation length of the material
        /// @param thetaMsAp Enable the zero order approximation of the theta angle (=1 mrad), instead of the Highland formula, for rough and fast simulations
        /// @param kinematics Enable the relativistic kinematics calulations. If false, only geometric trajectory tracking is performed.
        static void MultipleScattering(Track * incomingTrack, Track * &outgoingTrack, TVector3 * interactionPoint, Int_t zMat, Double_t x, Double_t xr, bool thetaMsAp = true, bool kinematics = true);

        //Bethe-Bloch equation ionization (NOT YET IMPLEMENTED!)
        static void Ionization(Track * incomingTrack, Track * outgoingTrack);

    private:
        static RndEngine * rndEngine;

        //Multiple scattering process components
        static void Rotate(Double_t th, Double_t ph, Double_t thp, Double_t php, Double_t * cd);

        //Bethe-Bloch equation ionization components
        
};

//Definition of static class member
RndEngine * TransportEngine::rndEngine;

#endif