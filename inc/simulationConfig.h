/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

/*  THIS FILE IS NOT INTENDED FOR ACliC COMPILATION, BUT FOR CLING INTERPRETER  */

#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGFontDialog
#include "TGFontDialog.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TRootContextMenu
#include "TRootContextMenu.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif

#include "Riostream.h"
#include <fstream>
#include <string>
#include <TF1.h>
#include <TF2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>

//_________________________________________________________________________________
//CLASS DECLARATION
class simulationConfigForm 
{
    RQ_OBJECT("simulationConfigForm")
    
    
    public:
        simulationConfigForm(const TGWindow *p,UInt_t w,UInt_t h);
        virtual ~simulationConfigForm();

        //SLOTS
        void openTBrowser();
        void plotPreview();
        void writeOutputFile();
        void assign();

    private:

        //GUI OBJECT:
        TGListBox *selectPdfListBox;
        TGListBox *assignListBox;
        TGNumberEntry *par0;
        TGNumberEntry *par1;
        TGNumberEntry *par2;
        TGNumberEntry *par3;
        TRootEmbeddedCanvas *previewCanvas;

        TGTextEntry *chooseDistName;    // CHOOSE DISTRIBUTION NAME
        TGTextEntry *fTextEntry661;     // simInputRootFileName
        TGTextEntry *fTextEntry664;     // simRootFileName
        TGTextEntry *configFilePath;     // CONFIGURATION FILE PATH

        TGNumberEntry *noEvents;        // eventNumber
        TGNumberEntry *reportInterval;  // reportEvery
        TGNumberEntry *rndSeed;         // rndSeed


        TGCheckButton *fCheckButton681; // not(singleCollisionInEvent)
        TGCheckButton *fCheckButton682; // singleEventPersistenceEnabled
        TGCheckButton *fCheckButton683; // enableSoftParticlesNoise
        TGCheckButton *fCheckButton684; // enableHitGaussianSmearing
        TGCheckButton *fCheckButton685; // hitClusterActivation

        TGNumberEntry *beamPipeTick;    // beamPipeTickness
        TGNumberEntry *innerTRadius;    // innerSiliconRadius
        TGNumberEntry *innerTLength;    // innerSiLenght
        TGNumberEntry *outerTRadius;    // outerSiliconRadius
        TGNumberEntry *outerTLength;    // outerSiLenght
        TGNumberEntry *siliconTickness; // siliconTickness
        TGNumberEntry *beamPipeRadius;  // beamPipeRadius
                                        //beamPipeLenght


        //DATA OBJECTS:
        TH1D * temp1d;
        TH2D * temp2d;
        bool temp1dAssigned = false;
        bool temp2dAssigned = false;

        TH1D * collisionPerEventDistribution    = nullptr;
        TH1D * momentumDistribution             = nullptr;
        TH1D * etaDistribution                  = nullptr;
        TH1D * multiplicityDistribution         = nullptr;
        TH1D * phiDistribution                  = nullptr;
        TH1D * bunchCrossingX                   = nullptr;
        TH1D * bunchCrossingY                   = nullptr;
        TH1D * zPosDistribution                 = nullptr;
        TH2D * innerSiliconNoise                = nullptr;
        TH2D * outerSiliconNoise                = nullptr;
        TH1D * innerCountsDistribution          = nullptr;
        TH1D * outerCountsDistribution          = nullptr;
        TH2D * pixelActivationMap               = nullptr;
        TH1D * pixelActivationCount             = nullptr;
        
        std::string configurationFileContent;

        //STRINGS, FILES AND STREAMS
};

#endif