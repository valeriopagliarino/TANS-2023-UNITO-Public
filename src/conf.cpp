/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

/*
    Note: Use TH1D->SetDirector(0) to copy the input object to RAM 
*/

#include "../inc/conf.h"


ProgramConfig::ProgramConfig()
{
    init = false;
}

ProgramConfig::~ProgramConfig()
{
    inputTFile->Close();
    init = false;

    delete collisionPerEventDistribution;
    delete momentumDistribution;
    delete etaDistribution;
    delete multiplicityDistribution;
    delete phiDistribution;
    delete bunchCrossingX;
    delete bunchCrossingY;
    delete zPosDistribution;
    delete innerSiliconNoise;
    delete outerSiliconNoise;
    delete innerCountsDistribution;
    delete outerCountsDistribution;
    delete pixelActivationCount;
    delete pixelActivationMap;
    delete inputTFile;
}

void ProgramConfig::SetFilename(std::string filename)
{
    confFilename = filename;
}

void ProgramConfig::ReadConfigurationFile()
{
    if (confFilename == "") return;

    std::ifstream fileStream;
    fileStream.open(confFilename);

    std::string line;
    bool confReadStatus = false;
    std::cerr << "\n";

    while(std::getline(fileStream, line))
    {
        confReadStatus = true;
        std::stringstream linestream(line);
        std::string word;
        std::string key;
        std::string value;
        unsigned short int type = 0; //0-> Key 1->Value
        while(std::getline(linestream, word, '=')) 
        {
            if(type == 0) 
            {
                key = word;
                type = 1;
            }
            else
            {
                value = word;
            }
        }
        SetRecord(key, value);
        std::cerr << "\nLoading param from file: " << key << " = " << value;
    }

    if(!confReadStatus) std::cerr << "\nCritical error while reading configuration file!"; else std::cerr << "\nInitialization completed.";

    
}

bool ProgramConfig::IsInit()
{
    return init;
}

