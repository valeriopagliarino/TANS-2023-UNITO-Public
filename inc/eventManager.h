#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TObject.h>
#include<TVector3.h>

#include "../inc/track.h"
#include "../inc/hit.h"
#include "../inc/runManager.h"

/// @brief The eventManager is the class representing a single event, it is a container that stores all the tracks
class EventManager : public TObject
{
    public:
        EventManager();
        EventManager(const EventManager &eventSource);
        ~EventManager(); //UNALLOCATE ALL THE CONTENT IN hits AND tracks

        /// @brief This function specify if the tracks and the hit of this event should be saved to memory or should be deallocated after recording the sensitive hits in the TTree. Recording the full MC truth can be useful for visualization with the Event Display after the simulation and comparison with reconstructed data.
        /// @param eventPersist Choose wheter the event is persistent
        void SetPersist(bool eventPersist);

        Int_t GetEventID();

        /// @brief Get a pointer to the RunManager object that owns this specific event 
        RunManager * GetRun(); //!

        bool IsPersist();

        /// @brief Invokes the deallocation from memory of all the Tracks and Hits objects (only data inside the TTree survives) and then the std::vectors holding the pointers are erased
        void CleanupEvent();

        std::vector<Track *> tracks; //!
        std::vector<Track *> inactiveTracks; //!
        std::vector<Hit *> hits; //!
        RunManager * runManager; //!

        void SetVertex(Double_t vx, Double_t vy, Double_t vz) {vertX = vx; vertY = vy; vertZ = vz;}
        Double_t GetVertX() {return vertX;}
        Double_t GetVertY() {return vertY;}
        Double_t GetVertZ() {return vertZ;}

    private:
        Int_t eventID;
        bool persist;
        static long int eventIDCounter;
        Double_t vertX, vertY, vertZ;

        static long int GenerateEventID();

    ClassDef(EventManager, 1)
};

//Definition of static data members
long int EventManager::eventIDCounter;

//Directives for dictionary generation by ROOTCLING for ACliC
#ifdef __ROOTCLING__
#pragma link C++ class EventManager;
#endif

#endif