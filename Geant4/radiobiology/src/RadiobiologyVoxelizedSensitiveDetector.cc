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
// FIXME test for the detectorconstruction of the radiobiological
// extended example in Geant4. Taken starting from the Hadrontherapy
// advanced example.
// AUTHOR: D. Chiappara
// Creation Date: 19/05/13
// Last modified: 19/05/13
//

#include "RadiobiologyVoxelizedSensitiveDetector.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"

#include "RadiobiologyDetectorConstruction.hh"
#include "RadiobiologyVoxelizedSensitiveDetectorMessenger.hh"
#include "RadiobiologyRadioBioSD.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"

RadiobiologyVoxelizedSensitiveDetector* RadiobiologyVoxelizedSensitiveDetector::instance = nullptr;

RadiobiologyVoxelizedSensitiveDetector* RadiobiologyVoxelizedSensitiveDetector::CreateInstance(RadiobiologyDetectorConstruction* det, double xWidth,
                                                                       double yWidth, double zWidth)
{
    if (instance)
    {
        delete instance;
        G4Exception("VoxelizedSensitiveDetector::createInstance", "RecreatingVoxelization",
                    FatalException, "Creating another, new, instance of VoxelizedSensitiveDetector");
    }
    instance = new RadiobiologyVoxelizedSensitiveDetector(det, xWidth, yWidth, zWidth);
    return instance;
}

RadiobiologyVoxelizedSensitiveDetector* RadiobiologyVoxelizedSensitiveDetector::GetInstance()
{
    return instance;
}

RadiobiologyVoxelizedSensitiveDetector::RadiobiologyVoxelizedSensitiveDetector(RadiobiologyDetectorConstruction* det, double xWidth,
                           double yWidth, double zWidth):
    fDetector(det), fVoxelWidthX(xWidth), fVoxelWidthY(yWidth),
    fVoxelWidthZ(zWidth), fVoxelNumberAlongX(1), fVoxelNumberAlongY(1),
    fVoxelNumberAlongZ(1), fTotalVoxelNumber(1), fVoxelMass(0.), fVoxelDensity(0.),
    VoxelizedDetectorXDivision(0),
    VoxelizedDetectorYDivision(0), VoxelizedDetectorZDivision(0),
    VoxelizedDetectorXDivisionLog(0), VoxelizedDetectorYDivisionLog(0),
    VoxelizedDetectorZDivisionLog(0), sensitiveLogicalVolume(0),
    VoxelizedDetectorXDivisionPhys(0), VoxelizedDetectorYDivisionPhys(0),
    VoxelizedDetectorZDivisionPhys(0), worldLogical(0), isBuilt(false),
    fVoxelizedSensitiveDetectorMessenger()
{
    fVoxelizedSensitiveDetectorMessenger = new VoxelizedSensitiveDetectorMessenger(this);
    UpdateVoxelVolume();
    CalculateVoxelNumber();
}

RadiobiologyVoxelizedSensitiveDetector::~RadiobiologyVoxelizedSensitiveDetector()
{
    delete fVoxelizedSensitiveDetectorMessenger;
}


void RadiobiologyVoxelizedSensitiveDetector::UpdateVoxelVolume()
{
    fVoxelVolume = fVoxelWidthX * fVoxelWidthY * fVoxelWidthZ;
    fVoxelDensity = fDetector->GetMaterial()->GetDensity();
    fVoxelMass = fVoxelVolume * fVoxelDensity;
}


void RadiobiologyVoxelizedSensitiveDetector::SetVoxelWidth(G4ThreeVector voxWidth)
{
    fVoxelWidthX = voxWidth.getX();
    fVoxelWidthY = voxWidth.getY();
    fVoxelWidthZ = voxWidth.getZ();
    CalculateVoxelNumber();

}

void RadiobiologyVoxelizedSensitiveDetector::SetVoxelWidthX(G4double voxWidthX)
{
    if(fVoxelWidthX == voxWidthX) return;
    fVoxelWidthX = voxWidthX;
    CalculateVoxelNumber();
}

void RadiobiologyVoxelizedSensitiveDetector::SetVoxelWidthY(G4double voxWidthY)
{
    if(fVoxelWidthY == voxWidthY) return;
    fVoxelWidthY = voxWidthY;
    CalculateVoxelNumber();
}