void ProgramConfig::LoadDebugData()
{
    bool debug = true;
    std::cerr << "\nLoading default values...";
    
    //Collisions per event
    if(debug) std::cerr << "\nCollision per event distribution";
    collisionPerEventDistribution = new TH1D("collisionPerEventDistribution", "collisionPerEventDistribution", 10, 0., 10.);
    for (unsigned int i = 0; i < 10; ++i) collisionPerEventDistribution->SetBinContent(i, 0.);
    collisionPerEventDistribution->Fill(1., 1.);

    //Momentum distribution*
    if(debug) std::cerr << "\nmomentumDistribution";
    momentumDistribution = new TH1D("momentumDistribution", "momentumDistribution", 100, 0., 1 * GeV / c);
    auto * fInitMomentumDistr = new TF1("fInitMomentumDistr", "gaus", 0., 10 * GeV / c);
    fInitMomentumDistr->SetParameter("Constant", 1);
    fInitMomentumDistr->SetParameter("Mean", 1200 * MeV / c);
    fInitMomentumDistr->SetParameter("Sigma", 100 * MeV / c);
    momentumDistribution->FillRandom("fInitMomentumDistr");
    delete fInitMomentumDistr;

    //Eta distribution*
    if(debug) std::cerr << "\netaDistribution";
    etaDistribution = new TH1D("etaDistribution", "etaDistribution", 100, -10., 10.);
    auto * fInitEtaDistr = new TF1("fInitEtaDistr", "gaus", 0);
    fInitEtaDistr->SetParameter("Constant", 1);
    fInitEtaDistr->SetParameter("Mean", 0.);
    fInitEtaDistr->SetParameter("Sigma", 4.);
    etaDistribution->FillRandom("fInitEtaDistr");
    delete fInitEtaDistr;

    //Multiplicity distribution*
    if(debug) std::cerr << "\nmultiplicityDistribution";
    multiplicityDistribution = new TH1D("multiplicityDistribution", "multiplicityDistribution", 100., 1, 100);
    auto * fInitmultiplicityDistr = new TF1("fInitmultiplicityDistr", "TMath::Poisson(x, [0])", 0., 100);
    fInitmultiplicityDistr->SetParameter(0, 10.5);
    multiplicityDistribution->FillRandom("fInitmultiplicityDistr");
    delete fInitmultiplicityDistr;

    //Phi distribution*
    if(debug) std::cerr << "\nphiDistribution";
    phiDistribution = new TH1D("phiDistribution", "phiDistribution", 100, 0., 2 * pi);
    auto * fPhiDistribution = new TF1("fPhiDistribution", "pol0", 0., 2 * pi);
    fPhiDistribution->SetParameter(0, 1.);
    phiDistribution->FillRandom("fPhiDistribution");
    delete fPhiDistribution;

    //Bunch crossing X*
    if(debug) std::cerr << "\nBunchCrossingX";
    bunchCrossingX = new TH1D("bunchCrossingX", "bunchCrossingX", 1000., -0.8 * mm, 0.8 * mm);
    auto * fInitxcrossDistr = new TF1("fInitxcrossDistr", "gaus", -0.8 * mm, 0.8 * mm);
    fInitxcrossDistr->SetParameter("Constant", 1.);
    fInitxcrossDistr->SetParameter("Mean", 0. * mm);
    fInitxcrossDistr->SetParameter("Sigma", 0.1 * mm);
    bunchCrossingX->FillRandom("fInitxcrossDistr");
    delete fInitxcrossDistr;

    //Bunch crossing Y*
    if(debug) std::cerr << "\nBunchCrossingY";
    bunchCrossingY = new TH1D("bunchCrossingY", "bunchCrossingY", 1000., -0.8 * mm, 0.8 * mm);
    auto * fInitycrossDistr = new TF1("fInitycrossDistr", "gaus", -0.8 * mm, 0.8 * mm);
    fInitycrossDistr->SetParameter("Constant", 1.);
    fInitycrossDistr->SetParameter("Mean", 0. * mm);
    fInitycrossDistr->SetParameter("Sigma", 0.1 * mm);
    bunchCrossingY->FillRandom("fInitycrossDistr");
    delete fInitycrossDistr;

    //Z pos distribution*
    if(debug) std::cerr << "\nZposDistribution";
    zPosDistribution = new TH1D("zPosDistribution", "zPosDistribution", 300., -30 * cm, 30 * cm);
    auto * fInitzposDistr = new TF1("fInitzposDistr", "gaus", -30 * cm, 30 * cm);
    fInitzposDistr->SetParameter("Constant", 1.);
    fInitzposDistr->SetParameter("Mean", 0. * cm);
    fInitzposDistr->SetParameter("Sigma", 5.3 * cm);
    zPosDistribution->FillRandom("fInitzposDistr");
    delete fInitzposDistr;

    //Soft particles: inner detector noise position*
    if(debug) std::cerr << "\nSoftParticlesNoiseTH2D";
    innerSiliconNoise = new TH2D("innerSiliconNoise", "innerSiliconNoise", 100, 0., 2 * pi, 100, -250 * mm, 250 * mm);
    outerSiliconNoise = new TH2D("outerSiliconNoise", "outerSiliconNoise", 100, 0., 2 * pi, 100, -250 * mm, 250 * mm);
    for (unsigned int i = 0; i < 100; ++i)
    {
        for (unsigned int j = 0; j < 100; ++j)
        {
            innerSiliconNoise->SetBinContent(i, j, 1.);
            outerSiliconNoise->SetBinContent(i, j, 1.);
        }
    }

    //Soft particles: inner detector noise counts*
    if(debug) std::cerr << "\nSoftParticlesInnerCounts";
    innerCountsDistribution = new TH1D("innerCountsDistribution", "innerCountsDistribution", 100., 1, 100);
    auto * fInnerCountsDistribution = new TF1("fInnerCountsDistribution", "TMath::Poisson(x, [0])", 0., 100);
    fInnerCountsDistribution->SetParameter(0, 6.5);
    innerCountsDistribution->FillRandom("fInnerCountsDistribution");
    delete fInnerCountsDistribution;
    
    //Soft particles: outer detector noise*
    if(debug) std::cerr << "\nSoftParticlesOuterCounts";
    outerCountsDistribution = new TH1D("outerCountsDistribution", "outerCountsDistribution", 100., 0., 100.);
    auto * fOuterCountsDistribution = new TF1("fOuterCountsDistribution", "TMath::Poisson(x, [0])", 0., 100);
    fOuterCountsDistribution->SetParameter(0, 6.5);
    outerCountsDistribution->FillRandom("fOuterCountsDistribution");
    delete fOuterCountsDistribution;

    //Pixel activation map and pixel activation distribution*
    if(debug) std::cerr << "\nPixelActivationMap";
    pixelActivationMap = new TH2D("pixelActivationMap", "pixelActivationMap", 100, -5 * mm, +5 * mm, 100, -5 * cm, +5 * mm); 
    TF2 * fPixelActivationMap = new TF2("fPixelActivationMap", "xygaus",-5 * mm, 5 * mm, -5 * mm, 5 * mm);
    //Params of xygaus: const,meanx,sigmax,meany,sigmay
    fPixelActivationMap->SetParameter(0, 1.);
    fPixelActivationMap->SetParameter(1, 0.);
    fPixelActivationMap->SetParameter(2, 120*um);
    fPixelActivationMap->SetParameter(3, 0.);
    fPixelActivationMap->SetParameter(4, 30.*um);
    for (unsigned int i = 0; i < 5000; ++i)
    {   
        Double_t xx, yy;
        fPixelActivationMap->GetRandom2(xx, yy);
        pixelActivationMap->Fill(xx, yy);
    } 

    //PixelActivation*
    if(debug) std::cerr << "\nPixelActivationCounts";
    pixelActivationCount = new TH1D("pixelActivationCount", "pixelActivationCount", 100., 1, 50);
    auto * fPixelActivationCount = new TF1("fPixelActivationCount", "TMath::Poisson(x, [0])", 0., 50);
    fPixelActivationCount->SetParameter(0, 5.5);
    multiplicityDistribution->FillRandom("fPixelActivationCount");
    delete fPixelActivationCount;

    init = true;
    enableHitGaussianSmearing = false;
    enableSoftParticlesNoise = false;

    //Geometry
    beamPipeRadius = 3 * cm;
    beamPipeTickness = 0.8 * mm;
    innerSiliconRadius = 4 * cm;
    outerSiliconRadius = 7 * cm;
    siliconTickness = 0.2 * mm;
    beamPipeLenght = 1 * m;
    innerSiLenght = 27 * cm;
    outerSiLenght = 27 * cm;
    
    enableHitGaussianSmearing = true;
    enableSoftParticlesNoise = true;
    singleEventPersistenceEnabled = false;
}

