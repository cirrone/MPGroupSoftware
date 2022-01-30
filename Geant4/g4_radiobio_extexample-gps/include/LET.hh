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
// Hadrontherapy advanced example for Geant4
// See more at: https://twiki.cern.ch/twiki/bin/view/Geant4/AdvancedExamplesHadrontherapy

#ifndef LET_h
#define LET_h 1
#include "G4AnalysisManager.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include <fstream>
#include <vector>
#include <string>
#include <valarray>


#include "VRadiobiologicalQuantity.hh"

class G4Material;

class RadioBioHit;
class LETAccumulable;
class LETMessenger;
class IonLet;

class LET : public VRadiobiologicalQuantity
{
public:

    LET();
    ~LET();

    // Virtual methods to override
    void virtual AddFromAccumulable(G4VAccumulable*) override;
    void virtual SetFromAccumulable(G4VAccumulable*) override;
    void virtual Initialize() override;
    void virtual Compute() override;
    void virtual Reset() override;
    void virtual Store() override;
    void virtual PrintParameters() override;
    
private:
    // Numerator and Denominator of Track-averaged LET
    array_type fNTotalLETT;
    array_type fDTotalLETT;

    // Numerator and Denominator of Dose-averaged LET
    array_type fNTotalLETD;
    array_type fDTotalLETD;

    // Track-averaged and Dose-averaged LET
    array_type fTotalLETT;
    array_type fTotalLETD;

    std::vector<IonLet> IonLetStore;

    std::ofstream ofs;

    // To be used for accumulation
    void SetNTotalLETT(const array_type NTotalLETT) {fNTotalLETT = NTotalLETT; }
    void SetNTotalLETD(const array_type NTotalLETD) {fNTotalLETD = NTotalLETD; }
    void SetDTotalLETT(const array_type DTotalLETT) {fDTotalLETT = DTotalLETT; }
    void SetDTotalLETD(const array_type DTotalLETD) {fDTotalLETD = DTotalLETD; }

    void AddNTotalLETT(const array_type NTotalLETT) {fNTotalLETT += NTotalLETT; }
    void AddNTotalLETD(const array_type NTotalLETD) {fNTotalLETD += NTotalLETD; }
    void AddDTotalLETT(const array_type DTotalLETT) {fDTotalLETT += DTotalLETT; }
    void AddDTotalLETD(const array_type DTotalLETD) {fDTotalLETD += DTotalLETD; }

    // To use to add an ion to the store (or merge data)
    void AddIon(const IonLet ion);

};

#endif //LET_h
