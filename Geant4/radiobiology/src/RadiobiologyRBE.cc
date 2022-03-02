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

#include "RadiobiologyRBE.hh"
#include "RadiobiologyVoxelizedSensitiveDetector.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Pow.hh"
#include "RadiobiologyRBEAccumulable.hh"
#include "RadiobiologyRBEMessenger.hh"

// Note that dose is needed in order to fully calculate RBE using
// this class. Therefore, here, the correct dependencies must be
// added.
#include "RadiobiologyManager.hh"
#include "RadiobiologyDose.hh"

#include <G4NistManager.hh>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <numeric>

#define width 15L


RadiobiologyRBE::RadiobiologyRBE():
RadiobiologyVRadiobiologicalQuantity(),
    fAlphaX(0.),
    fBetaX(0.),
    fDoseCut(0.),
    fActiveCellLine(""),
    fActiveTableEnergy(0),
    fActiveTableAlpha(0),
    fActiveTableBeta(0),
    fSaved(false)
{
    fPath = "RadioBio";

    // CreateMessenger
    fMessenger = new RadiobiologyRBEMessenger(this);

    Initialize();
}

RadiobiologyRBE::~RadiobiologyRBE()
{
  delete fMessenger;
}

void RadiobiologyRBE::Initialize()
{
    fLnS.resize(RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetTotalVoxelNumber());
    fDoseX.resize(RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetTotalVoxelNumber());

    fInitialized = true;
}

void RadiobiologyRBE::Store()
{
    StoreAlphaAndBeta();
    StoreRBE();
}

void RadiobiologyRBE::PrintParameters()
{
    G4cout << "*******************************************" << G4endl
           << "******* Parameters of the class RBE *******" << G4endl
           << "*******************************************" << G4endl;
           PrintVirtualParameters();
    G4cout << "** RBE Cell line: " << fActiveCellLine << G4endl;
    G4cout << "** RBE Dose threshold value: " << fDoseCut / gray << " gray" << G4endl;
    G4cout << "** RBE Alpha X value: " << fAlphaX * gray << " 1/gray" << G4endl;
    G4cout << "** RBE Beta X value: " << fBetaX * pow(gray, 2.0) << " 1/gray2" << G4endl;
    G4cout << "*******************************************" << G4endl;

}

/**
  * @short Split string into parts using a delimiter.
  *
  * @param line a string to be splitted
  * @param delimiter a string to be looked for
  *
  * Usage: Help function for reading CSV
  * Also remove spaces and quotes around.
  * Note: If delimiter is inside a string, the function fails!
  */
std::vector<G4String> split(const G4String& line, const G4String& delimiter)
{
    std::vector<G4String> result;

    size_t current = 0;
    size_t pos = 0;

    while(pos != G4String::npos)
    {
        pos = line.find(delimiter, current);
        G4String token = line.substr(current, pos - current);
        
        
        G4StrUtil::strip(token);
        G4StrUtil::strip(token, '\"');

        result.push_back(token);
        current = pos + delimiter.size();
    }
    return result;
}

