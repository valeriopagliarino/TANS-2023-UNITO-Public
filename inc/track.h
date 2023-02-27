#ifndef TRACK_H
#define TRACK_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

//Forward declarations
class Track;
class EventManager;
class RunManager;
class TransportEngine;
class ExperimentSimulation;
class DetectorEffects;
class ParticleGun;

#include<TNamed.h>
#include<TObject.h>
#include<TMath.h>
#include<Math/Vector3D.h>
#include<Math/Vector4D.h>
#include<Math/Vector3Dfwd.h>

#include "../inc/units.h"
#include "../inc/hit.h"

/// @brief Class for representing the track of a single particle with given mass and momentum between two points
class Track : public TObject
{
    public:
        //Constructor and destructor
        Track();
        Track(unsigned long int particleIDnum);
        Track(const Track &Particle);
        ~Track();

        //Setter methods
        void SetMomentum(Double_t momentum_x, Double_t momentum_y, Double_t momentum_z);
        void SetTrackStart(Double_t pos_x, Double_t pos_y, Double_t pos_z);
        void SetTrackStop(Double_t pos_x, Double_t pos_y, Double_t pos_z);
        void SetElectricalCharge(Int_t charge_in_e_units);
        void SetMass(Double_t particle_rest_mass);
        void SetActiveTrack(bool status = false);
        void SetParticleID(unsigned long int particleIdentifier);
        void SetNoStop(bool stop);

        void SetEvent(EventManager * eventIdentifier, unsigned long int eventIdNum);

        //Getter methods
        unsigned long int GetParticleID();

        EventManager * GetEvent();

        Double_t GetMomentum();
        
        Double_t GetMomentumX();
        Double_t GetMomentumY();
        Double_t GetMomentumZ();
        void     GetMomentum(Double_t &px, Double_t &py, Double_t &pz);
        
        Double_t GetTrackStartX();
        Double_t GetTrackStartY();
        Double_t GetTrackStartZ();

        Double_t GetTrackStopX();
        Double_t GetTrackStopY();
        Double_t GetTrackStopZ();

        Double_t GetMass();
        Int_t    GetCharge();

        Bool_t     isActive();
        Bool_t     GetNoStop();

        //Tracks are indexed using a unique identifier that is their memory address
        ULong64_t GetTrackId(); 
        ULong64_t GetEventId(); 

        //Getter methods requiring computation
        Double_t GetVelocityX();
        Double_t GetVelocityY();
        Double_t GetVelocityZ();
        Double_t GetGamma();
        Double_t GetBeta();
        Double_t GetEnergy();


    private:
        bool trackingActive = true; 
        //If false the particle has already been absorbed or decayed and should be ignored
        // by the next iteration of the transport code

        bool noStop = false;
        //If true, the particle does not have a stop point

        ULong64_t particleID;
        ULong64_t eventID;

        EventManager * currentEvent;  //!

        Double_t px;        //Momentum vector magnitude
        Double_t py;        //Momentum vector magnitude
        Double_t pz;        //Momentum vector magnitude

        Double_t ax;        //Track start point
        Double_t ay;        //Track start point
        Double_t az;        //Track start point

        Double_t bx;        //Track stop point
        Double_t by;        //Track stop point
        Double_t bz;        //Track stop point

        Double_t m;         //Particle mass at rest
        Double_t q;         //Particle charge

        Double_t LorentzGamma();                                         //Compute the Lorentz gamma
        Double_t Energy();                                               //Compute particle energy

    ClassDef(Track, 1)
};

//Directives for dictionary generation by ROOTCLING for ACliC
#ifdef __ROOTCLING__
#pragma link C++ class Track;
#endif

#endif



