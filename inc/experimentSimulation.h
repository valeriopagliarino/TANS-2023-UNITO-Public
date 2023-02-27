#ifndef EXPERIMENTSIMULATION_H
#define EXPERIMENTSIMULATION_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TNamed.h>
#include<TGeoManager.h>
#include<TGeoMaterial.h>
#include<TGeoVolume.h>
#include<TGeoShape.h>
#include<TGeoTube.h>
#include<TGeoShapeAssembly.h>
#include<TTree.h>
#include<vector>

#include "../inc/transportEngine.h"
#include "../inc/rndEngine.h"
#include "../inc/eventManager.h"
#include "../inc/runManager.h"
#include "../inc/track.h"
#include "../inc/hit.h"
#include "../inc/conf.h"

/// @brief Setting the data members of this struct the user can enable or disable specific functions modeling radiation-matter interaction effects
typedef struct PhysicsListTypedef {
    bool multipleScattering = true;
    bool multipleScatteringThetaMsApprox = true;
    bool betheblochIonization = false; //Possible future upgrade, compatible with the software architecture, but not yet implemented!
    } PhysicsList;

/// @brief This class describes this specific simulation, its geometry and it acts on the information stored inside each eventManager. It uses the physics tools (as static methods) from the TransportEngine class.
class ExperimentSimulation : public TNamed
{
    public:
        ExperimentSimulation();
        ~ExperimentSimulation();

        void BuildGeometry();
        void SetRndEngine(RndEngine * rndE);
        void SetConfiguration(ProgramConfig * config);
        void SetHitTree(RunManager * outTree) {tree = outTree;}
        std::vector<TGeoTube *> GetGeometry();

        void ProcessEvent(EventManager * currentEvent);

        PhysicsList physicsList;

    private:
        RunManager * tree;
        TGeoManager  * worldVolume;

        TGeoMaterial * vacuum;
        TGeoMaterial * berillium;
        TGeoMaterial * silicon;

        TGeoTube     * beamPipe;
        TGeoTube     * innerSiPlane;
        TGeoTube     * outerSiPlane;

        std::vector<TGeoTube *> geometryRegister;

        ProgramConfig * conf;
        static RndEngine * rndEngine;
        bool msg = false;

        void ProcessTrack(Track * currentTrack);
        void ProcessHit(Track * &currentTrack, Hit * hit, int detectorId);
        int  GetGeomIntersection(Track * currentTrack, Hit * hit);

};

//Definition of static data members
RndEngine * ExperimentSimulation::rndEngine;

#endif