void RadiobiologyRBE::LoadLEMTable(G4String path)
{
    // TODO: Check for presence? Perhaps we want to be able to load more

    std::ifstream in(path);
    if (!in)
    {
        std::stringstream ss;
        ss << "Cannot open LEM table input file: " << path;
        G4Exception("RBE::LoadData", "WrongTable", FatalException, ss.str().c_str());
    }

    // Start with the first line
    G4String line;
    if (!getline(in, line))
    {
        std::stringstream ss;
        ss << "Cannot read header from the LEM table file: " << path;
        G4Exception("RBE::LoadLEMTable", "CannotReadHeader", FatalException, ss.str().c_str());
    }
    std::vector<G4String> header = split(line, ",");

    // Find the order of columns
    std::vector<G4String> columns = { "alpha_x", "beta_x", "D_t", "specific_energy", "alpha", "beta", "cell", "particle"};
    std::map<G4String, int> columnIndices;
    for (auto columnName : columns)
    {
        auto pos = find(header.begin(), header.end(), columnName);
        if (pos == header.end())
        {
            std::stringstream ss;
            ss << "Column " << columnName << " not present in the LEM table file.";
            G4Exception("RBE::LoadLEMTable", "ColumnNotPresent", FatalException, ss.str().c_str());
        }
        else
        {
            columnIndices[columnName] = distance(header.begin(), pos);
        }
    }

    // Continue line by line
    while (getline(in, line))
    {
        std::vector<G4String> lineParts = split(line, ",");
        G4String cellLine = lineParts[columnIndices["cell"]];
        //G4int element = elements.at(lineParts[columnIndices["particle"]]);
        G4NistManager* man = G4NistManager::Instance();
        G4int element = man->FindOrBuildElement(lineParts[columnIndices["particle"]])->GetZasInt();

        fTablesEnergy[cellLine][element].push_back(
            std::stod(lineParts[columnIndices["specific_energy"]]) * MeV
        );
        fTablesAlpha[cellLine][element].push_back(
            stod(lineParts[columnIndices["alpha"]])
        );
        /* fTablesLet[cellLine][element].push_back(
            stod(lineParts[columnIndices["let"]])
        ); */
        fTablesBeta[cellLine][element].push_back(
            stod(lineParts[columnIndices["beta"]])
        );

        fTablesAlphaX[cellLine] = stod(lineParts[columnIndices["alpha_x"]]) / gray;
        fTablesBetaX[cellLine] = stod(lineParts[columnIndices["beta_x"]]) / (gray * gray);
        fTablesDoseCut[cellLine] = stod(lineParts[columnIndices["D_t"]]) * gray;
    }

    // Sort the tables by energy
    // (https://stackoverflow.com/a/12399290/2692780)
    for (auto aPair : fTablesEnergy)
    {
        for (auto ePair : aPair.second)
        {
            std::vector<G4double>& tableEnergy = fTablesEnergy[aPair.first][ePair.first];
            std::vector<G4double>& tableAlpha = fTablesAlpha[aPair.first][ePair.first];
            std::vector<G4double>& tableBeta = fTablesBeta[aPair.first][ePair.first];

            std::vector<size_t> idx(tableEnergy.size());
            iota(idx.begin(), idx.end(), 0);
            sort(idx.begin(), idx.end(),
                [&tableEnergy](size_t i1, size_t i2) {return tableEnergy[i1] < tableEnergy[i2];});

            std::vector<std::vector<G4double>*> tables = {
                &tableEnergy, &tableAlpha, &tableBeta
            };
            for (std::vector<G4double>* table : tables)
            {
                std::vector<G4double> copy = *table;
                for (size_t i = 0; i < copy.size(); ++i)
                {
                    (*table)[i] = copy[idx[i]];
                }
                // G4cout << (*table)[0];
                // reorder(*table, idx);
                // G4cout << (*table)[0] << G4endl;
            }
        }
    }

    if (fVerboseLevel > 0)
    {
        G4cout << "RBE: read LEM data for the following cell lines and elements [number of points]: " << G4endl;
        for (auto aPair : fTablesEnergy)
        {
            G4cout << "- " << aPair.first << ": ";
            for (auto ePair : aPair.second)
            {
                G4cout << ePair.first << "[" << ePair.second.size() << "] ";
            }
            G4cout << G4endl;
        }
    }
}

void RadiobiologyRBE::SetCellLine(G4String name)
{
    G4cout << "*************************" << G4endl
           << "*******SetCellLine*******" << G4endl
           << "*************************" << G4endl;
    if (fTablesEnergy.size() == 0)
    {
        G4Exception("RBE::SetCellLine", "NoCellLine", FatalException, "Cannot select cell line, probably LEM table not loaded yet?");
    }
    if (fTablesEnergy.find(name) == fTablesEnergy.end())
    {
        std::stringstream str;
        str << "Cell line " << name << " not found in the LEM table.";
        G4Exception("RBE::SetCellLine", "", FatalException, str.str().c_str());
    }
    else
    {
        fAlphaX = fTablesAlphaX[name];
        fBetaX = fTablesBetaX[name];
        fDoseCut = fTablesDoseCut[name];

        fActiveTableEnergy = &fTablesEnergy[name];
        fActiveTableAlpha = &fTablesAlpha[name];
        fActiveTableBeta = &fTablesBeta[name];

        fMinZ = 0;
        fMaxZ = 0;
        fMinEnergies.clear();
        fMaxEnergies.clear();

        for (auto energyPair : *fActiveTableEnergy)
        {
            if (!fMinZ || (energyPair.first < fMinZ)) fMinZ = energyPair.first;
            if (energyPair.first > fMaxZ) fMaxZ = energyPair.first;

            fMinEnergies[energyPair.first] = energyPair.second[0];
            fMaxEnergies[energyPair.first] = energyPair.second[energyPair.second.size() - 1];
        }
    }

    fActiveCellLine = name;

    if (fVerboseLevel > 0)
    {
        G4cout << "RBE: cell line " << name << " selected." << G4endl;
    }
}

