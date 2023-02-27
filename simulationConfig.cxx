/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include "./src/simulationConfigGUI.cpp"

//_________________________________________________________________________________
//MACRO ENTRY POINT
void simulationConfig()
{
    new simulationConfigForm(gClient->GetRoot(),200,200);
}

//_________________________________________________________________________________
void simulationConfigForm::openTBrowser()
{
    std::cerr << "\n\nBrowse the input file.";
    TFile * rootfile = new TFile(fTextEntry661->GetText(), "READ");
    if(!rootfile->IsOpen())
    {
        std::cerr << "\nROOT input file not found!";
        return;
    }
    rootfile->Close();
    auto *tb = new TBrowser(fTextEntry661->GetText());
}

//_________________________________________________________________________________
void simulationConfigForm::plotPreview()
{
    int selPDF = selectPdfListBox->GetSelected();
    if (selPDF == 8)
    {
        std::cerr << "\nPlease use the TBrowser to inspect distributions already saved in the input TFile.\n";
        return;
    }

    if (temp1dAssigned) delete temp1d;
    if (temp2dAssigned) delete temp2d;

    temp1dAssigned = false;
    temp2dAssigned = false;

    double p0 = par0->GetNumber();
    double p1 = par1->GetNumber();
    double p2 = par2->GetNumber();
    double p3 = par3->GetNumber();

    TF1 * f1;
    TF2 * f2;

    double upperlimit = 1.;

    switch(selPDF)
    {
        case 1: //"TH1D - Uniform distribution between p0 and p1",1
            if (!(p1 > p0))
            {
                std::cerr << "\nCondition p1 > p0 not satisfied! Change the param. values.";
                return;
            }
            temp1dAssigned = true;
            temp1d = new TH1D("temp1d", "Uniform distribution between p0 and p1", 500, p0, p1);
            for (int i = 0; i < 500; ++i) temp1d->SetBinContent(i, 0.01);
            break;

        case 2: //"TH1D - Gaussian distribution with mu=p0 sigma=p1",2
            if (p1 < 1e-100)
            {
                std::cerr << "\nSigma cannot be zero! Change the param. values.";
                return;
            }
            temp1dAssigned = true;
            temp1d = new TH1D("temp1d", "Gaussian distribution with mu=p0", 500, p0 - 5*p1, p0 + 5*p1);
            f1 = new TF1("f1", "gaus", p0 - 5*p1, p0 + 5*p1);
            f1->SetParameter(0, 1.);
            f1->SetParameter(1, p0);
            f1->SetParameter(2, p1);
            for (int i = 0; i  < 10000; ++i) temp1d->Fill(f1->GetRandom());
            delete f1;
            break;

        case 3: //"TH1D - Poisson distribution with mean=p0",3
            upperlimit = p0 + 5*sqrt(p0);
            if (p0 < 0.01)
            {
                upperlimit = 0.6;
            }
            temp1dAssigned = true;
            temp1d = new TH1D("temp1d", "Poisson distribution with mean=p0", 500, p0 - 5*sqrt(p0), upperlimit);
            f1 = new TF1("f1", "TMath::Poisson(x, [0])", p0 - 5*sqrt(p0), upperlimit);
            f1->SetParameter(0, p0);
            for (int i = 0; i  < 10000; ++i) temp1d->Fill(f1->GetRandom());
            delete f1;
            break;

        case 4: //"TH1D - Fixed value = p0"
            temp1dAssigned = true;
            temp1d = new TH1D("temp1d", "TH1D - Fixed value = p0", 1000, p0 - 1e-100, p0 + 1e-100);
            temp1d->Fill(p0, 1.);
            break;

        case 5: //"TH2D - Uniform between X:p0,p1  Y:p2,p3",5
            if (!((p1 > p0) && (p3 > p2)))
            {
                std::cerr << "\nCondition p1 > p0 and p3 > p2 not satisfied! Change the param. values.";
                return;
            }
            temp2dAssigned = true;
            temp2d = new TH2D("temp2d", "TH2D - Uniform between X:p0,p1  Y:p2,p3", 50, p0, p1, 50, p2, p3);
            for (int i = 0; i < 50; ++i)
            {
                for (int j = 0; j< 50; ++j) temp2d->SetBinContent(i, j, 1.);
            }
            break;

        case 6: //"TH2D - Gaussian with muX=p0, sigmaX=p1, muY=p2, sigmaY=p3",6
            if (p1 < 1e-100)
            {
                std::cerr << "\nSigma cannot be zero! Change the param. values.";
                return;
            }
            if (p3 < 1e-100)
            {
                std::cerr << "\nSigma cannot be zero! Change the param. values.";
                return;
            }
            temp2dAssigned = true;
            temp2d = new TH2D("temp2d", "TH2D - Gaussian with muX=p0, sigmaX=p1, muY=p2, sigmaY=p3", 500, p0 - 5*p1, p0 + 5*p1, 500, p2 - 5*p3, p2 + 5*p3);
            f2 = new TF2("f1", "xygaus", p0 - 5*p1, p0 + 5*p1, p2 - 5*p3, p2 + 5*p3);
            f2->SetParameter(0, 1.);
            f2->SetParameter(1, p0); //muX
            f2->SetParameter(2, p1); //sigmaX 
            f2->SetParameter(3, p2); //muY
            f2->SetParameter(4, p3); //sigmaY
            for (unsigned int i = 0; i < 5000; ++i)
            {   
                Double_t xx, yy;
                f2->GetRandom2(xx, yy);
                temp2d->Fill(xx, yy);
            } 
            delete f2;
            break;

        case 7: //"TH2D - Fixed value X=p0  Y=p1",7
            temp2dAssigned = true;
            temp2d = new TH2D("temp2d", "TH2D - Fixed value X=p0  Y=p1", 1000, p0 - 1e-100, p0 + 1e-100, 1000, p1 - 1e-100, p1 + 1e-100);
            temp2d->Fill(p0, p1, 1.);
            break;
    }


    TCanvas * c1 = previewCanvas->GetCanvas();
    c1->cd();
    if (temp1dAssigned) temp1d->Draw();
    if (temp2dAssigned) temp2d->Draw("COL");
    c1->Modified(); 
    c1->Update();
    std::cerr << "\n\nPlot probability density function updated and plotted in the embedded canvas.";
}