void RadiobiologyVoxelizedSensitiveDetector::SetVoxelWidthZ(G4double voxWidthZ)
{
    if(fVoxelWidthZ == voxWidthZ) return;
    fVoxelWidthZ = voxWidthZ;
    CalculateVoxelNumber();
}

// Calculte number of voxel approximating for an integer number of voxels
// Then recalculates voxels size according to approximations
void RadiobiologyVoxelizedSensitiveDetector::CalculateVoxelNumber()
{
  fVoxelNumberAlongX = G4int(fDetector->GetSizeX() / fVoxelWidthX);
  fVoxelWidthX = fDetector->GetSizeX() / G4double(fVoxelNumberAlongX);

  fVoxelNumberAlongY = G4int(fDetector->GetSizeY() / fVoxelWidthY);
  fVoxelWidthY = fDetector->GetSizeY() / G4double(fVoxelNumberAlongY);

  fVoxelNumberAlongZ = G4int(fDetector->GetSizeZ() / fVoxelWidthZ);
  fVoxelWidthZ = fDetector->GetSizeZ() / G4double(fVoxelNumberAlongZ);

  if(fVoxelNumberAlongY % 2 == 0)
      G4Exception("VoxelizedSensitiveDetector::CalculateVoxelNumber", "VoxelNumberYEven",
                  JustWarning, "Trying to voxelize with an even number of voxels along the Y axis."
                               "Please select an odd number to prevent from warnings due to tracking");

  if(fVoxelNumberAlongZ % 2 == 0)
      G4Exception("VoxelizedSensitiveDetector::CalculateVoxelNumber", "VoxelNumberZEven",
                  JustWarning, "Trying to voxelize with an even number of voxels along the Z axis."
                               "Please select an odd number to prevent from warnings due to tracking");

  fTotalVoxelNumber = fVoxelNumberAlongX * fVoxelNumberAlongY * fVoxelNumberAlongZ;
  G4cout << "ERASE calculated vox number = " << fTotalVoxelNumber << G4endl;

  UpdateVoxelVolume();
}

void RadiobiologyVoxelizedSensitiveDetector::ConstructXDivision()
{
    if(worldLogical == nullptr)
        G4Exception("VoxelizedSensitiveDetector::ConstructXDivision", "WorldNotInit",
                    FatalException, "Voxelizing without having a pointer to world logical volume!");

    if(!fDetector)
        G4Exception("VoxelizedSensitiveDetector::ConstructXDivision", "DetConstInit",
                    FatalException, "Voxelizing without having a pointer to DetectorConstruction!");

    VoxelizedDetectorXDivision = new G4Box("VoxelizedDetectorXDivision",
                                           fVoxelWidthX/2,
                                           fDetector->GetSizeY()/2,
                                           fDetector->GetSizeZ()/2);

    VoxelizedDetectorXDivisionLog = new G4LogicalVolume(VoxelizedDetectorXDivision,
                                                        worldLogical->GetMaterial(),
                                                        "VoxelizedDetectorXDivisionLog",
                                                        0,0,0);

    VoxelizedDetectorXDivisionPhys = new G4PVReplica("VoxelizedDetectorXDivisionPhys",
                                                     VoxelizedDetectorXDivisionLog,
                                                     worldLogical,
                                                     kXAxis,
                                                     fVoxelNumberAlongX,
                                                     fVoxelWidthX);
}

void RadiobiologyVoxelizedSensitiveDetector::ConstructYDivision()
{
    VoxelizedDetectorYDivision = new G4Box("VoxelizedDetectorYDivision",
                                           fVoxelWidthX/2,
                                           fVoxelWidthY/2,
                                           fDetector->GetSizeZ()/2);

    VoxelizedDetectorYDivisionLog = new G4LogicalVolume(VoxelizedDetectorYDivision,
                                                        worldLogical->GetMaterial(),
                                                        "VoxelizedDetectorYDivisionLog",
                                                        0,0,0);

    VoxelizedDetectorYDivisionPhys = new G4PVReplica("VoxelizedDetectorYDivisionPhys",
                                                     VoxelizedDetectorYDivisionLog,
                                                     VoxelizedDetectorXDivisionLog,
                                                     kYAxis,
                                                     fVoxelNumberAlongY,
                                                     fVoxelWidthY);
}

