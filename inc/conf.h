#ifndef CONF_H
#define CONF_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include<TObject.h>
#include<TFile.h>
#include<TH1D.h>
#include<TH2D.h>
#include<TF1.h>
#include<TF2.h>

#include "../inc/units.h"

/// @brief This class handles the simulation / reconstruction / analysis configuration
class ProgramConfig : public TObject
{
    public:
        ProgramConfig();
        ~ProgramConfig();

        void ReadConfigurationFile();
        void LoadDebugData();
        bool IsInit();
        void SetFilename(std::string filename);
        void SetRecord(std::string key, std::string value);

        //Run configuration
        std::string simRootFileName = "./simulationOutput.root";
        std::string simInputRootFileName = "./inputData.root";
        //std::string simInputRootFileName = "./kinem.root";
        unsigned long int eventNumber = 200;
        unsigned long int reportEvery = 15;
        int rndSeed = 234;
        bool singleCollisionInEvent = true;
        bool singleEventPersistenceEnabled = false;
        bool hitDebugMode = true;
        TH1D * collisionPerEventDistribution = nullptr; //!

        //Particle gun configuration
        TH1D * momentumDistribution = nullptr; //!
        TH1D * etaDistribution = nullptr; //!
        TH1D * multiplicityDistribution = nullptr; //!
        TH1D * phiDistribution = nullptr; //!
        TH1D * bunchCrossingX = nullptr; //!
        TH1D * bunchCrossingY = nullptr; //!
        TH1D * zPosDistribution = nullptr; //!

        //Detector hit noise
        TH2D * innerSiliconNoise = nullptr; //!
        TH2D * outerSiliconNoise = nullptr; //!
        TH1D * innerCountsDistribution = nullptr; //!
        TH1D * outerCountsDistribution = nullptr; //!

        //Detector hit pixel activation
        TH2D * pixelActivationMap = nullptr; //!
        TH1D * pixelActivationCount = nullptr; //!

        double mass = 511*keV/(c*c);
        double charge = -1 * e;

        void OpenInputTFile(std::string path);
        void WriteInputTFile(std::string path);

        bool enableHitGaussianSmearing;
        bool enableSoftParticlesNoise;
        bool disableKin = false;

        //Geometry
        Double_t beamPipeRadius;
        Double_t beamPipeTickness;
        Double_t innerSiliconRadius;
        Double_t outerSiliconRadius;
        Double_t siliconTickness;
        Double_t beamPipeLenght;
        Double_t innerSiLenght;
        Double_t outerSiLenght;

        //Reconstruction parameters
        bool enableDeltaPhiMaxCalculation = true;
        std::string outRecoRootFileName = "./recoOutput.root";
        Double_t runningWindowSize = 5 *mm;
        Double_t runningWPercentStep = 0.2;
        int minVertNumber = 1;
        Double_t limitPercMaxNumVert = 1.;

        //Analysis parameters 
        std::string outAnalysisRootFileName = "./analysisOutput.root";
        Double_t sigmasNumber = 3.;
        int minMult = 3;
        int maxMult = 52;
        

    private:
        TFile * inputTFile;  //!
        std::string confFilename = "";
        bool init = false;
        bool inputFileInitialized = false;

        TH1D * ReadTH1D(std::string key); //!
        TF1  * ReadTF1(std::string key); //!
        TH2D * ReadTH2D(std::string key); //!
        TF2  * ReadTF2(std::string key); //!

};



#endif