//_________________________________________________________________________________
void simulationConfigForm::assign()
{
    int selPDF = selectPdfListBox->GetSelected();
    

    int listIndex = assignListBox->GetSelected();
    if (listIndex < 1) return;
    if (listIndex > 14) return;
    plotPreview();
    std::string configurationFileEntry = "";
    std::string def = "";
    

    if (temp1dAssigned)
    {
        temp1d->SetName(chooseDistName->GetText());
        temp1d->SetTitle(chooseDistName->GetText());
    }

    if (temp2dAssigned)
    {
        temp2d->SetName(chooseDistName->GetText());
        temp2d->SetTitle(chooseDistName->GetText());
    }

    switch(listIndex)
    {
        case 1:
            configurationFileEntry = "collisionPerEventDistribution"; 
            collisionPerEventDistribution=temp1d;
            def = "collisionPerEventDistribution";
            break;
        case 2:
            configurationFileEntry = "momentumDistribution"; 
            momentumDistribution=temp1d;
            def = "momentumDistribution";
            break;
        case 3:
            configurationFileEntry = "etaDistribution"; 
            etaDistribution=temp1d;
            def = "etaDistribution";
            break;
        case 4:
            configurationFileEntry = "multiplicityDistribution"; 
            multiplicityDistribution=temp1d;
            def = "multiplicityDistribution";
            break;
        case 5:
            configurationFileEntry = "phiDistribution"; 
            phiDistribution=temp1d;
            def = "phiDistribution";
            break;
        case 6:
            configurationFileEntry = "bunchCrossingX"; 
            bunchCrossingX=temp1d;
            def = "bunchCrossingX";
            break;
        case 7:
            configurationFileEntry = "bunchCrossingY"; 
            bunchCrossingY=temp1d;
            def = "bunchCrossingY";
            break;
        case 8:
            configurationFileEntry = "zPosDistribution"; 
            zPosDistribution=temp1d;
            def = "zPosDistribution";
            break;
        case 9:
            configurationFileEntry = "innerSiliconNoise"; 
            innerSiliconNoise=temp2d;
            def = "innerSiliconNoise";
            break;
        case 10:
            configurationFileEntry = "outerSiliconNoise"; 
            outerSiliconNoise=temp2d;
            def = "outerSiliconNoise";
            break;
        case 11:
            configurationFileEntry = "innerCountsDistribution"; 
            innerCountsDistribution=temp1d;
            def = "innerCountsDistribution";
            break;
        case 12:
            configurationFileEntry = "outerCountsDistribution"; 
            outerCountsDistribution=temp1d;
            def = "outerCountsDistribution";
            break;
        case 13:
            configurationFileEntry = "pixelActivationMap"; 
            pixelActivationMap=temp2d;
            def = "pixelActivationMap";
            break;
        case 14:
            configurationFileEntry = "pixelActivationCount"; 
            pixelActivationCount=temp1d;
            def = "pixelActivationCount";
            break;
    }

    if(strcmp(chooseDistName->GetText(), "<default>") == 0) chooseDistName->SetText(def.c_str());

    if (selPDF != 8)
    {
        TFile * rootfile = new TFile(fTextEntry661->GetText(), "UPDATE");
        if(!rootfile->IsOpen())
        {
            std::cerr << "\nERROR while opening / creating the input ROOT file!";
            return;
        }
        if(temp1dAssigned)
        {
            std::cerr << "\nWriting TObject to input TFile...\n";
            if(!rootfile->FindObject(chooseDistName->GetText()))
                temp1d->Write(chooseDistName->GetText());
        }
        else
        {
            if(temp2dAssigned)
            {
                std::cerr << "\nWriting TObject to input TFile...\n";
                if(!rootfile->FindObject(chooseDistName->GetText()))
                    temp2d->Write(chooseDistName->GetText());
            }
        }

        rootfile->Close();
    }

    configurationFileEntry = configurationFileEntry + "=" + std::string(chooseDistName->GetText());
    configurationFileContent = configurationFileContent + "\n" + configurationFileEntry;
    std::cerr << "\n\nProbability density function assigned.";
    temp1dAssigned = false;
    temp2dAssigned = false;
    chooseDistName->SetText("<default>");
}

