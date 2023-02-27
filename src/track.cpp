/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/track.h"

ClassImp(Track)

Track::Track()
{

}

Track::Track(unsigned long int particleIDnum)
{
    particleID = particleIDnum;
}

Track::Track(const Track &Particle) : TObject()
{   
    SetTrackStart(Particle.ax, Particle.ay, Particle.az);
    SetTrackStop(Particle.bx, Particle.by, Particle.bz);
    SetMomentum(Particle.px, Particle.py, Particle.pz);
    SetElectricalCharge(Particle.q);
    SetMass(Particle.q);
    SetEvent(Particle.currentEvent, Particle.eventID);
    trackingActive = Particle.trackingActive;
    particleID = Particle.particleID;
}

Track::~Track()
{
    // [MEM LEAK CHECK] std::cerr << "\nDelete track invocated!";
}

void Track::SetMomentum(Double_t momentum_x, Double_t momentum_y, Double_t momentum_z)
{
    px = momentum_x;
    py = momentum_y;
    pz = momentum_z;
}

void Track::SetTrackStart(Double_t pos_x, Double_t pos_y, Double_t pos_z)
{
    ax = pos_x;
    ay = pos_y;
    az = pos_z;
}

void Track::SetTrackStop(Double_t pos_x, Double_t pos_y, Double_t pos_z)
{
    bx = pos_x;
    by = pos_y;
    bz = pos_z;
}

void Track::SetElectricalCharge(Int_t charge_in_e_units)
{
    q = charge_in_e_units;
}

void Track::SetMass(Double_t particle_rest_mass)
{
    m = particle_rest_mass;
}

void Track::SetActiveTrack(bool status)
{
    trackingActive = status;
}

void Track::SetEvent(EventManager * eventIdentifier, unsigned long int eventIdNum)
{
    currentEvent = eventIdentifier;
    eventID = eventIdNum;
}

void Track::SetParticleID(unsigned long int particleIdentifier)
{
    particleID = particleIdentifier;
}

void Track::SetNoStop(bool stop)
{
    noStop = stop;
}

unsigned long int Track::GetParticleID()
{
    return particleID;
}

EventManager * Track::GetEvent()
{
    return currentEvent;
}

Double_t Track::GetMomentum()
{
    return TMath::Sqrt(px*px + py*py + pz*pz);
}

Double_t Track::GetMomentumX()
{
    return px;
}
Double_t Track::GetMomentumY()
{
    return py;
}
Double_t Track::GetMomentumZ()
{
    return pz;
}

void Track::GetMomentum(Double_t &ppx, Double_t &ppy, Double_t &ppz)
{
    ppx = px;
    ppy = py;
    ppz = pz;
}

Double_t Track::GetTrackStartX()
{
    return ax;
}

Double_t Track::GetTrackStartY()
{
    return ay;
}

Double_t Track::GetTrackStartZ()
{
    return az;
}

Double_t Track::GetTrackStopX()
{
    return bx;
}

Double_t Track::GetTrackStopY()
{
    return by;
}

Double_t Track::GetTrackStopZ()
{
    return bz;
}

Double_t Track::GetMass()
{
    return m;
}

Int_t Track::GetCharge()
{
    return q;
}

Bool_t Track::isActive()
{
    return trackingActive;
}

Bool_t Track::GetNoStop()
{
    return noStop;
}

ULong64_t Track::GetTrackId()
{
    return particleID;
}

ULong64_t Track::GetEventId()
{
    return eventID;
}
    
Double_t Track::GetVelocityX()
{
    return px / (GetGamma() * m);
}

Double_t Track::GetVelocityY()
{
    return py / (GetGamma() * m);
}

Double_t Track::GetVelocityZ()
{
    /*Double_t p2 = px * px + py * py + pz * pz; 
    Double_t v = -1. * m + TMath::Sqrt(m*m + 4*p2*p2 / (c*c));
    v = v / (2*p2 / (c*c));
    Double_t gamma = TMath::Sqrt(1./(1 - (v*v / c*c))); */

    return pz / (GetGamma() * m);
}

Double_t Track::GetGamma()
{
    Double_t p2 = px*px + py*py + pz*pz;
    return TMath::Sqrt(1 + p2/(m*m*c*c));
}

Double_t Track::GetBeta()
{
    Double_t gammainv = 1./ GetGamma();
    return TMath::Sqrt(1 - gammainv * gammainv);
}

Double_t Track::GetEnergy()
{
    Double_t p2 = px * px + py * py + pz * pz; 
    return TMath::Sqrt(m*m*c*c*c*c + p2*c*c); 
}