std::tuple<G4double, G4double> RadiobiologyRBE::GetHitAlphaAndBeta(G4double E, G4int Z)
{
    if (!fActiveTableEnergy)
    {
        G4Exception("RBE::GetHitAlphaAndBeta", "NoCellLine", FatalException, "No cell line selected. Please, do it using the /rbe/cellLine command.");
    }

    // Check we are in the tables
    if ((Z < fMinZ) || (Z > fMaxZ))
    {
        if (fVerboseLevel > 2)
        {
            std::stringstream str;
            str << "Alpha & beta calculation supported only for ";
            str << fMinZ << " <= Z <= " << fMaxZ << ", but " << Z << " requested.";
            G4Exception("RBE::GetHitAlphaAndBeta", "", JustWarning, str.str().c_str());
        }
        return std::make_tuple<G4double, G4double>( 0.0, 0.0 ); //out of table!
    }
    if ((E < fMinEnergies[Z]) || (E >= fMaxEnergies[Z]))
    {
        if (fVerboseLevel > 2)
        {
            G4cout << "RBE hit: Z=" << Z << ", E=" << E << " => out of LEM table";
            if (fVerboseLevel > 3)
            {
                G4cout << " (" << fMinEnergies[Z] << " to " << fMaxEnergies[Z] << " MeV)";
            }
            G4cout << G4endl;
        }
        return std::make_tuple<G4double, G4double>( 0.0, 0.0 ); //out of table!
    }

    std::vector<G4double>& vecEnergy = (*fActiveTableEnergy)[Z];
    std::vector<G4double>& vecAlpha = (*fActiveTableAlpha)[Z];
    std::vector<G4double>& vecBeta = (*fActiveTableBeta)[Z];

    // Find the row in energy tables
    const auto eLarger = upper_bound(begin(vecEnergy), end(vecEnergy), E);
    const G4int lower = distance(begin(vecEnergy), eLarger) - 1;
    const G4int upper = lower + 1;

    // Interpolation
    const G4double energyLower = vecEnergy[lower];
    const G4double energyUpper = vecEnergy[upper];
    const G4double energyFraction = (E - energyLower) / (energyUpper - energyLower);

    //linear interpolation along E
    const G4double alpha = ((1 - energyFraction) * vecAlpha[lower] + energyFraction * vecAlpha[upper]);
    const G4double beta = ((1 - energyFraction) * vecBeta[lower] + energyFraction * vecBeta[upper]);
    if (fVerboseLevel > 2)
    {
        G4cout << "RBE hit: Z=" << Z << ", E=" << E << " => alpha=" << alpha << ", beta=" << beta << G4endl;
    }

    return std::make_tuple(alpha, beta);
}

// Zaider & Rossi alpha & Beta mean
void RadiobiologyRBE::ComputeAlphaAndBeta()
{
    // Skip RBE computation if calculation not enabled.
    if(!fCalculationEnabled)
    {
      if(fVerboseLevel > 0)
      {
        G4cout << "RBE::ComputeAlphaAndBeta() called but skipped as calculation not enabled"
               << G4endl;
      }
      return;
    }

    for(auto f : fDenominator)
        if(f <= 0.0001)
            G4cout << "denominator in RBE too small: fDeonimator = " << f <<  G4endl;

    if (fVerboseLevel > 0)
    {
        G4cout << "RBE: Computing alpha and beta..." << G4endl;
    }
    fAlpha = fAlphaNumerator / (fDenominator * gray);

    fBeta = pow(fBetaNumerator / (fDenominator * gray), 2);

    //g4pow -> powN(fBetaNumerator / fDenominator * gray, 2)
}

void RadiobiologyRBE::ComputeRBE()
{
    // Skip RBE computation if calculation not enabled.
    if(!fCalculationEnabled)
    {
      if(fVerboseLevel > 0)
      {
        G4cout << "RBE::ComputeRBE() called but skipped as calculation not enabled"
               << G4endl;
      }
      return;
    }

    if (fVerboseLevel > 0)
    {
        G4cout << "RBE: Computing survival and RBE..." << G4endl;
    }
    G4double smax = fAlphaX + 2 * fBetaX * fDoseCut;


    for (G4int i = 0; i < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetTotalVoxelNumber(); i++)
    {
        if (std::isnan(fAlpha[i]) || std::isnan(fBeta[i]))
        {
            fLnS[i] = 0.0;
            fDoseX[i] = 0.0;
        }
        else if (fDose[i] <= fDoseCut)
        {
            fLnS[i] = -(fAlpha[i] * fDose[i]) - (fBeta[i] * (pow(fDose[i], 2.0))) ;
            fDoseX[i] = sqrt((-fLnS[i] / fBetaX) + pow((fAlphaX / (2 * fBetaX)), 2.0)) - (fAlphaX / (2 * fBetaX));
        }
        else
        {
            G4double ln_Scut = -(fAlpha[i] * fDoseCut) - (fBeta[i] * (pow((fDoseCut), 2.0)));
            fLnS[i] = ln_Scut - ((fDose[i] - fDoseCut) * smax);

            // TODO: CHECK THIS!!!
            fDoseX[i] = ( (-fLnS[i] + ln_Scut) / smax ) + fDoseCut;
        }
    }
    fRBE = fDoseX / fDose;
    fSurvival = exp(fLnS);
}

