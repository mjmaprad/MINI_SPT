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
/// \file optical/OpNovice2/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4OpticalSurface.hh"

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"


class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  DetectorConstruction();
  virtual ~DetectorConstruction();

  G4VPhysicalVolume* GetScint() {return fScint;}
  G4double GetScintXSize() {return fScint_x;}

  G4OpticalSurface* GetSurface(void) {return fSurface;}

  void SetSurfaceFinish(const G4OpticalSurfaceFinish finish) { fSurface->SetFinish(finish); G4RunManager::GetRunManager()->GeometryHasBeenModified(); }
  G4OpticalSurfaceFinish GetSurfaceFinish(void) {return fSurface->GetFinish();}

  void SetSurfaceType(const G4SurfaceType type) { fSurface->SetType(type); G4RunManager::GetRunManager()->GeometryHasBeenModified(); }

  void SetSurfaceModel(const G4OpticalSurfaceModel model) { fSurface->SetModel(model); G4RunManager::GetRunManager()->GeometryHasBeenModified(); }
  G4OpticalSurfaceModel GetSurfaceModel(void) {return fSurface->GetModel();}

  void SetSurfaceSigmaAlpha(G4double v);
  void SetSurfacePolish(G4double v);

  void AddScintMPV(const char* c, G4MaterialPropertyVector* mpv);
  void AddScintMPC(const char* c, G4double v);
  G4MaterialPropertiesTable* GetScintMaterialPropertiesTable() {return fScintMPT;}

  void AddWorldMPV(const char* c, G4MaterialPropertyVector* mpv);
  void AddWorldMPC(const char* c, G4double v);
  G4MaterialPropertiesTable* GetWorldMaterialPropertiesTable() {return fWorldMPT;}

  void AddSurfaceMPV(const char* c, G4MaterialPropertyVector* mpv);
  void AddSurfaceMPC(const char* c, G4double v);
  G4MaterialPropertiesTable* GetSurfaceMaterialPropertiesTable() {return fSurfaceMPT;}

  void        SetWorldMaterial(const G4String&);
  G4Material* GetWorldMaterial() const {return fWorldMaterial;}
  void        SetScintMaterial(const G4String&);
  G4Material* GetScintMaterial() const {return fScintMaterial;}

  void SetScintillatorHalfX(G4double v) {fScint_x=v;}
  void SetScintillatorHalfY(G4double v) {fScint_y=v;}
  void SetScintillatorHalfZ(G4double v) {fScint_z=v;}

  void SetWorldHalfX(G4double v) {fExpHall_x=v*2;}
  void SetWorldHalfY(G4double v) {fExpHall_y=v*2;}
  void SetWorldHalfZ(G4double v) {fExpHall_z=v*2;}
  
  void SetScintupHalfX(G4double v) {fScintU_x=v*2;}
  void SetScintupHalfY(G4double v) {fScintU_y=v*2;}
  void SetScintupHalfZ(G4double v) {fScintU_z=v*2;}
  void SetScintupZPos(G4double v) {fscint1_zpos=v;}

  void SetScintlowHalfX(G4double v) {fScintL_x=v*2;}
  void SetScintlowHalfY(G4double v) {fScintL_y=v*2;}
  void SetScintlowHalfZ(G4double v) {fScintL_z=v*2;}
  void SetScintlowZPos(G4double v) {fscint2_zpos=v;}

  void SetSPD1HalfX(G4double v) {SPD1_x=v*2;}
  void SetSPD1HalfY(G4double v) {SPD1_y=v*2;}
  void SetSPD1HalfZ(G4double v) {SPD1_z=v*2;}
  void SetSPD1ZPos(G4double v) {fSPD1_zpos=v;}

  void SetSPD2HalfX(G4double v) {SPD2_x=v*2;}
  void SetSPD2HalfY(G4double v) {SPD2_y=v*2;}
  void SetSPD2HalfZ(G4double v) {SPD2_z=v*2;}
  void SetSPD2ZPos(G4double v) {fSPD2_zpos=v;}
