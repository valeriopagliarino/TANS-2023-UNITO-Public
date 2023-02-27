/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/experimentSimulation.h"

#if !defined(__CLING__)
//ClassImp(ExperimentSimulation);
#endif

ExperimentSimulation::ExperimentSimulation()
{
    //gSystem->Load("libGeom");
    //R__LOAD_LIBRARY("libGeom");
    //worldVolume = new TGeoManager();
    //worldVolume->SetDefaultUnits(kRootUnits); //cm, kg, GeV
}

ExperimentSimulation::~ExperimentSimulation()
{
    delete outerSiPlane;
    delete innerSiPlane;
    delete beamPipe;
    delete vacuum;
    delete berillium;
    delete silicon;
    delete worldVolume;
}

void ExperimentSimulation::BuildGeometry()
{
    vacuum      = new TGeoMaterial("Vacuum",0,0,0);
    berillium   = new TGeoMaterial("Berillium",0,0,0);
    silicon     = new TGeoMaterial("Silicon",0,0,0);

    beamPipe     = new TGeoTube(conf->beamPipeRadius - conf->beamPipeTickness / 2., conf->beamPipeRadius + conf->beamPipeTickness / 2., conf->beamPipeLenght / 2.);
    innerSiPlane = new TGeoTube(conf->innerSiliconRadius - conf->siliconTickness / 2., conf->innerSiliconRadius + conf->siliconTickness / 2., conf->innerSiLenght / 2.);
    outerSiPlane = new TGeoTube(conf->outerSiliconRadius - conf->siliconTickness / 2., conf->outerSiliconRadius + conf->siliconTickness / 2., conf->outerSiLenght / 2.);

    
    geometryRegister.push_back(beamPipe);       //index = 0;
    geometryRegister.push_back(innerSiPlane);   //index = 1;
    geometryRegister.push_back(outerSiPlane);   //index = 2;

}

void ExperimentSimulation::ProcessEvent(EventManager * currentEvent)
{
    if (geometryRegister.size() == 0)
    {
        if (msg == false) std::cout << "\nCritical error: empty geometry register. Aborting simulation.";
        msg = true;
        return;
    } 

    bool iterate = true;

    unsigned long int iterator = 0;
    while(iterator < currentEvent->tracks.size())
    {
        if (currentEvent->tracks[iterator]->isActive())
        {
            ProcessTrack(currentEvent->tracks[iterator]);
        }
        iterator++;
    }
}

int ExperimentSimulation::GetGeomIntersection(Track * currentTrack, Hit * hit)
{
    int interactionGeomIndex = -1;
    Double_t tMin = 1; //1s
    bool empty = true;
    Double_t rMin = 0;
    
    Double_t x0 = currentTrack->GetTrackStartX();
    Double_t y0 = currentTrack->GetTrackStartY();
    Double_t z0 = currentTrack->GetTrackStartZ();
    Double_t v1, v2, v3;

    if (conf->disableKin == false)
    {
        currentTrack->GetMomentum(v1, v2, v3);
    }
    v1 = currentTrack->GetVelocityX();
    v2 = currentTrack->GetVelocityY();
    v3 = currentTrack->GetVelocityZ();

    for (unsigned int j = 0; j < (uint)geometryRegister.size(); ++j)
    {
        Double_t t = 0;
        Double_t R = (geometryRegister[j]->GetRmax() + geometryRegister[j]->GetRmin()) / 2; //Mean radius
        Double_t H = geometryRegister[j]->GetDz(); //Half lenght

       Double_t Delta = (x0*v1 + y0*v2) * (x0*v1 + y0*v2);
       Delta -= (v1*v1 + v2*v2) * (x0*x0 + y0*y0 - R*R);
       
       t  = (-1. * (x0*v1 + y0*v2) + TMath::Sqrt(Delta)) / (v1*v1 + v2*v2);
       Double_t zz = z0 + v3*t;
       
       if((zz < H) && (zz > (-1. * H)) && (t > 1e-15)) //check Z of the intersection point
       { 
            if((t < tMin)) 
            {
                interactionGeomIndex = j;
                tMin = t;
                empty = false;
                rMin = R;
            }
       }
       else
       {

       }
    }

    if(!empty)
    {
        hit->SetX(x0 + tMin * v1);
        hit->SetY(y0 + tMin * v2);
        hit->SetZ(z0 + tMin * v3);
        hit->SetT(tMin);
        //std::cerr << "\nTmin =" <<  tMin;
    }

    return interactionGeomIndex;
}