void RadiobiologyRBE::Compute()
{
    // Skip RBE computation if calculation not enabled.
    if(!fCalculationEnabled)
    {
      if(fVerboseLevel > 0)
      {
        G4cout << "RBE::Compute() called but skipped as calculation not enabled"
               << G4endl;
      }
      return;
    }

    if(fCalculated == true)
        return;

    GetDose();

    ComputeAlphaAndBeta();
    ComputeRBE();

    // If this method reached the bottom, set calculated to true
    fCalculated = true;
}

void RadiobiologyRBE::GetDose()
{
    // Get the pointer to dose. If it does not exist, launch an exception
    const RadiobiologyDose* dose = dynamic_cast<const RadiobiologyDose*>(
                                RadiobiologyManager::GetInstance()->GetQuantity("Dose"));
    if(dose == nullptr)
        G4Exception("RBE::Compute", "RBEMissingDose", FatalException,
                    "Trying to compute RBE without knowing the dose. Please add a valid dose or "
                    "disable RBE calculation");

    // Check whether dose has been calculated.
    // If not, give a warning
    if(!dose->HasBeenCalculated())
        G4Exception("RBE::Compute", "RBEDoseNotCalculated", JustWarning,
                    "Dose has not been calculated yet while computing RBE, that will be wrong."
                    " Please, first calculate dose");
    // Copy the proper vector from Dose class to RBE class
    fDose = dose->fDose;
}

void RadiobiologyRBE::SetDenominator(const RadiobiologyRBE::array_type denom)
{
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Setting denominator..."  << G4endl;
    }
    fDenominator = denom;
}

void RadiobiologyRBE::AddDenominator(const RadiobiologyRBE::array_type denom)
{
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Adding denominator...";
    }
    if (fDenominator.size())
    {
        fDenominator += denom;
    }
    else
    {
        if (fVerboseLevel > 1)
        {
            G4cout << " (created empty array)";
        }
        fDenominator = denom;
    }
    G4cout << G4endl;
}

void RadiobiologyRBE::SetAlphaNumerator(const RadiobiologyRBE::array_type alpha)
{
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Setting alpha numerator..."  << G4endl;
    }
    fAlphaNumerator = alpha;
}

void RadiobiologyRBE::SetBetaNumerator(const RadiobiologyRBE::array_type beta)
{
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Setting beta numerator..." << G4endl;
    }
    fBetaNumerator = beta;
}

void RadiobiologyRBE::AddAlphaNumerator(const RadiobiologyRBE::array_type alpha)
{
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Adding alpha numerator...";
    }
    if (fAlphaNumerator.size())
    {
        fAlphaNumerator += alpha;
    }
    else
    {
        if (fVerboseLevel > 1)
        {
            G4cout << " (created empty array)";
        }
        fAlphaNumerator = alpha;
    }
    G4cout << G4endl;
}

void RadiobiologyRBE::AddBetaNumerator(const RadiobiologyRBE::array_type beta)
{
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Adding beta numerator...";
    }
    if (fBetaNumerator.size())
    {
        fBetaNumerator += beta;
    }
    else
    {
        if (fVerboseLevel > 1)
        {
            G4cout << " (created empty array)";
        }
        fBetaNumerator = beta;
    }
    G4cout << G4endl;
}

