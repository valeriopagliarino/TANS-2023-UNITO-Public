/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/
#include "../inc/reconstructionConfigGUI.h"

//fNumberEntry950 = new TGNumberEntry(fMainFrame1372, (Double_t) 5,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,2.5,7.5);

reconstructionConfig::reconstructionConfig(const TGWindow *p,UInt_t w,UInt_t h)
{
    // main frame
   TGMainFrame *fMainFrame3164 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame3164->SetName("fMainFrame3164");
   fMainFrame3164->SetLayoutBroken(kTRUE);

   // "" group frame
   TGGroupFrame *fGroupFrame560 = new TGGroupFrame(fMainFrame3164,"");
   fGroupFrame560->SetLayoutBroken(kTRUE);

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   TGGC   *uGC;           // will reflect user GC changes
   // graphics context changes
   GCValues_t valationOutput;
   valationOutput.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valationOutput.fForeground);
   gClient->GetColorByName("#e8e8e8",valationOutput.fBackground);
   valationOutput.fFillStyle = kFillSolid;
   valationOutput.fFont = ufont->GetFontHandle();
   valationOutput.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valationOutput, kTRUE);
   simulationOutput = new TGTextEntry(fGroupFrame560, new TGTextBuffer(31),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kOwnBackground);
   simulationOutput->SetMaxLength(4096);
   simulationOutput->SetAlignment(kTextLeft);
   simulationOutput->SetText("./simulationOutput.root");
   simulationOutput->Resize(224,simulationOutput->GetDefaultHeight());
   fGroupFrame560->AddFrame(simulationOutput, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   simulationOutput->MoveResize(16,120,224,20);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valutput;
   valutput.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valutput.fForeground);
   gClient->GetColorByName("#e8e8e8",valutput.fBackground);
   valutput.fFillStyle = kFillSolid;
   valutput.fFont = ufont->GetFontHandle();
   valutput.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valutput, kTRUE);
   recoOutput = new TGTextEntry(fGroupFrame560, new TGTextBuffer(31),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kOwnBackground);
   recoOutput->SetMaxLength(4096);
   recoOutput->SetAlignment(kTextLeft);
   recoOutput->SetText("./recoOutput.root");
   recoOutput->Resize(224,recoOutput->GetDefaultHeight());
   fGroupFrame560->AddFrame(recoOutput, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   recoOutput->MoveResize(16,190,224,20);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valsisOutput;
   valsisOutput.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valsisOutput.fForeground);
   gClient->GetColorByName("#e8e8e8",valsisOutput.fBackground);
   valsisOutput.fFillStyle = kFillSolid;
   valsisOutput.fFont = ufont->GetFontHandle();
   valsisOutput.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valsisOutput, kTRUE);
   analysisOutput = new TGTextEntry(fGroupFrame560, new TGTextBuffer(31),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kOwnBackground);
   analysisOutput->SetMaxLength(4096);
   analysisOutput->SetAlignment(kTextLeft);
   analysisOutput->SetText("./analysisOutput.root");
   analysisOutput->Resize(224,analysisOutput->GetDefaultHeight());
   fGroupFrame560->AddFrame(analysisOutput, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   analysisOutput->MoveResize(16,260,224,20);
   TGLabel *fLabel1070 = new TGLabel(fGroupFrame560,"Configuration file path");
   fLabel1070->SetTextJustify(36);
   fLabel1070->SetMargins(0,0,0,0);
   fLabel1070->SetWrapLength(-1);
   fGroupFrame560->AddFrame(fLabel1070, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel1070->MoveResize(16,24,216,16);
   TGLabel *fLabel1118 = new TGLabel(fGroupFrame560,"Simulation output file path");
   fLabel1118->SetTextJustify(36);
   fLabel1118->SetMargins(0,0,0,0);
   fLabel1118->SetWrapLength(-1);
   fGroupFrame560->AddFrame(fLabel1118, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel1118->MoveResize(16,94,216,16);
   TGLabel *fLabel1160 = new TGLabel(fGroupFrame560,"Reconstruction output file path");
   fLabel1160->SetTextJustify(36);
   fLabel1160->SetMargins(0,0,0,0);
   fLabel1160->SetWrapLength(-1);
   fGroupFrame560->AddFrame(fLabel1160, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel1160->MoveResize(16,164,216,16);
   TGLabel *fLabel1210 = new TGLabel(fGroupFrame560,"Analysis output file path");
   fLabel1210->SetTextJustify(36);
   fLabel1210->SetMargins(0,0,0,0);
   fLabel1210->SetWrapLength(-1);
   fGroupFrame560->AddFrame(fLabel1210, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel1210->MoveResize(16,234,224,16);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valstructionConf;
   valstructionConf.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valstructionConf.fForeground);
   gClient->GetColorByName("#e8e8e8",valstructionConf.fBackground);
   valstructionConf.fFillStyle = kFillSolid;
   valstructionConf.fFont = ufont->GetFontHandle();
   valstructionConf.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valstructionConf, kTRUE);
   reconstructionConf = new TGTextEntry(fGroupFrame560, new TGTextBuffer(31),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kOwnBackground);
   reconstructionConf->SetMaxLength(4096);
   reconstructionConf->SetAlignment(kTextLeft);
   reconstructionConf->SetText("./reconstructionConfig.txt");
   reconstructionConf->Resize(224,reconstructionConf->GetDefaultHeight());
   fGroupFrame560->AddFrame(reconstructionConf, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   reconstructionConf->MoveResize(16,50,224,20);

   fGroupFrame560->SetLayoutManager(new TGVerticalLayout(fGroupFrame560));
   fGroupFrame560->Resize(264,440);
   fMainFrame3164->AddFrame(fGroupFrame560, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame560->MoveResize(16,56,264,440);

   // "RECONSTRUCTION" group frame
   TGGroupFrame *fGroupFrame575 = new TGGroupFrame(fMainFrame3164,"RECONSTRUCTION");
   fGroupFrame575->SetLayoutBroken(kTRUE);
   fNumberEntry950 = new TGNumberEntry(fGroupFrame575, (Double_t) 5,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,2.5,7.5);
   fGroupFrame575->AddFrame(fNumberEntry950, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2)); 
   fNumberEntry950->MoveResize(240,30,58,20);
   fNumberEntry952 = new TGNumberEntry(fGroupFrame575, (Double_t) 20,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,10,40);
   fGroupFrame575->AddFrame(fNumberEntry952, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry952->MoveResize(240,65,58,20);
   fNumberEntry969 = new TGNumberEntry(fGroupFrame575, (Double_t) 1,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,1,5);
   fGroupFrame575->AddFrame(fNumberEntry969, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry969->MoveResize(240,100,58,20);
   fNumberEntry982 = new TGNumberEntry(fGroupFrame575, (Double_t) 100,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,60,100);
   fGroupFrame575->AddFrame(fNumberEntry982, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry982->MoveResize(240,135,58,20);
   enableDeltaPhiMaxCalculationwithMCtruth = new TGCheckButton(fGroupFrame575,"Disable Delta Phi Max calc. through MC truth");
   enableDeltaPhiMaxCalculationwithMCtruth->SetTextJustify(36);
   enableDeltaPhiMaxCalculationwithMCtruth->SetMargins(0,0,0,0);
   enableDeltaPhiMaxCalculationwithMCtruth->SetWrapLength(-1);
   fGroupFrame575->AddFrame(enableDeltaPhiMaxCalculationwithMCtruth, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   enableDeltaPhiMaxCalculationwithMCtruth->MoveResize(14,180,288,17);
   TGLabel *fLabel2077 = new TGLabel(fGroupFrame575,"Running window size (mm)");
   fLabel2077->SetTextJustify(36);
   fLabel2077->SetMargins(0,0,0,0);
   fLabel2077->SetWrapLength(-1);
   fGroupFrame575->AddFrame(fLabel2077, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2077->MoveResize(16,34,200,16);
   TGLabel *fLabel2127 = new TGLabel(fGroupFrame575,"Running window step \n (percent. of r. window size)");
   fLabel2127->SetTextJustify(36);
   fLabel2127->SetMargins(0,0,0,0);
   fLabel2127->SetWrapLength(-1);
   fGroupFrame575->AddFrame(fLabel2127, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2127->MoveResize(16,62,200,32);
   TGLabel *fLabel2257 = new TGLabel(fGroupFrame575,"Min. num of candidate vertices");
   fLabel2257->SetTextJustify(36);
   fLabel2257->SetMargins(0,0,0,0);
   fLabel2257->SetWrapLength(-1);
   fGroupFrame575->AddFrame(fLabel2257, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2257->MoveResize(16,104,200,24);
   TGLabel *fLabel2293 = new TGLabel(fGroupFrame575,"Max. num. c.v.o.w. (see readme)");
   fLabel2293->SetTextJustify(36);
   fLabel2293->SetMargins(0,0,0,0);
   fLabel2293->SetWrapLength(-1);
   fGroupFrame575->AddFrame(fLabel2293, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2293->MoveResize(16,136,216,24);

   fGroupFrame575->SetLayoutManager(new TGVerticalLayout(fGroupFrame575));
   fGroupFrame575->Resize(312,224);
   fMainFrame3164->AddFrame(fGroupFrame575, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame575->MoveResize(296,56,312,224);
   GenerateConfigurationFile = new TGTextButton(fMainFrame3164,"Generate Configuration File",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);

   GenerateConfigurationFile->Resize(312,56);
   fMainFrame3164->AddFrame(GenerateConfigurationFile, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   GenerateConfigurationFile->MoveResize(296,504,312,56);

   // "ANALYSIS" group frame
   TGGroupFrame *fGroupFrame1344 = new TGGroupFrame(fMainFrame3164,"ANALYSIS");
   fGroupFrame1344->SetLayoutBroken(kTRUE);
   fNumberEntry1030 = new TGNumberEntry(fGroupFrame1344, (Double_t) 3,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,1,3);
   fGroupFrame1344->AddFrame(fNumberEntry1030, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry1030->MoveResize(240,30,58,20);
   fNumberEntry1051 = new TGNumberEntry(fGroupFrame1344, (Double_t) 3,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,1,10);
   fGroupFrame1344->AddFrame(fNumberEntry1051, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry1051->MoveResize(240,65,58,20);
   fNumberEntry1064 = new TGNumberEntry(fGroupFrame1344, (Double_t) 52,6,-1,(TGNumberFormat::EStyle) TGNumberFormat::kNESReal, (TGNumberFormat::EAttribute) TGNumberFormat::kNEAAnyNumber, (TGNumberFormat::ELimit) TGNumberFormat::kNELLimitMinMax,40,50);
   fGroupFrame1344->AddFrame(fNumberEntry1064, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry1064->MoveResize(240,100,58,20);
   TGLabel *fLabel2381 = new TGLabel(fGroupFrame1344,"Primary vertices n-sigmas");
   fLabel2381->SetTextJustify(36);
   fLabel2381->SetMargins(0,0,0,0);
   fLabel2381->SetWrapLength(-1);
   fGroupFrame1344->AddFrame(fLabel2381, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2381->MoveResize(8,32,216,24);
   TGLabel *fLabel2449 = new TGLabel(fGroupFrame1344,"Min. multiplicity considered");
   fLabel2449->SetTextJustify(36);
   fLabel2449->SetMargins(0,0,0,0);
   fLabel2449->SetWrapLength(-1);
   fGroupFrame1344->AddFrame(fLabel2449, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2449->MoveResize(16,64,207,24);
   TGLabel *fLabel2480 = new TGLabel(fGroupFrame1344,"Max. multiplicity considered");
   fLabel2480->SetTextJustify(36);
   fLabel2480->SetMargins(0,0,0,0);
   fLabel2480->SetWrapLength(-1);
   fGroupFrame1344->AddFrame(fLabel2480, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2480->MoveResize(16,98,208,24);

   fGroupFrame1344->SetLayoutManager(new TGVerticalLayout(fGroupFrame1344));
   fGroupFrame1344->Resize(312,208);
   fMainFrame3164->AddFrame(fGroupFrame1344, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame1344->MoveResize(296,288,312,208);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-14-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t vall1757;
   vall1757.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall1757.fForeground);
   gClient->GetColorByName("#e8e8e8",vall1757.fBackground);
   vall1757.fFillStyle = kFillSolid;
   vall1757.fFont = ufont->GetFontHandle();
   vall1757.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall1757, kTRUE);
   TGLabel *fLabel1757 = new TGLabel(fMainFrame3164,"Configuration file generator for reconstruction",uGC->GetGC(),ufont->GetFontStruct());
   fLabel1757->SetTextJustify(36);
   fLabel1757->SetMargins(0,0,0,0);
   fLabel1757->SetWrapLength(-1);
   fMainFrame3164->AddFrame(fLabel1757, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel1757->MoveResize(16,16,584,32);

   fMainFrame3164->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame3164->MapSubwindows();

   fMainFrame3164->Resize(fMainFrame3164->GetDefaultSize());
   fMainFrame3164->MapWindow();
   fMainFrame3164->Resize(624,579);



   //END CODE GENERATED BY TGUIBUILDER

    GenerateConfigurationFile->Connect("Clicked()","reconstructionConfig",this,"WriteOutputFile()");

}  

