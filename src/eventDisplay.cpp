/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/eventDisplay.h"

EventDisplay::EventDisplay()
{
    
}

EventDisplay::EventDisplay(EventManager * loadEvent, ExperimentSimulation * experimentGeometry)
{
    LoadEvent(loadEvent);
    LoadExperimentGeometry(experimentGeometry);
}

EventDisplay::~EventDisplay()
{
    //Do not allocate geometry, since it is shared with the experiment simulation!
    //Deallocate only the TEve copies of geometry
    Cleanup();
}

void EventDisplay::LoadEvent(EventManager * loadEvent)
{
    currentEvent = loadEvent;
}

void EventDisplay::LoadExperimentGeometry(ExperimentSimulation * experimentGeometry)
{
    geometryList = experimentGeometry->GetGeometry();
}

void EventDisplay::Cleanup()
{
    init = false;
    delete marker;

    for (unsigned int i = 0; i < tEveShapes.size(); ++i)
    {
        delete tEveShapes[i];
    }
    tEveShapes.clear();

    for (unsigned int i = 0; i < tEveTracks.size(); ++i)
    {
        delete tEveTracks[i];
    }
    tEveTracks.clear();

    for (unsigned int i = 0; i < tHits.size(); ++i)
    {
        delete tHits[i];
    }
    tHits.clear();

    delete vertex;
    delete manager;
}

void EventDisplay::DrawEvent()
{
    if(!currentEvent->IsPersist())
    {
        std::cerr << "\nError: this event has no persistent tracks. Nothing to draw.\n";
    }

    if (init) std::cerr << "\nCritical warning: adding new tracks to a not-empty event display istance!";
    init = true;
    gSystem->IgnoreSignal(kSigSegmentationViolation, true);

    //Create a new Event Display Manager istance
    manager = TEveManager::Create();
    
    //Create the position markers
    marker = new TEvePointSet(8);
    marker->SetName("Origin marker");
    marker->SetMarkerColor(6);
    marker->SetMarkerStyle(3);
    Float_t a = 1;
    marker->SetPoint(0, a,  +a, +a);
    marker->SetPoint(1, a,  -a, +a);
    marker->SetPoint(2, -a, -a, +a);
    marker->SetPoint(3, -a, +a, +a);
    marker->SetPoint(4, +a, +a, -a);
    marker->SetPoint(5, +a, -a, -a);
    marker->SetPoint(6, -a, +a, -a);
    marker->SetPoint(7, -a, -a, -a);
    gEve->AddElement(marker);

    //Create the TGeoTube geometry (silicon planes and beam pipe)
    for (unsigned int i = 0; i  < geometryList.size(); ++i)
    {
        TEveGeoShape * shape = new TEveGeoShape();
        shape->SetShape(geometryList[i]);
        shape->SetFillColor(kRed);
        if (i == 0) shape->SetFillColor(kBlue);
        //shape->SetNameTitle("Name", "Title");
        shape->SetPickable(kTRUE);
        gEve->AddElement(shape);
        tEveShapes.push_back(shape);
    }

    //Create the tracks
    for (unsigned int i = 0; i < currentEvent->tracks.size(); ++i)
    {
        if(!currentEvent->tracks[i]->GetNoStop())
        {
            Double_t deltaX = currentEvent->tracks[i]->GetTrackStopX();
            Double_t deltaY = currentEvent->tracks[i]->GetTrackStopY();
            Double_t deltaZ = currentEvent->tracks[i]->GetTrackStopZ();

            Double_t origX = currentEvent->tracks[i]->GetTrackStartX();
            Double_t origY = currentEvent->tracks[i]->GetTrackStartY();
            Double_t origZ = currentEvent->tracks[i]->GetTrackStartZ();

            deltaX = deltaX - origX;
            deltaY = deltaY - origY;
            deltaZ = deltaZ - origZ;

            TEveArrow * a1 = new TEveArrow(deltaX, deltaY, deltaZ, origX, origY, origZ);
            std::string trackname = "Part.ID=" + std::to_string(currentEvent->tracks[i]->GetParticleID()) + " P=" + std::to_string(currentEvent->tracks[i]->GetMomentum() * c / GeV) + " GeV/c";
            a1->SetNameTitle(trackname.c_str(), trackname.c_str());
            a1->SetMainColor(kGreen);
            a1->SetTubeR(lineWidth);
            a1->SetConeR(hitR);
            a1->SetConeL(hitL);
            a1->SetPickable(kTRUE);
            gEve->AddElement(a1);
            tEveTracks.push_back(a1);
        }
    }

    //Create the hits
    for (unsigned int i = 0; i < currentEvent->hits.size(); ++i)
    {
        TEvePointSet * hitPoint = new TEvePointSet(1);
        hitPoint->SetMarkerColor(6);
        hitPoint->SetMarkerStyle(20);
        hitPoint->SetMarkerSize(3.5);
        hitPoint->SetPoint(0, currentEvent->hits[i]->X(),  currentEvent->hits[i]->Y(), currentEvent->hits[i]->Z());
        tHits.push_back(hitPoint);
        gEve->AddElement(hitPoint);
    }

    //Create the Vertex
    TEvePointSet * vertex = new TEvePointSet(1);
    vertex->SetMarkerColor(6);
    vertex->SetMarkerStyle(20);
    vertex->SetMarkerSize(3.5);
    vertex->SetPoint(0, currentEvent->GetVertX(), currentEvent->GetVertY(), currentEvent->GetVertZ());
    gEve->AddElement(vertex);
    gEve->FullRedraw3D(kTRUE);
}