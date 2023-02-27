/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "./src/reconstructionConfigGUI.cpp"

void reconstructionConf()
{
    new reconstructionConfig(gClient->GetRoot(),200,200);
}

void reconstructionConfig::WriteOutputFile()
{
    std::cerr << "\n\nWrite configuration file.\n";
    std::string body;
    body = body + "simRootFileName="                 + std::string(simulationOutput->GetText());
    body = body + "\noutRecoRootFileName="           + std::string(recoOutput->GetText());
    body = body + "\noutAnalysisRootFileName="       + std::string(analysisOutput->GetText());
    body = body + "\nrunningWindowSize="             + std::to_string(fNumberEntry950->GetIntNumber() / double(1000));
    body = body + "\nrunningWPercentStep="           + std::to_string(fNumberEntry952->GetIntNumber() / double(100));
    body = body + "\nminVertNumber="                 + std::to_string(fNumberEntry969->GetIntNumber());
    body = body + "\nlimitPercMaxNumVert="           + std::to_string(fNumberEntry982->GetIntNumber() / double(100));
    body = body + "\nsigmasNumber="                  + std::to_string(fNumberEntry1030->GetIntNumber());
    body = body + "\nminMult="                       + std::to_string(fNumberEntry1051->GetIntNumber());
    body = body + "\nmaxMult="                       + std::to_string(fNumberEntry1064->GetIntNumber());
    body = body + "\nenableDeltaPhiMaxCalculation="  + std::to_string(!enableDeltaPhiMaxCalculationwithMCtruth->IsOn());
    std::cerr << "\x1b[33;40;3m" << body << "\x1b[0m";
    std::ofstream filestream;
    filestream.open(reconstructionConf->GetText());
    filestream << body;
    filestream.close();
}

reconstructionConfig::~reconstructionConfig()
{
    
}

//