void ExperimentSimulation::ProcessTrack(Track * currentTrack)
{
    bool reuseMem = false;
    Hit * hit = new Hit();
    EventManager * currentEvent = currentTrack->GetEvent();

    //Get the first intersection between the track and geometry (time ordered)
    int interactionGeomIndex = GetGeomIntersection(currentTrack, hit);

    //Beam pipe
    if(interactionGeomIndex  == 0)
    {
        
        //Add the hit to the event hit storage and to the run TTree
        ProcessHit(currentTrack, hit, interactionGeomIndex);

        //Check if the multiple scattering physics process simulation is enabled
        if (physicsList.multipleScattering == true)
        {
            Track * tr1 = new Track();
            Double_t zMat = 0., x = 0., xr = 0.;
            TransportEngine::MultipleScattering(currentTrack, tr1, hit, zMat, x, xr, physicsList.multipleScatteringThetaMsApprox, !conf->disableKin);
            //std::cerr << "\nCalled multiple scattering tracks: " << currentTrack << "  ParticleID=" << currentTrack->GetParticleID() << "  -> " << tr1 << "  ParticleID=" << tr1->GetParticleID();
            currentEvent->tracks.push_back(tr1);
        }
        
        return;
    }

    //Inner silicon plane
    if(interactionGeomIndex == 1)
    {


        //Add the hit to the event hit storage and to the run TTree
        ProcessHit(currentTrack, hit, interactionGeomIndex);

        //Check if the multiple scattering physics process simulation is enabled
        if (physicsList.multipleScattering == true)
        {
            Track * tr2 = new Track();
            Double_t zMat = 0., x = 0., xr = 0.;
            TransportEngine::MultipleScattering(currentTrack, tr2, hit, zMat, x, xr, physicsList.multipleScatteringThetaMsApprox);
            //std::cerr << "\nCalled multiple scattering tracks: " << currentTrack << "  ParticleID=" << currentTrack->GetParticleID() << "  -> " << tr2 << "  ParticleID=" << tr2->GetParticleID();
            currentEvent->tracks.push_back(tr2);
            
        }

        return;
    }

    //Outer silicon plane
    if(interactionGeomIndex == 2)
    {
        //Add the hit to the run TTree
        ProcessHit(currentTrack, hit, interactionGeomIndex);
        currentTrack->SetTrackStop(hit->X(), hit->Y(), hit->Z());
        currentTrack->SetActiveTrack(false);
        return;
    }
    
    //This particle does not interact with any part of the detector, stop tracking
    currentTrack->SetNoStop(true);
    if (!reuseMem) currentTrack->SetActiveTrack(false);
    
}


void ExperimentSimulation::ProcessHit(Track * &currentTrack, Hit * hit, int detectorId)
{
    RunManager * currentRun = currentTrack->GetEvent()->GetRun();
    Double_t xHit, yHit, zHit;
    xHit = hit->X();
    yHit = hit->Y();
    zHit = hit->Z();

    unsigned long int particleID = currentTrack->GetParticleID();

    Double_t norm = TMath::Sqrt(xHit * xHit + yHit * yHit + zHit * zHit);
    Double_t normPlane = TMath::Sqrt(xHit * xHit + yHit * yHit);
    Double_t thetaHit = TMath::ACos(zHit / norm);
    Double_t phiHit   = TMath::ACos((xHit / norm) / TMath::Sin(thetaHit));
    if(yHit < 0) phiHit = 2*TMath::Pi() - phiHit;

    // Int_t noPixels = conf->pixelActivationCount->GetRandom(rndEngine);
    // for (unsigned int i = 0; i < noPixels; ++i)
    // {

    Double_t deltaZHit = 0.;
    Double_t deltaAr = 0.;
    if (conf->enableHitGaussianSmearing) conf->pixelActivationMap->GetRandom2(deltaZHit, deltaAr, rndEngine);

    Double_t recX = normPlane * TMath::Cos(phiHit + deltaAr/normPlane);
    Double_t recY = normPlane * TMath::Sin(phiHit + deltaAr/normPlane);
    Double_t recZ = zHit + deltaZHit;

    //std::cerr << "\nRecorded hit from track " << currentTrack << "  ParticleID=" << currentTrack->GetParticleID() << "  detectorID = " << detectorId;

    //Do not record in the TTree hits with the beam pipe
    if (detectorId != 0)
    {
        //Fill the TTree!
        tree->GetBranch("DetectorHits")->SetAddress(&dhit.X);
        dhit.X = recX;
        dhit.Y = recY;
        dhit.Z = recZ;
        dhit.eventID = currentTrack->GetEvent()->GetEventID();
        dhit.particleID = currentTrack->GetParticleID();
        dhit.detectorID = detectorId;
        tree->GetBranch("DetectorHits")->Fill();
        //std::cerr << "\nSILICON TRACKER HIT - detectorId=" << detectorId << "  (" << recX << "  " << recY << "  " << recZ << ")  ParticleID=" << particleID << "  eventID = " << dhit.eventID;
    }

    // }
    

    if(currentTrack->GetEvent()->IsPersist())
    {
        currentTrack->GetEvent()->hits.push_back(hit);
    }
    else
    {
        delete hit;
    }
}

void ExperimentSimulation::SetRndEngine(RndEngine * RndE)
{
    rndEngine = RndE;
}

void ExperimentSimulation::SetConfiguration(ProgramConfig * config)
{
    conf = config;
}

std::vector<TGeoTube *> ExperimentSimulation::GetGeometry()
{
    return geometryRegister;
}