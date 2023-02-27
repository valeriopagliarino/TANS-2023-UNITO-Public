#ifndef UNITS_H
#define UNITS_H
/*
*   Tecniche di Analisi Numerica e Simulazione
*   Dipartimento di Fisica - Università degli Studi di Torino 
*   Authors: Enrica Bergalla e Valerio Pagliarino
*   Title: Software di simulazione Monte Carlo e ricostruzione di vertici 
*   Date: December 2022 - Licenza Creative Commons CC BY-SA 3.0 IT
*/

/*
*   The numerical values in this simple Monte Carlo simulation & reconstruction tools
*   are always represented in International System MKS units. This header file contains
*   definitions of units and contants
*/

Double_t    kg  = 1;
Double_t    g   = 0.001 * kg;
Double_t    mg  = 0.001 * g;
Double_t    ug  = 0.001 * mg;

Double_t    m   = 1;
Double_t    km  = 1000 * m;
Double_t    cm  = 0.01  * m;
Double_t    mm  = 0.001 * m;
Double_t    um  = 0.001 * mm;
Double_t    nm  = 0.001 * um;

Double_t    s   = 1;
Double_t    ms  = 0.001 * s;
Double_t    us  = 0.001 * ms;
Double_t    ns  = 0.001 * us;
Double_t    ps  = 0.001 * ns;

Double_t    V   = 1;
Double_t    mV  = 0.001 * V;
Double_t    kV  = 1000 * V;

Double_t    T   = 1;
Double_t    mT  = 0.001 * T;
Double_t    uT  = 0.001 * mT;

Double_t    A   = 1;
Double_t    kA  = 1000 * A;
Double_t    mA  = 0.001 * A;
Double_t    uA  = 0.001 * mA;
Double_t    nA  = 0.001 * uA;

Double_t    C   = 1;
Double_t    mC  = 0.001 * C;
Double_t    uC  = 0.001 * mC;
Double_t    pC  = 0.001 * uC;
Double_t    fC  = 0.001 * pC;

Double_t    c   = 299792458 * m / s;
Double_t    e   = 1.60217663e-19 * C;
Double_t    h   = 6.62607015e-34 * kg * m * m / s;
Double_t    pi  = 3.1415926535;
Double_t    eV  = e * V;

Double_t    keV = 1000 * eV;
Double_t    MeV = 1000 * keV;
Double_t    GeV = 1000 * MeV;
Double_t    TeV = 1000 * GeV;

#endif