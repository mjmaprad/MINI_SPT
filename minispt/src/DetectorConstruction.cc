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
/// \file optical/OpNovice2/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

#include <iostream>
#include <sstream>
using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fDetectorMessenger(nullptr)
{

  G4cout << "Inizializing DetectorConstruction... " << G4endl;

  fExpHall_x = fExpHall_y = fExpHall_z = 50.*cm;
  ///////////////////////////////////////////
     Al_x = 5 * cm;
   Al_y = 5 * cm;
   Al_z = 5 * mm;
  /////
     fScintU_x = 2 * cm;
   fScintU_y = 2 * cm;
   fScintU_z = 1 * mm;
  //

   fScintL_x = 2 * cm;
   fScintL_y = 2 * cm;
   fScintL_z = 1 * mm;
  ///
   SPD1_x = 8 * mm;
   SPD1_y = 8 * mm;
   SPD1_z = 0.285 * mm;
   SPD1_pcb_z = 0.3 * mm;
     SPD2_x = 8 * mm;
   SPD2_y = 8 * mm;
   SPD2_z = 0.285 * mm;
   SPD2_pcb_z =SPD1_pcb_z;
     SPD3_x = 8 * mm;
   SPD3_y = 8 * mm;
   SPD3_z = 0.285 * mm;
   SPD3_pcb_z =SPD1_pcb_z;
     SPD4_x = 8 * mm;
   SPD4_y = 8 * mm;
   SPD4_z = 0.285 * mm;
   SPD4_pcb_z =SPD1_pcb_z;
  ////////////////////////////////////////////
  fScint_x = fScint_y = fScint_z = 26.0/2*mm;
  fAl_zpos = 9 * cm;
  fscint1_zpos = 8 * cm;
  fSPD1_zpos = 7 * cm;
  fSPD2_zpos = 6 * cm;
  fSPD3_zpos = 5 * cm;
  fSPD4_zpos = 4 * cm;
  fscint2_zpos = 3 * cm;



  numBottomSIPM = 64;
  numSideSIPM = 0;
  
  fScint = nullptr;

  fScintMPT    = new G4MaterialPropertiesTable();
  fWorldMPT   = new G4MaterialPropertiesTable();
  fSurfaceMPT = new G4MaterialPropertiesTable();

  fSurface = new G4OpticalSurface("Surface");
  fSurface->SetType(dielectric_dielectric);
  //fSurface->SetFinish(groundfrontpainted); //groundbackpainted
  //fSurface->SetFinish(polishedteflonair);
  fSurface->SetFinish(ground);
  fSurface->SetSigmaAlpha(0.5); // only for ground type finish
  fSurface->SetModel(unified);

  //G4double pp[2] = {2.038*eV, 4.144*eV};
  //G4double reflectivity[2] = {0.003, 0.005};
  //fSurfaceMPT->AddProperty("REFLECTIVITY",pp,reflectivity,2);
  fSurface->SetMaterialPropertiesTable(fSurfaceMPT);

  fScint_LV  = nullptr;
  fWorld_LV = nullptr;

  //G4cout << "DEBUG: Ok here... " << G4endl;
  fScintMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  //DetectorConstruction::BuildGAGGMaterial();

  //fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
 fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  G4cout << "Start detector construction" << G4endl;

  G4Material *fAl = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  G4Material *fscint1mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  G4Material *fscint2mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  G4Material *fsi = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  G4Material *fcppr = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
   G4Material *fplexyglass = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS");

  //   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // ---------------------- Volumes ------------------------
  // The experimental Hall
  G4Box *world_box = new G4Box("Worlds", fExpHall_x / 2, fExpHall_y / 2, fExpHall_z / 2);

  fWorld_LV = new G4LogicalVolume(world_box, fWorldMaterial, "fWorld_LV", 0, 0, 0);
  G4cout << "the world is " << fWorldMaterial->GetName() << G4endl;

  G4VPhysicalVolume *world_PV = new G4PVPlacement(0, G4ThreeVector(), fWorld_LV, "world_PV", 0, false, 0, true);
  ////////////////////////////////cage/////////////////////////////////////
  // G4Box *cage_box = new G4Box("cage_box", fcage_x / 2, fcage_y / 2, fcage_z / 2);
  // G4Box *cage_box2 = new G4Box("cage_box2", fcage_x / 2 + 0.1 * mm, fcage_y / 2 + 0.1 * mm, fcage_z / 2 + 0.1 * mm);
  // G4SubtractionSolid *cage_boxsub = new G4SubtractionSolid("cage_boxsub",
  //                                                          cage_box2, cage_box, 0,
  //                                                          G4ThreeVector(
  //                                                              0,
  //                                                              0, 0));
  // G4LogicalVolume *cage_LV = new G4LogicalVolume(cage_boxsub, fWorldMaterial, "cage_LV", 0, 0, 0);

  // // G4VPhysicalVolume *cage_PV =
  // new G4PVPlacement(0, G4ThreeVector(0, 0, fcage_zpos), cage_LV, "cage_PV", fWorld_LV, false, 0, true);
  //////////////////////////////////AL////////////////////////////////////////////

  G4Box *Al_box = new G4Box("Al_box", Al_x / 2, Al_y / 2, Al_z / 2);
  G4LogicalVolume *Al_LV = new G4LogicalVolume(Al_box, fAl, "Al_LV", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0, 0, fAl_zpos), Al_LV, "Al_PV", fWorld_LV, false, 0, true);
  //////////////////////////////////scint1////////////////////////////////////////////
G4cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<G4endl;
G4cout<<" Component"<<" Xmm "<<" Ymm "<<" zmm "<<"zPoscm"<<G4endl;
G4cout<<"MLI "<<Al_x/mm<<" "<<Al_y/mm<<" "<<Al_z/mm<<" "<<fAl_zpos/cm<<G4endl;
G4cout<<"Scint1 "<<fScintU_x/mm<<" "<<fScintU_y/mm<<" "<<fScintU_z/mm<<" "<<fscint1_zpos/cm<<G4endl;
G4cout<<"SPD1 "<<SPD1_x/mm<<" "<<SPD1_y/mm<<" "<<SPD1_z/mm<<" "<<fSPD1_zpos/cm<<G4endl;
G4cout<<"SPD2 "<<SPD2_x/mm<<" "<<SPD2_y/mm<<" "<<SPD2_z/mm<<" "<<fSPD2_zpos/cm<<G4endl;
G4cout<<"SPD3 "<<SPD3_x/mm<<" "<<SPD3_y/mm<<" "<<SPD3_z/mm<<" "<<fSPD3_zpos/cm<<G4endl;
G4cout<<"SPD4 "<<SPD4_x/mm<<" "<<SPD4_y/mm<<" "<<SPD4_z/mm<<" "<<fSPD4_zpos/cm<<G4endl;
G4cout<<"Scint2 "<<fScintL_x/mm<<" "<<fScintL_y/mm<<" "<<fScintL_z/mm<<" "<<fscint2_zpos/cm<<G4endl;