//_________________________________________________________________________________
void simulationConfigForm::writeOutputFile()
{
    std::cerr << "\n\nWrite configuration and .root file.\n";
    std::string body;
    body = body + "\nsimInputRootFileName="          + std::string(fTextEntry661->GetText());
    body = body + "\nsimRootFileName="               + std::string(fTextEntry664->GetText());
    body = body + "\neventNumber="                   + std::to_string(noEvents->GetIntNumber());
    body = body + "\nreportEvery="                   + std::to_string(reportInterval->GetIntNumber());
    body = body + "\nrndSeed="                   + std::to_string(rndSeed->GetIntNumber());
    body = body + "\nbeamPipeTickness="              + std::to_string(beamPipeTick->GetIntNumber() / (double)1e6);
    body = body + "\ninnerSiliconRadius="            + std::to_string(innerTRadius->GetIntNumber() / (double)1e3);
    body = body + "\ninnerSiLenght="                 + std::to_string(innerTLength->GetIntNumber() / (double)1e3);
    body = body + "\nouterSiliconRadius="            + std::to_string(outerTRadius->GetIntNumber() / (double)1e3);
    body = body + "\nouterSiLenght="                 + std::to_string(outerTLength->GetIntNumber() / (double)1e3);
    body = body + "\nsiliconTickness="               + std::to_string(siliconTickness->GetIntNumber() / (double)1e6);
    body = body + "\nbeamPipeRadius="                + std::to_string(beamPipeRadius->GetIntNumber() / (double)1e3);
    body = body + "\nbeamPipeLenght="                + std::to_string(5); //5 meters (big value, not critical!)
    body = body + "\nsingleCollisionInEvent="        + std::to_string(!fCheckButton681->IsOn());
    body = body + "\nsingleEventPersistenceEnabled=" + std::to_string(fCheckButton682->IsOn());
    body = body + "\nenableSoftParticlesNoise="      + std::to_string(fCheckButton683->IsOn());
    body = body + "\nenableHitGaussianSmearing="     + std::to_string(fCheckButton684->IsOn());
    body = body + "\nhitClusterActivation="          + std::to_string(fCheckButton685->IsOn());
    body = body + configurationFileContent;
    std::cerr << "\x1b[33;40;3m" << body << "\x1b[0m";

    std::ofstream filestream;
    filestream.open(configFilePath->GetText());
    filestream << body;
    filestream.close();
}

//_________________________________________________________________________________
//DESTRUCTOR (CLEANUP EVERYTHING)
simulationConfigForm::~simulationConfigForm()
{
    
}