void RadiobiologyRBE::StoreAlphaAndBeta()
{
    // Skip RBE storing if calculation not enabled.
    if(!fCalculationEnabled)
    {
      if(fVerboseLevel > 0)
      {
        G4cout << "RBE::StoreAlphaAndBeta() called but skipped as calculation not enabled"
               << G4endl;
      }
      return;
    }

    G4String AlphaBetaPath = fPath + "_AlphaAndBeta.out";
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Writing alpha and beta..." << G4endl;
    }
    std::ofstream ofs(AlphaBetaPath);

    Compute();

    if (ofs.is_open())
    {
        ofs << std::left
            << std::setw(width) << "i" << std::setw(width) << "j" << std::setw(width) << "k"
            << std::setw(width) << "alpha" << std::setw(width) << "beta " << G4endl;

        // Alpha and beta are written only if valid. If value is -nan, 0 is written
        // on the text file
        for(G4int i = 0; i < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongX(); i++)
            for(G4int j = 0; j < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongY(); j++)
                for(G4int k = 0; k < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongZ(); k++)
                {
                    G4int v = RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetThisVoxelNumber(i, j, k);

                    ofs << std::left
                        << std::setw(width) << i << std::setw(width) << j << std::setw(width) << k
                        << std::setw(width) << (std::isnan(fAlpha[v]) ? 0 : (fAlpha[v] * gray))
                        << std::setw(width) << (std::isnan(fBeta[v]) ? 0 : (fBeta[v] * pow(gray, 2.0)))
                        << G4endl;
                }
    }
    if (fVerboseLevel > 0)
    {
        G4cout << "RBE: Alpha and beta written to " << AlphaBetaPath << G4endl;
    }
}

void RadiobiologyRBE::StoreRBE()
{
    // Skip RBE storing if calculation not enabled.
    if(!fCalculationEnabled)
    {
      if(fVerboseLevel > 0)
      {
        G4cout << "RBE::StoreRBE() called but skipped as calculation not enabled"
               << G4endl;
      }
      return;
    }

    G4String RBEPath = fPath + "_RBE.out";
    if(fSaved == true)
        G4Exception("RBE::StoreRBE", "RBEOverwrite", JustWarning,
                    "Overwriting RBE files. To accumulate along runs, use Use \"/RBE/accumulate true\"");
    std::ofstream ofs(RBEPath);

    Compute();

    if (ofs.is_open())
    {
        ofs << std::left
            << std::setw(width) << "i" << std::setw(width) << "j" << std::setw(width) << "k"
            << std::setw(width)<< "Dose(Gy)" << std::setw(width) << "ln(S) "
            << std::setw(width) << "Survival" << std::setw(width) << "DoseB(Gy)"
            << std::setw(width) << "RBE" << G4endl;

        for(G4int i = 0; i < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongX(); i++)
            for(G4int j = 0; j < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongY(); j++)
                for(G4int k = 0; k < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongZ(); k++)
                {
                    G4int v = RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetThisVoxelNumber(i, j, k);

                    ofs << std::left
                        << std::setw(width) << i << std::setw(width) << j << std::setw(width) << k
                        << std::setw(width) << (fDose[v] / gray)
                        << std::setw(width) << fLnS[v]
                        << std::setw(width) << fSurvival[v]
                        << std::setw(width) << fDoseX[v] / gray
                        << std::setw(width) << (std::isnan(fRBE[v]) ? 0. : fRBE[v])
                        << G4endl;
                }
    }
    if (fVerboseLevel > 0)
    {
        G4cout << "RBE: RBE written to " << RBEPath << G4endl;
    }

    fSaved = true;
}

void RadiobiologyRBE::Reset()
{
    if (fVerboseLevel > 1)
    {
        G4cout << "RBE: Reset(): ";
    }
    fAlphaNumerator = 0.0;
    fBetaNumerator = 0.0;
    fDenominator = 0.0;
    fDose = 0.0;
    fCalculated = false;
    if (fVerboseLevel > 1)
    {
        G4cout << fAlphaNumerator.size() << " points." << G4endl;
    }
}

void RadiobiologyRBE::AddFromAccumulable(G4VAccumulable* GenAcc)
{
    RadiobiologyRBEAccumulable* acc = (RadiobiologyRBEAccumulable*) GenAcc;
    AddAlphaNumerator(acc->GetAlphaNumerator());
    AddBetaNumerator(acc->GetBetaNumerator());
    AddDenominator(acc->GetDenominator());

    fCalculated = false;
}

void RadiobiologyRBE::SetFromAccumulable(G4VAccumulable* GenAcc)
{
    RadiobiologyRBEAccumulable* acc = (RadiobiologyRBEAccumulable*) GenAcc;
    SetAlphaNumerator(acc->GetAlphaNumerator());
    SetBetaNumerator(acc->GetBetaNumerator());
    SetDenominator(acc->GetDenominator());

    fCalculated = false;
}
