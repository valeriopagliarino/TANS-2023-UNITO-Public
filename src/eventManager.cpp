/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "../inc/eventManager.h"

ClassImp(EventManager)

EventManager::EventManager()
{
    eventID = GenerateEventID();
    tracks.reserve(100);
    hits.reserve(100);
}

EventManager::EventManager(const EventManager &eventSource) : TObject()
{
    for (unsigned long int i = 0; i < eventSource.tracks.size(); ++i)
        tracks.push_back(eventSource.tracks[i]);
    
    for (unsigned long int i = 0; i < eventSource.hits.size(); ++i)
        hits.push_back(eventSource.hits[i]);

    runManager = eventSource.runManager;
    eventID = eventSource.eventID;
    persist = eventSource.persist;
    tracks.reserve(100);
    hits.reserve(100);
}

EventManager::~EventManager()
{
    CleanupEvent();
}

void EventManager::SetPersist(bool eventPersist)
{
    persist = eventPersist;
}

Int_t EventManager::GetEventID()
{
    return eventID;
}

RunManager * EventManager::GetRun()
{
    return runManager;
}

long int EventManager::GenerateEventID()
{
    eventIDCounter++;
    return eventIDCounter;
}

bool EventManager::IsPersist()
{
    return persist;
}

void EventManager::CleanupEvent()
{
    // [MEM LEAK CHECK] std::cerr << "\nDelete event invocated!";
    for (unsigned int i = 0; i < tracks.size(); ++i)
        delete tracks[i];

    for (unsigned int i = 0; i < hits.size(); ++i)
        delete hits[i];

    tracks.clear();
    hits.clear();
}