G4cout<<"ROC "<< "like SPD"<< "under each SPD thickness="<<SPD1_pcb_z/mm<<G4endl;


  G4Box *scintU_box = new G4Box("scintU_box", fScintU_x / 2, fScintU_y / 2, fScintU_z / 2);

  //	G4Tubs* scint1_cyl = new G4Tubs("scint1_cyl", 0, fScint1_d/2,
  //				fScint1_h / 2, 0, 360*deg);
  G4LogicalVolume *scint1_LV = new G4LogicalVolume(scintU_box, fscint1mat, "scint1_LV", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0, 0, fscint1_zpos), scint1_LV, "scint1_PV", fWorld_LV, false, 0, true);
  //////////////////////////////////scint2////////////////////////////////////////////


  G4Box *scintL_box = new G4Box("scintL_box", fScintL_x / 2, fScintL_y / 2, fScintL_z / 2);

  //	G4Tubs* scint1_cyl = new G4Tubs("scint1_cyl", 0, fScint1_d/2,
  //				fScint1_h / 2, 0, 360*deg);
  G4LogicalVolume *scint2_LV = new G4LogicalVolume(scintL_box, fscint1mat, "scint2_LV", 0, 0, 0);
  new G4PVPlacement(0, G4ThreeVector(0, 0, fscint2_zpos), scint2_LV, "scint2_PV", fWorld_LV, false, 0, true);
  //////////////////////////////////AL////////////////////////////////////////////
  
  //G4double SPD_cppr_z = 0.1 * mm;
  G4Box *SPD1_box = new G4Box("SPD1_box", SPD1_x / 2, SPD1_y / 2, SPD1_z / 2);
    G4Box *SPD2_box = new G4Box("SPD2_box", SPD2_x / 2, SPD2_y / 2, SPD2_z / 2);
  G4Box *SPD3_box = new G4Box("SPD3_box", SPD3_x / 2, SPD3_y / 2, SPD3_z / 2);
  G4Box *SPD4_box = new G4Box("SPD4_box", SPD4_x / 2, SPD4_y / 2, SPD4_z / 2);

  G4LogicalVolume *SPD1_LV = new G4LogicalVolume(SPD1_box, fsi, "SPD1_LV", 0, 0, 0);
  G4LogicalVolume *SPD2_LV = new G4LogicalVolume(SPD2_box, fsi, "SPD2_LV", 0, 0, 0);
  G4LogicalVolume *SPD3_LV = new G4LogicalVolume(SPD3_box, fsi, "SPD3_LV", 0, 0, 0);
  G4LogicalVolume *SPD4_LV = new G4LogicalVolume(SPD4_box, fsi, "SPD4_LV", 0, 0, 0);

  G4Box *SPD1_pcb_box = new G4Box("SPD1_pcb_box ", SPD1_x / 2, SPD1_y / 2, SPD1_pcb_z / 2);
  G4Box *SPD2_pcb_box = new G4Box("SPD2_pcb_box ", SPD2_x / 2, SPD2_y / 2, SPD2_pcb_z / 2);
    G4Box *SPD3_pcb_box = new G4Box("SPD3_pcb_box ", SPD3_x / 2, SPD3_y / 2, SPD3_pcb_z / 2);
  G4Box *SPD4_pcb_box = new G4Box("SPD4_pcb_box ", SPD4_x / 2, SPD4_y / 2, SPD4_pcb_z / 2);
  G4LogicalVolume *SPD1_pcb_LV = new G4LogicalVolume(SPD1_pcb_box, fcppr, "SPD1_pcb_LV", 0, 0, 0);
  G4LogicalVolume *SPD2_pcb_LV = new G4LogicalVolume(SPD2_pcb_box, fcppr, "SPD2_pcb_LV", 0, 0, 0);
  G4LogicalVolume *SPD3_pcb_LV = new G4LogicalVolume(SPD3_pcb_box, fcppr, "SPD3_pcb_LV", 0, 0, 0);
  G4LogicalVolume *SPD4_pcb_LV = new G4LogicalVolume(SPD4_pcb_box, fcppr, "SPD4_pcb_LV", 0, 0, 0);

  //G4Box *SPD_cppr_box = new G4Box("SPD_cppr_box ", SPD_x / 2, SPD_y / 2, SPD_cppr_z / 2);
  //G4LogicalVolume *SPD_cppr_LV = new G4LogicalVolume(SPD_cppr_box, fcppr, "SPD_cppr_LV", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD1_zpos), SPD1_LV, "SPD1_PV", fWorld_LV, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD1_zpos - SPD1_z / 2 - SPD1_pcb_z / 2), SPD1_pcb_LV, "SPD1_pcb_PV", fWorld_LV, false, 0, true);
  //new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD1_zpos - SPD_z / 2 - SPD_pcb_z - SPD_cppr_z / 2), SPD_cppr_LV, "SPD1_cppr_PV", fWorld_LV, false, 0, true);

  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD2_zpos), SPD2_LV, "SPD2_PV", fWorld_LV, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD2_zpos - SPD2_z / 2 - SPD2_pcb_z / 2), SPD2_pcb_LV, "SPD2_pcb_PV", fWorld_LV, false, 0, true);
  //new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD2_zpos - SPD_z / 2 - SPD_pcb_z - SPD_cppr_z / 2), SPD_cppr_LV, "SPD2_cppr_PV", fWorld_LV, false, 0, true);

  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD3_zpos), SPD3_LV, "SPD3_PV", fWorld_LV, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD3_zpos - SPD3_z / 2 - SPD3_pcb_z / 2), SPD3_pcb_LV, "SPD3_pcb_PV", fWorld_LV, false, 0, true);
  //new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD3_zpos - SPD_z / 2 - SPD_pcb_z - SPD_cppr_z / 2), SPD_cppr_LV, "SPD3_cppr_PV", fWorld_LV, false, 0, true);

  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD4_zpos), SPD4_LV, "SPD4_PV", fWorld_LV, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD4_zpos - SPD4_z / 2 - SPD4_pcb_z / 2), SPD4_pcb_LV, "SPD4_pcb_PV", fWorld_LV, false, 0, true);
  //new G4PVPlacement(0, G4ThreeVector(0, 0, fSPD4_zpos - SPD_z / 2 - SPD_pcb_z - SPD_cppr_z / 2), SPD_cppr_LV, "SPD4_cppr_PV", fWorld_LV, false, 0, true);

  //////////////////////////////////scint1////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //--------------------------------------------------
  //  GAGG
  //--------------------------------------------------

  G4Element* eGa = new G4Element("Gallium", "Ga", 31, 69.723*g/mole);
  G4Element* eAl = new G4Element("Aluminum", "Al", 13, 26.981*g/mole);
  G4Element* eGd = new G4Element("Gadolinum", "Gd", 64, 157.25*g/mole);
  G4Element* eO  = new G4Element("Oxygen", "O", 8, 16*g/mole);
  G4Element* eCe = new G4Element("Cerium", "Ce", 58, 140.116*g/mole);

  G4double GAGGdensity = 6.63*g/cm3;
  //https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjjmNPGqYryAhWJ-6QKHfvdD_EQFjAOegQIFhAD&url=https%3A%2F%2Fmoem.pensoft.net%2Farticle%2F33240%2Fdownload%2Fpdf%2F290098&usg=AOvVaw3H1ZUR6pq9B-3YXaafYlGN
  //﻿Optical characteritics of single crystal Gd3Al2Ga3O12 : Ce
  //Energy Resolution and Temperature Dependence of Ce:GAGG Coupled to 3mm × 3mm Silicon Photomultipliers, http://eprints.gla.ac.uk/116774/7/116774.pdf
  G4Material *fGAGG= new G4Material("GAGG", GAGGdensity, 5);
  fGAGG->AddElement(eGd,0.509);
  fGAGG->AddElement(eAl,0.058);
  fGAGG->AddElement(eGa, 0.226);
  fGAGG->AddElement(eO,0.207);
  fGAGG->AddElement(eCe,0.0006358);
  /////https://www.advatech-uk.co.uk/GAGG_Ce%20Spectrum.png

  fScintMaterial = fGAGG;

  G4double EnergyGAGG[] =
    {2.65947043332535*eV,
     2.60032958801498*eV,
     2.55252941176471*eV,
     2.52640618603593*eV,
     2.50363038088799*eV,
     2.48126155907974*eV,
     2.44844787304386*eV,
     2.42440157136622*eV,
     2.3827987987988*eV,
     2.33521294933782*eV,
     2.29185227976068*eV,
     2.23423330651649*eV,
     2.17944045516971*eV,
     2.1252358905682*eV,
     2.06211397809541*eV,
     1.99902969228001*eV,
     1.9295827688032*eV,
     1.85390654205607*eV,
     1.80129852440409*eV,
     1.7613140954495*eV
    };

  const G4int nEntriesGAGG = sizeof(EnergyGAGG)/sizeof(G4double);

  ////Effect of Ca 2+ and Zr 4+ co-doping on the optical properties of Gd 3Al 2Ga 3O 12: Ce single crystals
  G4double refractiveIndexGAGG[] =
    {1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9
    };

  assert(sizeof(refractiveIndexGAGG) == sizeof(EnergyGAGG));

  ////Effect of Ca 2+ and Zr 4+ co-doping on the optical properties of Gd 3Al 2Ga 3O 12: Ce single crystals

  G4double absGAGG[] =
    {64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm	
    };

  assert(sizeof(absGAGG) == sizeof(EnergyGAGG));

  ///https://www.advatech-uk.co.uk/GAGG_Ce%20Spectrum.png
  G4double scintillationFastGAGG[] =
    {0.000138612725604,
     0.103013153869685,
     0.259406546344448,
     0.475723654022637,
     0.640644118996635,
     0.803423256347507,
     0.938384253594371,
     0.987677806668706,
     1.00488490364026,
     0.985703770266136,
     0.917267321810951,
     0.786765830529214,
     0.63056840776996,
     0.482941075252371,
     0.335342421229734,
     0.243427844906699,
     0.183661861425512,
     0.128211991434689,
     0.100542023554604,
     0.079262580299786
    }; 

  assert(sizeof(scintillationFastGAGG) == sizeof(EnergyGAGG));
  /// In GAGG the emission wavelength peak is 520 nm

  // Add entries into properties table
  G4MaterialPropertiesTable* mptGAGG = new G4MaterialPropertiesTable();
  mptGAGG->AddProperty("RINDEX",EnergyGAGG,refractiveIndexGAGG,nEntriesGAGG);
  mptGAGG->AddProperty("ABSLENGTH",EnergyGAGG,absGAGG,nEntriesGAGG);
  mptGAGG->AddProperty("FASTCOMPONENT",EnergyGAGG, scintillationFastGAGG,nEntriesGAGG);
  //mptGAGG->AddProperty("SLOWCOMPONENT",EnergyGAGG, scintillationSlowGAGG,nEntriesGAGG);
  mptGAGG->AddConstProperty("FASTTIMECONSTANT", 90*ns);///fast component of the order of 60-130ns
  mptGAGG->AddConstProperty("SLOWTIMECONSTANT", 260*ns);//a slow component of the order of several hundred nanoseconds (260-530ns).
  mptGAGG->AddConstProperty("SCINTILLATIONYIELD",50000./MeV);
  mptGAGG->AddConstProperty("YIELDRATIO",1.0);
  mptGAGG->AddConstProperty("RESOLUTIONSCALE",5.2);//http://eprints.gla.ac.uk/116774/7/116774.pdf

  fScintMPT = mptGAGG;//*/

  fScintMaterial->SetMaterialPropertiesTable(fScintMPT);
  fScintMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  fWorldMaterial->SetMaterialPropertiesTable(fWorldMPT);

  G4double density;

  //G4int ncomponents;
  //G4double fractionmass;
  //std::vector<G4int> natoms;
  //std::vector<G4double> fractionMass;
  //std::vector<G4String> elements;

  //Optical Grease
  // Silicone (Template for Optical Grease)

  /*elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(6);
  density = 1.060*g/cm3;
  G4Material *fSilicone = G4NistManager::Instance()->ConstructNewMaterial("Silicone", elements, natoms, density);
  elements.clear();
  natoms.clear();//*/

  G4Element* H = new G4Element("H", "H", 1., 1.01*g/mole);
  G4Element* C = new G4Element("C", "C", 6., 12.01*g/mole);
  G4Material* fGreaseMaterial= new G4Material("Silicone", density= 1.032*g/cm3, 2);
  fGreaseMaterial->AddElement(C,9);
  fGreaseMaterial->AddElement(H,10);


  // Mylar/Teflon Foil

  /*density = 1.39*g/cm3;
  elements.push_back("O");        natoms.push_back(2);
  elements.push_back("C");        natoms.push_back(5);
  elements.push_back("H");        natoms.push_back(4);
  G4Material *fFoilM = G4NistManager::Instance()->ConstructNewMaterial("Foil", elements, natoms, density);
  elements.clear();
  natoms.clear();//*/

  //G4Material *fFoilM = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
  G4Material *fFoilM = G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
  fFoilM->SetMaterialPropertiesTable(fSurfaceMPT);


  //Grease Properties

  G4double photonEnergy[] =
  //{ 3.44*eV, 3.26*eV, 3.1*eV, 3.02*eV, 2.95*eV,2.92*eV, 2.82*eV, 2.76*eV, 2.7*eV, 2.58*eV,2.38*eV, 2.08*eV };
    {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
     2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
     2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
     2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
     3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

   G4double refractiveIndexSilicone[] =
    { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
      1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
      1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
      1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
      1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46};

  G4double absGrease[] =
    { 20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
      20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
      20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
      20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
      20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m};

  //assert(sizeof(absGrease) == sizeof(photonEnergy));

  //assert(sizeof(refractiveIndexSilicone) == sizeof(photonEnergy));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptGrease = new G4MaterialPropertiesTable();
  mptGrease->AddProperty("RINDEX",photonEnergy,refractiveIndexSilicone,nEntries);
  mptGrease->AddProperty("ABSLENGTH",photonEnergy,absGrease,nEntries);
  fGreaseMaterial->SetMaterialPropertiesTable(mptGrease);


  // ------------- Volumes --------------
  // // The experimental Hall
  // G4Box* world_box = new G4Box("World", fExpHall_x, fExpHall_y, fExpHall_z);

  // fWorld_LV = new G4LogicalVolume(world_box, fWorldMaterial, "World", 0, 0, 0);

  // G4VPhysicalVolume* world_PV = new G4PVPlacement(0, G4ThreeVector(), fWorld_LV, "World", 0, false, 0);


  //The foil
  G4double foil_thickness= 0.015*mm;
  G4Box* foil_box = new G4Box("foil", fScint_x+foil_thickness, fScint_y+foil_thickness, fScint_z+foil_thickness);

  G4LogicalVolume *fFoil_LV = new G4LogicalVolume(foil_box, fFoilM, "foil", 0, 0, 0);

  G4VPhysicalVolume *fFoil = new G4PVPlacement(0, G4ThreeVector(), fFoil_LV, "foil",  fWorld_LV, false, 0);


  // The Scint
  G4Box* Scint_box = new G4Box("Scint", fScint_x, fScint_y, fScint_z);

  fScint_LV = new G4LogicalVolume(Scint_box, fScintMaterial, "Scint", 0, 0, 0);

  fScint = new G4PVPlacement(0, G4ThreeVector(), fScint_LV, "Scint",  fFoil_LV, false, 0);


  //Scint-Foil Surface
  G4LogicalBorderSurface* surface = new G4LogicalBorderSurface("Surface", fScint, fFoil, fSurface);


  G4double sipm_length=3/2.0*mm;
  G4double sipm_thickness=0.125/2.0*mm;
  G4double sipm_distance=0.2*mm;
  G4double grease_thickness=2.5/2.0*um;

  //Optical grease
  G4Box *grease_box = new G4Box("grease_box", sipm_length, sipm_length, grease_thickness );
  //Sipm
  G4Box *sipm_box = new G4Box("sipm_box", sipm_length, sipm_length, sipm_thickness );

  G4LogicalVolume *greaseLV = new G4LogicalVolume(grease_box, fGreaseMaterial, "Grease", 0, 0, 0);

  G4LogicalVolume *sipmLV = new G4LogicalVolume(sipm_box, G4NistManager::Instance()->FindOrBuildMaterial("G4_Si"), "sipm", 0, 0, 0);

  G4RotationMatrix* xRot = new G4RotationMatrix;
  xRot->rotateX(90.*deg);
  //xRot->rotateX(0.*deg);

  string sipm_name;
  string grease_name;
  G4VPhysicalVolume *fsipm[numBottomSIPM+numSideSIPM];
  G4VPhysicalVolume *fgrease[numBottomSIPM+numSideSIPM];
  
  //Scint-Grease Surface
  G4OpticalSurface* fSurface_scint_grease = new G4OpticalSurface("Surface_scint_grease");
  fSurface_scint_grease->SetType(dielectric_dielectric);
  fSurface_scint_grease->SetModel(unified);
  fSurface_scint_grease->SetFinish(ground);
  fSurface_scint_grease->SetSigmaAlpha(0.1);
  fSurface_scint_grease->SetMaterialPropertiesTable(mptGrease);

  G4LogicalBorderSurface* surfaceGrease[numBottomSIPM+numSideSIPM];

  //Grease-SiPM Surface
  G4OpticalSurface *sipmSurface = new G4OpticalSurface("sipmSurface");
  sipmSurface->SetType(dielectric_dielectric);
  sipmSurface->SetFinish(ground);
  sipmSurface->SetModel(unified);
  sipmSurface->SetSigmaAlpha(0.1);
  sipmSurface->SetMaterialPropertiesTable(fSurfaceMPT);

  G4LogicalBorderSurface* surfaceSiPM[numBottomSIPM+numSideSIPM];
  
  G4double sipm_start = -0.5 * ( 2*sipm_length + sipm_distance ) * ( numBottomSIPM_nxn - 1 );
  //G4cout<<"bottom sipm_start = "<<sipm_start<<G4endl;
  
  for (int i_sipm = 0; i_sipm < numBottomSIPM; i_sipm++) {

    sipm_name="sipmPV"+to_string(i_sipm);
    grease_name="greasePV"+to_string(i_sipm);

    fsipm[i_sipm] = new G4PVPlacement(0,
				      //G4ThreeVector((-3.5 + i_sipm%8)*cm, 0.0 * mm - grease_thickness * 2 - sipm_thickness - fScint_y, (-3.5 + i_sipm/8)*cm),
				      G4ThreeVector(
						    (sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm%numBottomSIPM_nxn))*mm,
						    (sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm/numBottomSIPM_nxn))*mm,
                						    0.0 * mm - grease_thickness*2 - sipm_thickness - fScint_y
						    ),
				      sipmLV,
				      sipm_name,
				      fFoil_LV,
				      false,
				      i_sipm
				      );
    
    fgrease[i_sipm] = new G4PVPlacement(0,
					//G4ThreeVector((-3.5 + i_sipm%8)*cm, 0.0*mm-grease_thickness-fScint_y, (-3.5 + i_sipm/8)*cm),
					G4ThreeVector((sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm%numBottomSIPM_nxn))*mm,
						      (sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm/numBottomSIPM_nxn))*mm,
                  						      0.0*mm-grease_thickness-fScint_y
						      ),
					greaseLV,
					grease_name,
					fFoil_LV,
					false,
					i_sipm
					);

    surfaceGrease[i_sipm] = new G4LogicalBorderSurface("SurfaceGrease"+to_string(i_sipm), fScint, fgrease[i_sipm], fSurface_scint_grease);
    surfaceSiPM[i_sipm] = new G4LogicalBorderSurface("SurfaceSiPM"+to_string(i_sipm), fgrease[i_sipm], fsipm[i_sipm], sipmSurface);

  }

  G4RotationMatrix* yRot = new G4RotationMatrix;
  yRot->rotateY(90.*deg);

  sipm_start = -0.5 * ( 2*sipm_length + sipm_distance ) * ( numSideSIPM_nxn - 1 );
  //G4cout<<"side sipm_start = "<<sipm_start<<G4endl;
  //getchar();
  
  for (int i_sipm = 0; i_sipm < numSideSIPM; i_sipm++) {

    sipm_name="sipmPV" + to_string(i_sipm+numBottomSIPM);
    grease_name="greasePV" + to_string(i_sipm+numBottomSIPM);

    //G4cout<<sipm_name<<G4endl;

    fsipm[i_sipm+numBottomSIPM] = new G4PVPlacement(yRot,
						    //G4ThreeVector(0.0*mm-grease_thickness*2-sipm_thickness-fScint_x, (-3.5 + i_sipm%8)*cm, (-3.5 + i_sipm/8)*cm),
						    G4ThreeVector(0.0*mm-grease_thickness*2-sipm_thickness-fScint_x,
								  (sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm%numBottomSIPM_nxn))*mm,
								  (sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm/numBottomSIPM_nxn))*mm
								  ),
						    sipmLV,
						    sipm_name,
						    fFoil_LV,
						    false,
						    i_sipm
						    );

    fgrease[i_sipm+numBottomSIPM] = new G4PVPlacement(yRot,
						      //G4ThreeVector(0.0*mm-grease_thickness-fScint_x, (-3.5 + i_sipm%8)*cm, (-3.5 + i_sipm/8)*cm),
						      G4ThreeVector(0.0*mm-grease_thickness-fScint_x,
								    (sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm%numBottomSIPM_nxn))*mm,
								    (sipm_start + ( 2*sipm_length + sipm_distance )*(i_sipm/numBottomSIPM_nxn))*mm
								    ),
						      greaseLV,
						      grease_name,
						      fFoil_LV,
						      false,
						      i_sipm
						      );

    surfaceGrease[i_sipm+numBottomSIPM] = new G4LogicalBorderSurface("SurfaceGrease"+to_string(i_sipm+numBottomSIPM), fScint, fgrease[i_sipm+numBottomSIPM], fSurface_scint_grease);
    surfaceSiPM[i_sipm+numBottomSIPM] = new G4LogicalBorderSurface("SurfaceSiPM"+to_string(i_sipm+numBottomSIPM), fgrease[i_sipm+numBottomSIPM], fsipm[i_sipm+numBottomSIPM], sipmSurface);

  }//*/


  // ------------- Surface Dump Info --------------

  G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>(surface->GetSurface(fScint, fFoil)->GetSurfaceProperty());
  G4cout << "******  opticalSurface->DumpInfo:" << G4endl;
  if (opticalSurface) { opticalSurface->DumpInfo(); }
  G4cout << "******  end of opticalSurface->DumpInfo" << G4endl;

  /*G4OpticalSurface* opticalSurface2 = dynamic_cast <G4OpticalSurface*>(surfaceGrease->GetSurface(fScint, fgrease[0])->GetSurfaceProperty());
  G4cout << "******  opticalSurface2->DumpInfo:" << G4endl;
  if (opticalSurface2) { opticalSurface2->DumpInfo(); }
  G4cout << "******  end of opticalSurface2->DumpInfo" << G4endl;//*/

  /*G4OpticalSurface* opticalSurface3 = dynamic_cast <G4OpticalSurface*>(surfaceSiPM->GetSurface(fgrease[0], fsipm[0])->GetSurfaceProperty());
  G4cout << "******  opticalSurface3->DumpInfo:" << G4endl;
  if (opticalSurface3) { opticalSurface3->DumpInfo(); }
  G4cout << "******  end of opticalSurface3->DumpInfo" << G4endl;//*/

  return world_PV;
}

