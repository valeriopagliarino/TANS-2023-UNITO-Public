/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

//EXAMPLES OF STARTUP COMMANDS:
/*
//INTERACTIVE:
root & .x start.cxx("sim", "./macro/simulationConfig.txt")

// NON INTERACTIVE:
Start simulation with given configuration file: root -l -b 'start.cxx("sim", "./simulationConfig.txt")'
Start reconstruction with given configuration file: root -l -b 'start.cxx("rec", "./reconstructionConfig.txt")'
*/

#include<iostream>
#include<fstream>
#include<string>
#include<TSystem.h>
#include<TString.h>
#include<TStopwatch.h>
#include<TROOT.h>

#include "./inc/cli.h"

//Forward declaration of the Command Line Interface
class Cli;

//Global variables
TString options;
TString configurationFilePath;

//Entry point
void start(TString myopt="fast", TString configurationFile = "nn")
{
  options = myopt;
  configurationFilePath = configurationFile;
  TStopwatch * stopwatch = new TStopwatch();

  std::cout << "\n   ------------------------------------------------------------ ";
  std::cout << "\n  |     TANS Vertex Sim & Reco - version 1.0 - 31/12/2022      |";
  std::cout << "\n  |             E. Bergalla, V. Pagliarino                     |";
  std::cout << "\n  |        Università di Torino - Dip. di Fisica               |";
  std::cout << "\n  |      Tecniche di Analisi Numerica e Simulazione            |";
  std::cout << "\n   ------------------------------------------------------------ ";

  TString opt;
  if(myopt.Contains("force"))
  {
    opt = "kfgOs";
  }
  else
  {
    opt = "kgOs";
  }

  std::cerr << "\n";
  std::cerr << "\nSource code compilation started using ACLiC...\n";
  gSystem->MakeDirectory("./build/");

  std::cerr << "\nCompiling Monte Carlo simulation classes:";

  //Compile module ProgramConfig (with units.h header)
  std::cerr << "\n\033[1mmake conf.cpp >> conf.so\033[0m ";
  if(gSystem->CompileMacro("./src/conf.cpp",opt.Data(), "ProgramConfig", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module rndEngine
  std::cerr << "\n\033[1mmake rndEngine.cpp >> rndEngine.so\033[0m ";
  if(gSystem->CompileMacro("./src/rndEngine.cpp",opt.Data(), "RndEngine", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module track
  std::cerr << "\n\033[1mmake track.cpp >> track.so\033[0m ";
  if(gSystem->CompileMacro("./src/track.cpp",opt.Data(), "Track", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module transportEngine
  std::cerr << "\n\033[1mmake transportEngine.cpp >> transportEngine.so\033[0m ";
  if(gSystem->CompileMacro("./src/transportEngine.cpp",opt.Data(), "TransportEngine", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module eventManager
  std::cerr << "\n\033[1mmake eventManager.cpp >> eventManager.so\033[0m ";
  if(gSystem->CompileMacro("./src/eventManager.cpp",opt.Data(), "EventManager", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module detectorEffects
  std::cerr << "\n\033[1mmake detectorEffects.cpp >> detectorEffects.so\033[0m ";
  if(gSystem->CompileMacro("./src/detectorEffects.cpp",opt.Data(), "DetectorEffects", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module experimentSimulation
  std::cerr << "\n\033[1mmake experimentSimulation.cpp >> experimentSimulation.so\033[0m ";
  if(gSystem->CompileMacro("./src/experimentSimulation.cpp",opt.Data(), "ExperimentSimulation", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module particleGun
  std::cerr << "\n\033[1mmake particleGun.cpp >> particleGun.so\033[0m ";
  if(gSystem->CompileMacro("./src/particleGun.cpp",opt.Data(), "ParticleGun", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module runManager
  std::cerr << "\n\033[1mmake runManager.cpp >> runManager.so\033[0m ";
  if(gSystem->CompileMacro("./src/runManager.cpp",opt.Data(), "RunManager", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module eventDisplay
  std::cerr << "\n\033[1mmake eventDisplay.cpp >> eventDisplay.so\033[0m ";
  if(gSystem->CompileMacro("./src/eventDisplay.cpp",opt.Data(), "EventDisplay", "build") == 0)
    {std::cerr << " ERR"; return;}

  std::cerr << "\n\nCompiling reconstruction classes:";
  

  //Compile module calculateDeltaPhiMax
  std::cerr << "\n\033[1mmake calculateDeltaPhiMax.cpp >> calculateDeltaPhiMax.so\033[0m ";
  if(gSystem->CompileMacro("./src/calculateDeltaPhiMax.cpp",opt.Data(), "CalculateDeltaPhiMax", "build") == 0)
    std::cerr << " ERR";

  //Compile module hitsAnalysis
  std::cerr << "\n\033[1mmake hitsAnalysis.cpp >> hitsAnalysis.so\033[0m ";
  if(gSystem->CompileMacro("./src/hitsAnalysis.cpp",opt.Data(), "HitsAnalysis", "build") == 0)
    {std::cerr << " ERR"; return;}

  //Compile module resultsAnalysis
  std::cerr << "\n\033[1mmake resultsAnalysis.cpp >> resultsAnalysis.so\033[0m ";
  if(gSystem->CompileMacro("./src/resultsAnalysis.cpp",opt.Data(), "ResultsAnalysis", "build") == 0)
    std::cerr << " ERR";

  //Compile module vertexReco
  std::cerr << "\n\033[1mmake vertexReco.cpp >> vertexReco.so\033[0m ";
  if(gSystem->CompileMacro("./src/vertexReco.cpp",opt.Data(), "VertexReco", "build") == 0)
    {std::cerr << " ERR"; return;}
  
  
  //Compile module Cli
  std::cerr << "\n\033[1mmake cli.cpp >> cli.so\033[0m ";
  if(gSystem->CompileMacro("./src/cli.cpp",opt.Data(), "Cli", "build") == 0)
    {std::cerr << " ERR"; return;}

    

  stopwatch->Stop();
  std::cout << "\n\nDone. ";
  stopwatch->Print();
  std::cout << "\n";
  std::cout << "Libraries loaded, parsing arguments...";
  delete stopwatch;

  gROOT->ProcessLine("Cli::Start(options, configurationFilePath)");

}