void RadiobiologyVoxelizedSensitiveDetector::ConstructZDivision()
{
    VoxelizedDetectorZDivision = new G4Box("VoxelizedDetectorZDivision",
                                           fVoxelWidthX/2,
                                           fVoxelWidthY/2,
                                           fVoxelWidthZ/2);

    VoxelizedDetectorZDivisionLog = new G4LogicalVolume(VoxelizedDetectorZDivision,
                                                        worldLogical->GetMaterial(),
                                                        "VoxelizedDetectorZDivisionLog",
                                                        0,0,0);

    VoxelizedDetectorZDivisionPhys = new G4PVReplica("VoxelizedDetectorZDivisionPhys",
                                                     VoxelizedDetectorZDivisionLog,
                                                     VoxelizedDetectorYDivisionPhys,
                                                     kZAxis,
                                                     fVoxelNumberAlongZ,
                                                     fVoxelWidthZ);

    sensitiveLogicalVolume = VoxelizedDetectorZDivisionLog;
}

// First voxelize along X, then Y, then Z
G4bool RadiobiologyVoxelizedSensitiveDetector::ConstructVoxelizedDetector()
{

    // Creating X division
    ConstructXDivision();

    // Creating Y division
    ConstructYDivision();

    // Creating Z division
    ConstructZDivision();

    // Set last, smallest volumes as sensitive
    sensitiveLogicalVolume = VoxelizedDetectorZDivisionLog;
    isBuilt = true;

    return true;
}

void RadiobiologyVoxelizedSensitiveDetector::UpdateVoxelizedGeometry()
{
  // Nothing happens if the voxelized geometry is not built. But parameters are properly set.
  if (!isBuilt)
    {
      return;
    }

  CalculateVoxelNumber();

  // Volume that will be deleted in order to update
  G4VPhysicalVolume* myVol;

  G4PhysicalVolumeStore* store =
    G4PhysicalVolumeStore::GetInstance();

  myVol = store->GetVolume("VoxelizedDetectorXDivisionPhys");
  store->DeRegister(myVol);
  myVol = store->GetVolume("VoxelizedDetectorYDivisionPhys");
  store->DeRegister(myVol);
  myVol = store->GetVolume("VoxelizedDetectorZDivisionPhys");
  store->DeRegister(myVol);
  VoxelizedDetectorXDivisionPhys = new G4PVReplica("VoxelizedDetectorXDivisionPhys",
                                                   VoxelizedDetectorXDivisionLog,
                                                   worldLogical,
                                                   kXAxis,
                                                   fVoxelNumberAlongX,
                                                   fVoxelWidthX);


  VoxelizedDetectorYDivisionPhys = new G4PVReplica("VoxelizedDetectorYDivisionPhys",
                                                   VoxelizedDetectorYDivisionLog,
                                                   worldLogical,
                                                   kYAxis,
                                                   fVoxelNumberAlongY,
                                                   fVoxelWidthY);


  VoxelizedDetectorZDivisionPhys = new G4PVReplica("VoxelizedDetectorZDivisionPhys",
                                                   VoxelizedDetectorZDivisionLog,
                                                   worldLogical,
                                                   kZAxis,
                                                   fVoxelNumberAlongZ,
                                                   fVoxelWidthZ);

  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
  G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
}


void RadiobiologyVoxelizedSensitiveDetector::ConstructSD()
{
 G4String sensitiveDetectorName = "VoxelizedDetector";
 G4String HCname = "LETdata";

 RadioBioSD* detectorSD = new RadioBioSD(sensitiveDetectorName, HCname);
 G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
 sensitiveLogicalVolume->SetSensitiveDetector(detectorSD);
}

void RadiobiologyVoxelizedSensitiveDetector::Construct()
{
 ConstructVoxelizedDetector();
// ConstructSD();
}

void RadiobiologyVoxelizedSensitiveDetector::InitializeWorldPtr(G4VPhysicalVolume* pWorld)
{
    if(pWorld == nullptr)
        G4Exception("VoxelizedSensitiveDetector::InitializeWorldPtr", "WorldinitNull",
                    FatalException, "Initializing Voxelization Class with a Null Pointer to World!");
    worldLogical = pWorld->GetLogicalVolume();
}

// Returns absolute voxel index given matrix indexes
G4int RadiobiologyVoxelizedSensitiveDetector::GetThisVoxelNumber(G4int x, G4int y, G4int z)
const
{
    G4int nz = GetVoxelNumberAlongZ();
    G4int ny = GetVoxelNumberAlongY();

    return z + nz * (y + ny * (x));
}