//DetectorMessenger stuff


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetSurfaceSigmaAlpha(G4double v) {
  fSurface->SetSigmaAlpha(v);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << "Surface sigma alpha set to: " << fSurface->GetSigmaAlpha()
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetSurfacePolish(G4double v) {
  fSurface->SetPolish(v);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << "Surface polish set to: " << fSurface->GetPolish()
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddScintMPV(const char* c,
                                     G4MaterialPropertyVector* mpv) {
  fScintMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the box is now: " << G4endl;
  fScintMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddWorldMPV(const char* c,
                                       G4MaterialPropertyVector* mpv) {
  fWorldMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the world is now: " << G4endl;
  fWorldMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddSurfaceMPV(const char* c,
                                         G4MaterialPropertyVector* mpv) {
  fSurfaceMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the surface is now: " << G4endl;
  fSurfaceMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddScintMPC(const char* c, G4double v) {
  fScintMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the box is now: " << G4endl;
  fScintMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddWorldMPC(const char* c, G4double v) {
  fWorldMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the world is now: " << G4endl;
  fWorldMPT->DumpTable();
  G4cout << "............." << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddSurfaceMPC(const char* c, G4double v) {
  fSurfaceMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the surface is now: " << G4endl;
  fSurfaceMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetWorldMaterial(const G4String& mat) {
  G4Material* pmat = G4NistManager::Instance()->FindOrBuildMaterial(mat);
  if (pmat && fWorldMaterial != pmat) {
    fWorldMaterial = pmat;
    if (fWorld_LV) {
      fWorld_LV->SetMaterial(fWorldMaterial);
      fWorldMaterial->SetMaterialPropertiesTable(fWorldMPT);
    }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    G4cout << "World material set to " << fWorldMaterial->GetName()
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetScintMaterial(const G4String& mat) {
  if (mat=="BC408") DetectorConstruction::BuildBC408Material();
  else if (mat=="LYSO") DetectorConstruction::BuildLYSOMaterial();
  //else if (mat=="GAGG") DetectorConstruction::BuildGAGGMaterial();
  else {
    G4Material* pmat = G4NistManager::Instance()->FindOrBuildMaterial(mat);
    if (pmat && fScintMaterial != pmat) {
      fScintMaterial = pmat;
      if (fScint_LV) {
	fScint_LV->SetMaterial(fScintMaterial);
	fScintMaterial->SetMaterialPropertiesTable(fScintMPT);
	fScintMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
      }
      G4RunManager::GetRunManager()->PhysicsHasBeenModified();
      G4cout << "Scint material set to " << fScintMaterial->GetName()
	     << G4endl;
    } else {
      G4cout << "WARNING! Failed to set scint material to " << mat
	     << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::BuildBC408Material() {

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //--------------------------------------------------
  //  BC408
  //--------------------------------------------------

  G4Element* H = new G4Element("H", "H", 1., 1.01*g/mole);
  G4Element* C = new G4Element("C", "C", 6., 12.01*g/mole);

  G4double density = 1.032*g/cm3;
  G4Material *fBC408= new G4Material("BC408", density= 1.032*g/cm3, 2);
  fBC408->AddElement(C,9);
  fBC408->AddElement(H,10);

  fScintMaterial = fBC408;

  G4double EnergyBC408[] =
    {3.299*eV,
     3.201*eV,
     3.130*eV,
     3.089*eV,
     3.069*eV,
     3.056*eV,
     3.036*eV,
     3.023*eV,
     3.016*eV,
     3.004*eV,
     2.991*eV,
     2.972*eV,
     2.947*eV,
     2.923*eV,
     2.905*eV,
     2.887*eV,
     2.863*eV,
     2.852*eV,
     2.829*eV,
     2.818*eV,
     2.818*eV,
     2.795*eV,
     2.768*eV,
     2.736*eV,
     2.699*eV,
     2.658*eV,
     2.638*eV,
     2.599*eV,
     2.562*eV,
     2.511*eV,
     2.455*eV,
     2.421*eV,
     2.384*eV
    };

  const G4int nEntriesBC408 = sizeof(EnergyBC408)/sizeof(G4double);

  G4double refractiveIndexBC408[] =
    {1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58,
     1.58
    };

  assert(sizeof(refractiveIndexBC408) == sizeof(EnergyBC408));

  G4double absBC408[] =
    {210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm,
     210*cm
    };

  assert(sizeof(absBC408) == sizeof(EnergyBC408));

  G4double scintillationFastBC408[] =
    {0.057,
     0.097,
     0.163,
     0.260,
     0.326,
     0.396,
     0.449,
     0.520,
     0.564,
     0.648,
     0.714,
     0.793,
     0.877,
     0.943,
     0.978,
     0.996,
     0.965,
     0.930,
     0.833,
     0.753,
     0.705,
     0.599,
     0.537,
     0.476,
     0.388,
     0.282,
     0.242,
     0.181,
     0.141,
     0.088,
     0.048,
     0.035,
     0.022
    };

  assert(sizeof(scintillationFastBC408) == sizeof(EnergyBC408));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptBC408 = new G4MaterialPropertiesTable();
  mptBC408->AddProperty("RINDEX",EnergyBC408,refractiveIndexBC408,nEntriesBC408);
  mptBC408->AddProperty("ABSLENGTH",EnergyBC408,absBC408,nEntriesBC408);
  mptBC408->AddProperty("FASTCOMPONENT",EnergyBC408, scintillationFastBC408,nEntriesBC408);
  //mptBC408->AddProperty("SLOWCOMPONENT",EnergyBC408, scintillationSlowBC408,nEntriesBC408);
  mptBC408->AddConstProperty("FASTTIMECONSTANT", 0.9*ns);
  mptBC408->AddConstProperty("SLOWTIMECONSTANT", 2.1*ns);
  mptBC408->AddConstProperty("SCINTILLATIONYIELD",10240./MeV);
  mptBC408->AddConstProperty("YIELDRATIO",1.0);
  mptBC408->AddConstProperty("RESOLUTIONSCALE",1.0);

  fScintMPT = mptBC408;
  
  //fBC408->SetMaterialPropertiesTable(mptBC408);
  // Set the Birks Constant for the BC408 scintillator
  //fBC408->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (fScint_LV) {
    fScint_LV->SetMaterial(fScintMaterial);
    fScintMaterial->SetMaterialPropertiesTable(fScintMPT);
    fScintMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();

  G4cout << "Scint material set to " << fScintMaterial->GetName() << G4endl;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::BuildLYSOMaterial() {

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //--------------------------------------------------
  //  LYSO
  //--------------------------------------------------

  G4Element* Lu = new G4Element("Lutetium", "Lu", 71, 174.967*g/mole);
  G4Element* Y  = new G4Element("Yttrium", "Y", 39, 88.90585*g/mole);
  G4Element* Si = new G4Element("Silicon", "Si", 14, 28.0855*g/mole);
  G4Element* O  = new G4Element("Oxygen", "O", 8, 16*g/mole);
  G4Element* Ce = new G4Element("Cerium", "Ce", 58, 140.116*g/mole);
  
  
  G4double density = 7.1*g/cm3;
  G4Material *fLYSO= new G4Material("LYSO", density, 5);
  fLYSO->AddElement(Lu,0.7138386);
  fLYSO->AddElement(Y,0.0403024);
  fLYSO->AddElement(Si,0.0637218);
  fLYSO->AddElement(O,0.1815012);
  fLYSO->AddElement(Ce,0.0006358);

  fScintMaterial = fLYSO;


  G4double EnergyLYSO[] =
    {3.38*eV,
     3.18*eV,
     3.13*eV,
     3.06*eV,
     3.04*eV, 
     3*eV,
     2.96*eV,
     2.88*eV,
     2.78*eV,
     2.72*eV,
     2.65*eV,
     2.58*eV,
     2.48*eV,
     2.34*eV,
     2.18*eV,
     2.03*eV,
     1.96*eV};
  const G4int nEntriesLYSO = sizeof(EnergyLYSO)/sizeof(G4double);

  G4double refractiveIndexLYSO[] =
    {1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81,
     1.81};
  assert(sizeof(refractiveIndexLYSO) == sizeof(EnergyLYSO));

  G4double absLYSO[] =
    {43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm,
     43.1*cm};
  assert(sizeof(absLYSO) == sizeof(EnergyLYSO));

  G4double scintillationFastLYSO[] =
    {0.010621600905209,
     0.106712413767931,
     0.279103551483739,
     0.422374712559769,
     0.583662444793226,
     0.756097382925138,
     0.879256852940103,
     1,
     0.876417125962697,
     0.737219403584334,
     0.580078110742052,
     0.431872102784977,
     0.272299886848925,
     0.152878052341497,
     0.073562798846589,
     0.023338321714057,
     0.011607110267548};  
  assert(sizeof(scintillationFastLYSO) == sizeof(EnergyLYSO));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptLYSO = new G4MaterialPropertiesTable();
  mptLYSO->AddProperty("RINDEX",EnergyLYSO,refractiveIndexLYSO,nEntriesLYSO);
  mptLYSO->AddProperty("ABSLENGTH",EnergyLYSO,absLYSO,nEntriesLYSO);
  mptLYSO->AddProperty("FASTCOMPONENT",EnergyLYSO, scintillationFastLYSO,nEntriesLYSO);
  //mptLYSO->AddProperty("SLOWCOMPONENT",EnergyLYSO, scintillationSlowLYSO,nEntriesLYSO);
  mptLYSO->AddConstProperty("FASTTIMECONSTANT", 41*ns);
  //mptLYSO->AddConstProperty("SLOWTIMECONSTANT", 41*ns);//????????
  mptLYSO->AddConstProperty("SCINTILLATIONYIELD",33200./MeV);
  mptLYSO->AddConstProperty("YIELDRATIO",1.0);
  mptLYSO->AddConstProperty("RESOLUTIONSCALE",8.0);   // 4.41 // for LYSO RESOLUTIONSCALE is 8 
  // https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/lyso-material-data-sheet.pdf

  fScintMPT = mptLYSO;
  
  if (fScint_LV) {
    fScint_LV->SetMaterial(fScintMaterial);
    fScintMaterial->SetMaterialPropertiesTable(fScintMPT);
    fScintMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();//*/

  G4cout << "Scint material set to " << fScintMaterial->GetName() << G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::BuildGAGGMaterial() {

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //--------------------------------------------------
  //  GAGG
  //--------------------------------------------------

  G4Element* eGa = new G4Element("Gallium", "Ga", 31, 69.723*g/mole);
  G4Element* eAl = new G4Element("Aluminum", "Al", 13, 26.981*g/mole);
  G4Element* eGd = new G4Element("Gadolinum", "Gd", 64, 157.25*g/mole);
  G4Element* eO  = new G4Element("Oxygen", "O", 8, 16*g/mole);
  G4Element* eCe = new G4Element("Cerium", "Ce", 58, 140.116*g/mole);

  G4double density = 6.63*g/cm3;
  //https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjjmNPGqYryAhWJ-6QKHfvdD_EQFjAOegQIFhAD&url=https%3A%2F%2Fmoem.pensoft.net%2Farticle%2F33240%2Fdownload%2Fpdf%2F290098&usg=AOvVaw3H1ZUR6pq9B-3YXaafYlGN
  //﻿Optical characteritics of single crystal Gd3Al2Ga3O12 : Ce
  //Energy Resolution and Temperature Dependence of Ce:GAGG Coupled to 3mm × 3mm Silicon Photomultipliers, http://eprints.gla.ac.uk/116774/7/116774.pdf
  G4Material *fGAGG= new G4Material("GAGG", density, 5);
  fGAGG->AddElement(eGd,0.509);
  fGAGG->AddElement(eAl,0.058);
  fGAGG->AddElement(eGa, 0.226);
  fGAGG->AddElement(eO,0.207);
  fGAGG->AddElement(eCe,0.0006358);
  /////https://www.advatech-uk.co.uk/GAGG_Ce%20Spectrum.png

  fScintMaterial = fGAGG;

  G4double EnergyGAGG[] =
    {2.65947043332535*eV,
     2.60032958801498*eV,
     2.55252941176471*eV,
     2.52640618603593*eV,
     2.50363038088799*eV,
     2.48126155907974*eV,
     2.44844787304386*eV,
     2.42440157136622*eV,
     2.3827987987988*eV,
     2.33521294933782*eV,
     2.29185227976068*eV,
     2.23423330651649*eV,
     2.17944045516971*eV,
     2.1252358905682*eV,
     2.06211397809541*eV,
     1.99902969228001*eV,
     1.9295827688032*eV,
     1.85390654205607*eV,
     1.80129852440409*eV,
     1.7613140954495*eV
    };

  const G4int nEntriesGAGG = sizeof(EnergyGAGG)/sizeof(G4double);

  ////Effect of Ca 2+ and Zr 4+ co-doping on the optical properties of Gd 3Al 2Ga 3O 12: Ce single crystals
  G4double refractiveIndexGAGG[] =
    {1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9,
     1.9
    };

  assert(sizeof(refractiveIndexGAGG) == sizeof(EnergyGAGG));

  ////Effect of Ca 2+ and Zr 4+ co-doping on the optical properties of Gd 3Al 2Ga 3O 12: Ce single crystals

  G4double absGAGG[] =
    {64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm,
     64.5*cm	
    };

  assert(sizeof(absGAGG) == sizeof(EnergyGAGG));

  ///https://www.advatech-uk.co.uk/GAGG_Ce%20Spectrum.png
  G4double scintillationFastGAGG[] =
    {0.000138612725604,
     0.103013153869685,
     0.259406546344448,
     0.475723654022637,
     0.640644118996635,
     0.803423256347507,
     0.938384253594371,
     0.987677806668706,
     1.00488490364026,
     0.985703770266136,
     0.917267321810951,
     0.786765830529214,
     0.63056840776996,
     0.482941075252371,
     0.335342421229734,
     0.243427844906699,
     0.183661861425512,
     0.128211991434689,
     0.100542023554604,
     0.079262580299786
    }; 

  assert(sizeof(scintillationFastGAGG) == sizeof(EnergyGAGG));
  /// In GAGG the emission wavelength peak is 520 nm

  // Add entries into properties table
  G4MaterialPropertiesTable* mptGAGG = new G4MaterialPropertiesTable();
  mptGAGG->AddProperty("RINDEX",EnergyGAGG,refractiveIndexGAGG,nEntriesGAGG);
  mptGAGG->AddProperty("ABSLENGTH",EnergyGAGG,absGAGG,nEntriesGAGG);
  mptGAGG->AddProperty("FASTCOMPONENT",EnergyGAGG, scintillationFastGAGG,nEntriesGAGG);
  //mptGAGG->AddProperty("SLOWCOMPONENT",EnergyGAGG, scintillationSlowGAGG,nEntriesGAGG);
  mptGAGG->AddConstProperty("FASTTIMECONSTANT", 90*ns);///fast component of the order of 60-130ns
  mptGAGG->AddConstProperty("SLOWTIMECONSTANT", 260*ns);//a slow component of the order of several hundred nanoseconds (260-530ns).
  mptGAGG->AddConstProperty("SCINTILLATIONYIELD",50000./MeV);
  mptGAGG->AddConstProperty("YIELDRATIO",1.0);
  mptGAGG->AddConstProperty("RESOLUTIONSCALE",5.2);//http://eprints.gla.ac.uk/116774/7/116774.pdf

  fScintMPT = mptGAGG;
  
  if (fScint_LV) {
    fScint_LV->SetMaterial(fScintMaterial);
    fScintMaterial->SetMaterialPropertiesTable(fScintMPT);
    fScintMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();//*/

  G4cout << "Scint material set to " << fScintMaterial->GetName() << G4endl;

}


/*//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::BuildCsITlMaterial() {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //--------------------------------------------------
    // CSI_TL
    //--------------------------------------------------
    G4Element* eCs= new G4Element("Caesium", "Cs", 55, 132.9055*g/mole);
    G4Element* eI = new G4Element("Iodine", "I", 53, 126.9044*g/mole);
    G4Material *fCSI= new G4Material("fCSI", 4.51*g/cm3, 2);
    fCSI->AddElement(eCs,1);
    fCSI->AddElement(eI,1);
  G4Material *fTl=G4NistManager::Instance()->FindOrBuildMaterial("G4_Tl");
  ////https://indico.cern.ch/event/771235/contributions/3205236/attachments/1778654/2894242/geant4_spdak2019_hanwook_2.pdf
    G4Material *fCSI_TL= new G4Material("fCSI_TL", 4.51*g/cm3, 2);
    fCSI_TL->AddMaterial(fTl,0.001*perCent);
    fCSI_TL->AddMaterial(fCSI,(100-0.001)*perCent);

  //https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/csitl-and-na-material-data-sheet.pdf
    G4double EnergyCSI_TL[] =
      {3.11*eV,2.80*eV,2.55*eV,2.41*eV,2.27*eV,2.11*eV,1.97*eV,1.87*eV,1.75*eV

      };

    const G4int nEntriesCSI_TL = sizeof(EnergyCSI_TL)/sizeof(G4double);

    G4double refractiveIndexCSI_TL[] =
      {1.79,     1.79,     1.79,     1.79,     1.79,     1.79,     1.79,     1.79,     1.79
      };

    assert(sizeof(refractiveIndexCSI_TL) == sizeof(EnergyCSI_TL));

    G4double absCSI_TL[] =
      {10*cm,     10*cm,     10*cm,
       10*cm,     10*cm,     10*cm,
       10*cm,     10*cm,     10*cm
      };

    assert(sizeof(absCSI_TL) == sizeof(EnergyCSI_TL));

    G4double scintillationFastCSI_TL[] =
      {
   1.13E-01,2.42E-01,5.91E-01,8.49E-01,9.84E-01,7.84E-01,5.26E-01,3.01E-01,1.24E-01

      };

    assert(sizeof(scintillationFastCSI_TL) == sizeof(EnergyCSI_TL));
    /// In CSI_TL the emission wavelength peak is 520 nm

    // Add entries into properties table
    G4MaterialPropertiesTable* mptCSI_TL = new G4MaterialPropertiesTable();
    mptCSI_TL->AddProperty("RINDEX",EnergyCSI_TL,refractiveIndexCSI_TL,nEntriesCSI_TL);
    mptCSI_TL->AddProperty("ABSLENGTH",EnergyCSI_TL,absCSI_TL,nEntriesCSI_TL);
    mptCSI_TL->AddProperty("FASTCOMPONENT",EnergyCSI_TL, scintillationFastCSI_TL,nEntriesCSI_TL);
    //mptCSI_TL->AddProperty("SLOWCOMPONENT",EnergyCSI_TL, scintillationSlowCSI_TL,nEntriesCSI_TL);
    mptCSI_TL->AddConstProperty("FASTTIMECONSTANT", 1000*ns);///
    mptCSI_TL->AddConstProperty("SLOWTIMECONSTANT", 3500*ns);//
    mptCSI_TL->AddConstProperty("SCINTILLATIONYIELD",54000./MeV);
    mptCSI_TL->AddConstProperty("YIELDRATIO",1.0);
    mptCSI_TL->AddConstProperty("RESOLUTIONSCALE",1);//
    //fCSI_TL->SetMaterialPropertiesTable(mptCSI_TL);
  // Set the Birks Constant for the BC408 scintillator
  //fCSI_TL->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

    if (fScint_LV) {
    fScint_LV->SetMaterial(fScintMaterial);
    fScintMaterial->SetMaterialPropertiesTable(fScintMPT);
    fScintMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();//*/

  //G4cout << "Scint material set to " << fScintMaterial->GetName() << G4endl;

//}//*/