void ProgramConfig::SetRecord(std::string key, std::string value)
{
    //Parsing strings

    if(key=="simRootFileName")
        simRootFileName = value;

    if(key=="simInputRootFileName")
    {
        simInputRootFileName = value;
        OpenInputTFile(value);
        inputFileInitialized = true;
    }
        

    //Parsing integers

    if(key=="eventNumber")
        eventNumber = atoi(value.c_str());

    if(key=="reportEvery")
        reportEvery = atoi(value.c_str());

    if(key=="rndSeed")
        rndSeed = atoi(value.c_str());

    //Parsing integers => to boolean

    if(key=="singleCollisionInEvent")
        singleCollisionInEvent = (bool)atoi(value.c_str());

    if(key=="singleEventPersistenceEnabled")
        singleEventPersistenceEnabled = (bool)atoi(value.c_str());

    if(key=="enableHitGaussianSmearing")
        enableHitGaussianSmearing = (bool)atoi(value.c_str());

    if(key=="enableSoftParticlesNoise")
        enableSoftParticlesNoise = (bool)atoi(value.c_str());

    //Parsing names of TObjects to be read from the input .root file

    if(key=="collisionPerEventDistribution")
    {
        delete collisionPerEventDistribution;
        collisionPerEventDistribution = ReadTH1D(value);
    }
        

    if(key=="momentumDistribution")
    {
        if(momentumDistribution != nullptr) delete momentumDistribution;
        momentumDistribution = ReadTH1D(value);
    }
  

    if(key=="etaDistribution")
    {
        if(etaDistribution != nullptr) delete etaDistribution;
        etaDistribution = ReadTH1D(value);
    }
        

    if(key=="multiplicityDistribution")
    {
        //if(multiplicityDistribution != nullptr) delete multiplicityDistribution;
        multiplicityDistribution = ReadTH1D(value);
    }
        

    if(key=="phiDistribution")
    {
        if(phiDistribution != nullptr) delete phiDistribution;
        phiDistribution = ReadTH1D(value);
    }
        

    if(key=="bunchCrossingX")
    {
        if(bunchCrossingX != nullptr) delete bunchCrossingX;
        bunchCrossingX = ReadTH1D(value);
    }
        

    if(key=="bunchCrossingY")
    {
        if(bunchCrossingY != nullptr) delete bunchCrossingY;
        bunchCrossingY = ReadTH1D(value);
    }
        

    if(key=="zPosDistribution")
    {
        if(zPosDistribution != nullptr) delete zPosDistribution;
        zPosDistribution = ReadTH1D(value);
    }
        

    if(key=="innerSiliconNoise")
    {
        if(innerSiliconNoise != nullptr) delete innerSiliconNoise;
        innerSiliconNoise = ReadTH2D(value);
    }
        

    if(key=="outerSiliconNoise")
    {
        if(outerSiliconNoise != nullptr) delete outerSiliconNoise;
        outerSiliconNoise = ReadTH2D(value);
    }
        

    if(key=="innerCountsDistribution")
    {
        if(innerCountsDistribution != nullptr) delete innerCountsDistribution;
        innerCountsDistribution = ReadTH1D(value);
    }
        

    if(key=="outerCountsDistribution")
    {
        if(outerCountsDistribution != nullptr) delete outerCountsDistribution;
        outerCountsDistribution = ReadTH1D(value);
    }
        

    if(key=="pixelActivationMap")
    {
        if(pixelActivationMap != nullptr) delete pixelActivationMap;
        pixelActivationMap = ReadTH2D(value);
    }
        

    if(key=="pixelActivationCount")
    {
        if(pixelActivationCount != nullptr) delete pixelActivationCount;
        pixelActivationCount = ReadTH1D(value);
    }
        

    //Parsing geometry parameters

    if(key=="beamPipeRadius")
        beamPipeRadius = atof(value.c_str());

    if(key=="beamPipeTickness")
        beamPipeTickness = atof(value.c_str());

    if(key=="innerSiliconRadius")
        innerSiliconRadius = atof(value.c_str());

    if(key=="outerSiliconRadius")
        outerSiliconRadius = atof(value.c_str());

    if(key=="siliconTickness")
        siliconTickness = atof(value.c_str());

    if(key=="beamPipeLenght")
        beamPipeLenght = atof(value.c_str());

    if(key=="innerSiLenght")
        innerSiLenght = atof(value.c_str());

    if(key=="outerSiLenght")
        outerSiLenght = atof(value.c_str());

    //Parsing reconstruction parameters

    if(key=="enableDeltaPhiMaxCalculation")
        enableDeltaPhiMaxCalculation= (bool)atoi(value.c_str());

    if(key=="outRecoRootFileName")
        outRecoRootFileName = value;

    if(key=="runningWindowSize")
        runningWindowSize = atof(value.c_str());

    if(key=="runningWPercentStep")
        runningWPercentStep = atof(value.c_str());

    if(key=="minVertNumber")
        minVertNumber = atof(value.c_str());    

    if(key=="limitPercMaxNumVert")
        limitPercMaxNumVert = atof(value.c_str());      
      
    //Parsing reconstruction parameters    

    if(key=="outAnalysisRootFileName")
        outAnalysisRootFileName = value;

    if(key=="sigmasNumber")
        sigmasNumber= atof(value.c_str());  

    if(key=="minMult")
        minMult = atoi(value.c_str());

    if(key=="maxMult")
        maxMult = atoi(value.c_str());
    
}

