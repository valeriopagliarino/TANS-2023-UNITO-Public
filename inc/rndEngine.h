#ifndef RNDENGINE_H
#define RNDENGINE_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

#include<TRandom3.h>

/// @brief This class is a singleton that represents the engine generating collision using Monte Carlo distributions
class RndEngine : public TRandom3
{
    public:
        RndEngine();
        ~RndEngine();
        
    private:

    #if !defined(__CLING__)
    //ClassDef(RndEngine, 1);
    #endif
};

#endif