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
// RBE.hh;
//

#ifndef RadiobiologyRBE_H
#define RadiobiologyRBE_H 1

#include "globals.hh"
#include <vector>
#include <valarray>
#include <map>
#include "G4Pow.hh"

#include "RadiobiologyVRadiobiologicalQuantity.hh"
class RadiobiologyDetectorConstruction;
class RadiobiologyRBEAccumulable;
class RadiobiologyRBEMessenger;
class RadiobiologyVoxelizedSensitiveDetector;


class RadiobiologyRBE : public RadiobiologyVRadiobiologicalQuantity
{
public:
    RadiobiologyRBE();
    ~RadiobiologyRBE();

    // Initialization of data from a CSV file
    void LoadLEMTable(G4String path);

    // Select the cell and update the pointer
    void SetCellLine(G4String name);

    // Calculate alpha and beta for single deposition, {0,0} if not applicable
    std::tuple<G4double, G4double> GetHitAlphaAndBeta(G4double E, G4int Z);


    // Virtual methods to override
    void virtual AddFromAccumulable(G4VAccumulable*) override;
    void virtual SetFromAccumulable(G4VAccumulable*) override;
    void virtual Initialize() override;
    void virtual Compute() override;
    void virtual Reset() override;
    void virtual Store() override;
    void virtual PrintParameters() override;

private:

    G4Pow* g4pow = G4Pow::GetInstance();

    // Calculation
    void ComputeAlphaAndBeta();
    void ComputeRBE();

    // Output to text files (called at the end of run)
    void StoreAlphaAndBeta();
    void StoreRBE();

    // Update the class with accumulated data
    // (To be used for accumulation)
    void SetAlphaNumerator(const array_type alpha);
    void SetBetaNumerator(const array_type beta);
    void SetDenominator(const array_type denom);

    // Accumulation variants necessary for multi-run sumation
    void AddAlphaNumerator(const array_type alpha);
    void AddBetaNumerator(const array_type beta);
    void AddDenominator(const array_type denom);

    // Method to copy dose from the proper Radiobiological Quantity
    void GetDose();

    // Parameters for calculation
    G4double fAlphaX;
    G4double fBetaX;
    G4double fDoseCut;

    // Matrices to be set when accumulated
    array_type fAlpha;
    array_type fBeta;
    array_type fDose; // Note: this is copied from calculation in Dose
    
    array_type fAlphaNumerator;
    array_type fBetaNumerator;
    array_type fDenominator;

    // Matrices of calculated values
    array_type fLnS;
    array_type fSurvival;
    array_type fDoseX;
    array_type fRBE;

    // Available tables and associated values.
    using vector_type = std::map<G4int, std::vector<G4double>>;
    std::map<G4String, vector_type> fTablesEnergy;
    std::map<G4String, vector_type> fTablesAlpha;
    std::map<G4String, vector_type> fTablesBeta;
    std::map<G4String, G4double> fTablesAlphaX;
    std::map<G4String, G4double> fTablesBetaX;
    std::map<G4String, G4double> fTablesDoseCut;

    // Selected tables and associated values.
    // (changed when the cell line is set)
    G4String fActiveCellLine;
    vector_type* fActiveTableEnergy;
    vector_type* fActiveTableAlpha;
    vector_type* fActiveTableBeta;
    std::map<G4int, G4double> fMaxEnergies;
    std::map<G4int, G4double> fMinEnergies;
    G4int fMinZ;
    G4int fMaxZ;
    G4bool fSaved;
};
#endif