//Possible upgrade: replace multiple functions with ::ReadObject() with templates!
TH1D * ProgramConfig::ReadTH1D(std::string key)
{
    if(inputFileInitialized == false){
        std::cerr<<"\nerror: File not initialized";
        init = false;
        return nullptr;
    }
    TH1D * h1;
    h1 = (TH1D*)inputTFile->Get(key.c_str());
    std::cerr << "\nObject loaded from file";
    return h1;
}

TF1 * ProgramConfig::ReadTF1(std::string key)
{
    if(inputFileInitialized == false){
        std::cerr<<"\nerror: File not initialized";
        init = false;
        return nullptr;
    }
    TF1 * f1;
    f1 = (TF1*)inputTFile->Get(key.c_str());
    std::cerr << "\nObject loaded from file";
    return f1;
}

TH2D * ProgramConfig::ReadTH2D(std::string key)
{
    if(inputFileInitialized == false){
        std::cerr<<"\nerror: File not initialized";
        init = false;
        return nullptr;
    }
    TH2D * h2;
    h2 = (TH2D*)inputTFile->Get(key.c_str());
    std::cerr << "\nObject loaded from file";
    return h2;
}

TF2 * ProgramConfig::ReadTF2(std::string key)
{
    if(inputFileInitialized == false){
        std::cerr<<"\nerror: File not initialized";
        init = false;
        return nullptr;
    }
    TF2 * f2;
    f2 = (TF2*)inputTFile->Get(key.c_str());
    std::cerr << "\nObject loaded from file";
    return f2;
}

void ProgramConfig::OpenInputTFile(std::string path)
{
    inputTFile = new TFile(path.c_str(), "READ");
}

void ProgramConfig::WriteInputTFile(std::string path)
{
    TFile * writeInputTFile = new TFile(path.c_str(), "RECREATE");
    
    collisionPerEventDistribution->Write("collisionPerEventDistribution");
    momentumDistribution->Write("momentumDistribution");
    etaDistribution->Write("etaDistribution");
    multiplicityDistribution->Write("multiplicityDistribution");
    phiDistribution->Write("phiDistribution");
    bunchCrossingX->Write("bunchCrossingX");
    bunchCrossingY->Write("bunchCrossingY");
    zPosDistribution->Write("zPosDistribution");
    innerSiliconNoise->Write("innerSiliconNoise");
    outerSiliconNoise->Write("outerSiliconNoise");
    innerCountsDistribution->Write("innerCountsDistribution");
    outerCountsDistribution->Write("outerCountsDistribution");
    pixelActivationMap->Write("pixelActivationMap");
    pixelActivationCount->Write("pixelActivationCount");

    writeInputTFile->Close();
    delete writeInputTFile;
}
