//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// VRadiobiologicalAccumulable.hh;
// This is a purely virtual class like G4Vaccumulable.
// it only adds the method Accumulate (RadioBioHit*), and is needed
// to permit a simpler and user-friendly way to create new
// radiobiological quantities, thanks to the loop in RadioBioSD
// class.


#ifndef RadiobiologyVRadiobiologicalAccumulable_H
#define RadiobiologyVRadiobiologicalAccumulable_H 1

#include "RadiobiologyHit.hh"


class RadiobiologyVRadiobiologicalAccumulable : public G4VAccumulable
{
public:
    virtual void Accumulate(RadiobiologyHit*) = 0;

    RadiobiologyVRadiobiologicalAccumulable(G4String name):
        G4VAccumulable(name)
    {;}

    virtual ~RadiobiologyVRadiobiologicalAccumulable()
    {;}

};
#endif

