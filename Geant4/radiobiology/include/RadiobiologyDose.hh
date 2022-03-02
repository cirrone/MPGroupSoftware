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
// AUTHOR: D. Chiappara
// Last modified: 19/11/14
//

#ifndef RadiobiologyDose_h
#define RadiobiologyDose_h 1

#include "globals.hh"
#include <valarray>

#include "RadiobiologyVRadiobiologicalQuantity.hh"

class RadiobiologyDetectorConstruction;
class RadiobiologyDoseAccumulable;
class RadiobiologyDoseMessenger;

class RadiobiologyDose : public RadiobiologyVRadiobiologicalQuantity
{
private:
    
    // RBE should be a friend to be able to see the dose
    // and use it for its calculations
    friend class RBE;

public:
    RadiobiologyDose();
    ~RadiobiologyDose();

    // Virtual methods to override
    void virtual AddFromAccumulable(G4VAccumulable*) override;
    void virtual SetFromAccumulable(G4VAccumulable*) override;
    void virtual Initialize() override;
    void virtual Compute() override;
    void virtual Reset() override;
    void virtual Store() override;
    void virtual PrintParameters() override;

    
private:

    // Deposited energy and dose array
    array_type fEnDep;
    array_type fDose;

    // To be used for accumulation
    void SetEnergyDeposit(const array_type Dep) {fEnDep = Dep; }
    void AddEnergyDeposit(const array_type Dep) {fEnDep += Dep; }

};

#endif // Dose_h
