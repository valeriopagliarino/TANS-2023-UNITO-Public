/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/transportEngine.h"

TransportEngine::TransportEngine()
{
    
}

TransportEngine::~TransportEngine()
{
    //Do not deallocate the RandomEngine, since it is shared with all the other components of the simulation!
    //It will be deallocated on deletion of RunManager class object
}

void TransportEngine::SetRandomEngine(RndEngine * rndE)
{
    rndEngine = rndE;
}

void TransportEngine::MultipleScattering(Track * incomingTrack, Track * &outgoingTrack, TVector3 * interactionPoint, Int_t zMat, Double_t x, Double_t xr, bool thetaMsAp, bool kinematics)
{
    //Compute the offset due to multiple scattering in thick material
    Double_t deltaX, deltaY, deltaZ;
    //In this case thin material approximation is good enough
    deltaX = 0;
    deltaY = 0;
    deltaZ = 0;

    //Setup the origin of the new track
    incomingTrack->SetTrackStop(interactionPoint->X(), interactionPoint->Y(), interactionPoint->Z());
    outgoingTrack->SetTrackStart(interactionPoint->X() + deltaX,
                                 interactionPoint->Y() + deltaY,
                                 interactionPoint->Z() + deltaZ);

    //Setup the momentum of the outgoing track
    Double_t ipx, ipy, ipz; //incoming
    Double_t opx, opy, opz; //outgoing

    incomingTrack->GetMomentum(ipx, ipy, ipz);

    Double_t theta0;

    if ((thetaMsAp) || (kinematics == false))
        theta0 = .001; // 1 mrad 
    else
    {
        theta0 = (13.6 * MeV) / (incomingTrack->GetBeta() * c * incomingTrack->GetMomentum());
        theta0 = theta0 * zMat * TMath::Sqrt(x / xr) * (1 + 0.038 * TMath::Log(x / xr));
    }

    Double_t phiP = rndEngine->Rndm() * 2*pi;
    Double_t thetaP = rndEngine->Gaus(0., theta0);

    Double_t momentumNorm = TMath::Sqrt(ipx*ipx + ipy*ipy + ipz*ipz);

    Double_t cd[3];

    /*
    x = sin(theta) * cos(phi)
    y = sin(theta) * sin(phi)
    z = cos(theta)
    */

    Double_t theta = TMath::ACos(ipz / momentumNorm);
    Double_t phi = TMath::ACos((ipx / momentumNorm) / TMath::Sin(theta));
    if(ipy < 0) phi = 2*TMath::Pi() - phi;

    Rotate(theta, phi, thetaP, phiP, cd);

    opx = momentumNorm * cd[0];
    opy = momentumNorm * cd[1];
    opz = momentumNorm * cd[2];

    
    
    
    outgoingTrack->SetMomentum(opx, opy, opz);

    //Switch the active track
    incomingTrack->SetActiveTrack(false);
    outgoingTrack->SetActiveTrack(true);

    //Copy metadata
    outgoingTrack->SetParticleID(incomingTrack->GetParticleID());
    outgoingTrack->SetEvent(incomingTrack->GetEvent(), incomingTrack->GetEventId());
    outgoingTrack->SetElectricalCharge(incomingTrack->GetCharge());
    outgoingTrack->SetMass(incomingTrack->GetMass());

    //std::cerr << "\nCalled multiple scattering from " << incomingTrack << " to " << outgoingTrack << " ";
}

void TransportEngine::Rotate(Double_t th, Double_t ph, Double_t thp, Double_t php, Double_t * cd)
{
    Double_t mr[3][3];
	mr[0][0] = -TMath::Sin(ph);	
	mr[1][0] = TMath::Cos(ph);
	mr[2][0] = 0.;
	mr[0][1] = -TMath::Cos(ph)*TMath::Cos(th);
	mr[1][1] = -TMath::Cos(th)*TMath::Sin(ph);
	mr[2][1] = TMath::Sin(th);
	mr[0][2] = TMath::Sin(th)*TMath::Cos(ph);
	mr[1][2] = TMath::Sin(th)*TMath::Sin(ph);
	mr[2][2] = TMath::Cos(th);

	Double_t cdp[3];
	cdp[0] = TMath::Sin(thp) * TMath::Cos(php);
	cdp[1] = TMath::Sin(thp) * TMath::Sin(php);
	cdp[2] = TMath::Cos(thp);
	for (unsigned int i = 0; i < 3; ++i)
	{
		cd[i] = 0.;
		for (unsigned int j = 0; j < 3; j++)
		{
			cd[i] += mr[i][j]*cdp[j];
		}
	}
} 