//
  void SetSPD3HalfX(G4double v) {SPD3_x=v*2;}
  void SetSPD3HalfY(G4double v) {SPD3_y=v*2;}
  void SetSPD3HalfZ(G4double v) {SPD3_z=v*2;}
  void SetSPD3ZPos(G4double v) {fSPD3_zpos=v;}

    void SetSPD4HalfX(G4double v) {SPD4_x=v*2;}
  void SetSPD4HalfY(G4double v) {SPD4_y=v*2;}
  void SetSPD4HalfZ(G4double v) {SPD4_z=v*2;}
  void SetSPD4ZPos(G4double v) {fSPD4_zpos=v;}


  void SetMLIHalfX(G4double v) {Al_x=v*2;}
  void SetMLIHalfY(G4double v) {Al_y=v*2;}
  void SetMLIHalfZ(G4double v) {Al_z=v*2;}
  void SetMLIZPos(G4double v) {fAl_zpos=v;}
  //
    void SetPCBHalfZ(G4double v) {SPD1_pcb_z=v*2;}
    /////////////////////////////////////////////////////////////

  G4int CheckSquareSIPMarray(G4int n) {
    if ( (G4int)sqrt(n) == sqrt(n) ) return (G4int)sqrt(n);
    else {
      G4cout<<"WARNING! Only square sipm matrix implemented. Please select a perfect square value as number of SiPMs"<<G4endl;
      exit(0);
    }
  }
  void SetNumBottomSIPM(G4int v) {
    numBottomSIPM_nxn = CheckSquareSIPMarray(v);
    numBottomSIPM = v;
  }
  void SetNumSideSIPM(G4int v) {
    numSideSIPM_nxn = CheckSquareSIPMarray(v);
    numSideSIPM = v;
  }
  G4int GetTotalNumSIPM() const {return numBottomSIPM+numSideSIPM;}
  
  void BuildBC408Material();
  void BuildLYSOMaterial();
  void BuildGAGGMaterial();

  virtual G4VPhysicalVolume* Construct();


private:
///////////////////////////////////////////
    G4double Al_x ;
   G4double Al_y ;
  G4double  Al_z ;
  /////
   G4double   fScintU_x;
  G4double  fScintU_y;
  G4double  fScintU_z;
  //

  G4double  fScintL_x;
  G4double  fScintL_y;
  G4double  fScintL_z;
  ///
   G4double SPD1_x;
   G4double SPD1_y;
   G4double SPD1_z;
   G4double SPD1_pcb_z;
     G4double SPD2_x;
   G4double SPD2_y;
   G4double SPD2_z;
G4double    SPD2_pcb_z;
   G4double   SPD3_x;
 G4double   SPD3_y;
 G4double   SPD3_z;
 G4double   SPD3_pcb_z;
  G4double    SPD4_x;
   G4double SPD4_y;
 G4double   SPD4_z;
  G4double  SPD4_pcb_z;
  ////////////////////////////////////////////
  ////
  G4double fExpHall_x;
  G4double fExpHall_y;
  G4double fExpHall_z;
  ///////////////////////////////
  G4double fAl_zpos;
  G4double fscint1_zpos;
  G4double fscint2_zpos;
  G4double fSPD1_zpos;
    G4double fSPD2_zpos;
  G4double fSPD3_zpos;
  G4double fSPD4_zpos;
  G4LogicalVolume* fWorld_LV;
  G4Material* fWorldMaterial;
  G4MaterialPropertiesTable* fWorldMPT;

  G4double fScint_x;
  G4double fScint_y;
  G4double fScint_z;
  G4LogicalVolume* fScint_LV;
  G4VPhysicalVolume* fScint;
  G4Material* fScintMaterial;
  G4MaterialPropertiesTable* fScintMPT;

  G4int numBottomSIPM;
  G4int numBottomSIPM_nxn;
  G4int numSideSIPM;
  G4int numSideSIPM_nxn;
  G4LogicalVolume* fLogicSiPM;        //pointer to the logical SiPM
  G4LogicalVolume* fLogicGrease;      //pointer to the logical Grease

  G4OpticalSurface* fSurface;
  G4MaterialPropertiesTable* fSurfaceMPT;

  DetectorMessenger* fDetectorMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DetectorConstruction_h*/
