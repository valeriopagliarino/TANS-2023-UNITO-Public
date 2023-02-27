#ifndef HIT_H
#define HIT_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TObject.h>
#include<TVector3.h>

/// @brief The Hits class is a container class inheriting from Vector3D that represents an hit with a sensitive detector or a non-sensitive material
class Hit: public TVector3
{
    public:
        Hit() {SetHitId();}
        Hit(const Hit &hitSource) : TVector3(){
            isOnSensitiveDetector = hitSource.isOnSensitiveDetector;
            sensitiveDetectorId = hitSource.sensitiveDetectorId;
            edep = hitSource.edep;
            hitID = hitSource.hitID;
        }

        ~Hit() {};

        Bool_t        isOnSensitiveDetector = false;
        Int_t         sensitiveDetectorId   = 0;
        Double_t      edep = 0;
        
        ULong64_t GetHitId() {return hitID;}
        void      SetT(Double_t tt) {t = tt;}
        Double_t  GetT() {return t;}

    private:
        Double_t t;
        static ULong64_t hitIdGenerator; //!
        ULong64_t hitID;
        void SetHitId() {hitID = ++hitIdGenerator;}

    ClassDef(Hit, 1);
};

//Definition of static class members
ULong64_t Hit::hitIdGenerator;

//Directives for dictionary generation by ROOTCLING for ACliC
#if defined(__ROOTCLING__)
#pragma link C++ class Hit;
#endif

#endif