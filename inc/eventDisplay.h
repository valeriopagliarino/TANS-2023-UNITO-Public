#ifndef EVENTDISPLAY_H
#define EVENTDISPLAY_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<vector>

#include<TNamed.h>
#include<TGeoTube.h>
#include<TEveTrack.h>
#include<TEveLine.h>
#include<TEveWindow.h>
#include "TSystem.h"
#include "TEveManager.h"
#include "TEvePointSet.h"
#include "TEveText.h"
#include "TEveArrow.h"
#include "TEveVector.h"
#include "TEveTrans.h"
#include "TEveShape.h"
#include "TEveGeoShape.h"
#include "TGeoTube.h"

#include "../inc/eventManager.h"
#include "../inc/runManager.h"
#include "../inc/experimentSimulation.h"

/// @brief This class enables the visualization of tracks and geometry of each event with an event display
class EventDisplay : public TNamed
{
    public:
        EventDisplay();
        EventDisplay(EventManager * LoadEvent, ExperimentSimulation * experimentGeometry);
        ~EventDisplay();

        /// @brief This function loads a specific event into the event display. When called, the tracks are copied from the event and the TEveShape objects are generated
        /// @param loadEvent Event to be loaded
        void LoadEvent(EventManager * loadEvent);

        /// @brief This function loads the geometry defined in the Geometry Register from a specific istance of the ExperimentSimulation class. Sensitive detectors and budget materials are imported and TEveShape objects are generated.
        /// @param experimentGeometry Geometry to be loaded
        void LoadExperimentGeometry(ExperimentSimulation * experimentGeometry);
        
        /// @brief Calling this function invokes the creation of the window and the OpenGL rendering engine
        void DrawEvent();
        void Cleanup();

    private:
        TEveManager * manager;
        EventManager * currentEvent;
        std::vector<TGeoTube *> geometryList;
        std::vector<TEveGeoShape *> tEveShapes;
        std::vector<TEvePointSet *> tHits;
        std::vector<TEveArrow *> tEveTracks;
        TEvePointSet * vertex;
        TEvePointSet * marker;
        bool init = false;
        Double_t lineWidth = 0.0009;
        Double_t hitL = 0.0005;
        Double_t hitR = 0.005;
        
    
};

#endif