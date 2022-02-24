// version G. Bartolini - 15.07.2021

/// \file optical/OpNovice2/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include <sstream>
#include <iostream>

#include "G4OpticalSurface.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),fDetector(Det)
{
  fOpticalDir = new G4UIdirectory("/mini_spt/");
  fOpticalDir->SetGuidance("Parameters for GAMCAM3D simulation.");

  fSurfaceTypeCmd = new G4UIcmdWithAString("/mini_spt/surface/type", this);
  fSurfaceTypeCmd->SetGuidance("Surface type.");
  fSurfaceTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSurfaceTypeCmd->SetToBeBroadcasted(false);

  fSurfaceFinishCmd = new G4UIcmdWithAString("/mini_spt/surface/finish", this);
  fSurfaceFinishCmd->SetGuidance("Surface finish.");
  fSurfaceFinishCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSurfaceFinishCmd->SetToBeBroadcasted(false);

  fSurfaceModelCmd = new G4UIcmdWithAString("/mini_spt/surface/model", this);
  fSurfaceModelCmd->SetGuidance("surface model.");
  fSurfaceModelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSurfaceModelCmd->SetToBeBroadcasted(false);

  fSurfaceSigmaAlphaCmd = new G4UIcmdWithADouble("/mini_spt/surface/sigmaAlpha", this);
  fSurfaceSigmaAlphaCmd->SetGuidance("surface sigma alpha");
  fSurfaceSigmaAlphaCmd->SetGuidance(" parameter.");
  fSurfaceSigmaAlphaCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSurfaceSigmaAlphaCmd->SetToBeBroadcasted(false);

  fSurfacePolishCmd = new G4UIcmdWithADouble("/mini_spt/surface/polish", this);
  fSurfacePolishCmd->SetGuidance("surface polish");
  fSurfacePolishCmd->SetGuidance(" parameter (for Glisur model).");
  fSurfacePolishCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSurfacePolishCmd->SetToBeBroadcasted(false);

  fSurfaceMatPropVectorCmd = new G4UIcmdWithAString("/mini_spt/surface/property", this);
  fSurfaceMatPropVectorCmd->SetGuidance("Set material property vector");
  fSurfaceMatPropVectorCmd->SetGuidance(" for the surface.");
  fSurfaceMatPropVectorCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSurfaceMatPropVectorCmd->SetToBeBroadcasted(false);

  fSurfaceMatPropConstCmd = new G4UIcmdWithAString("/mini_spt/surface/constProperty", this);
  fSurfaceMatPropConstCmd->SetGuidance("Set material constant property");
  fSurfaceMatPropConstCmd->SetGuidance(" for the surface.");
  fSurfaceMatPropConstCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSurfaceMatPropConstCmd->SetToBeBroadcasted(false);

  fScintillatorMaterialCmd = new G4UIcmdWithAString("/mini_spt/scint/material", this);
  fScintillatorMaterialCmd->SetGuidance("Set material of scintillator. ");
  fScintillatorMaterialCmd->SetGuidance("Can be BC408, LYSO, GAGG or G4 ");
  fScintillatorMaterialCmd->SetGuidance("predef. materials (e.g. G4_BGO)");
  fScintillatorMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintillatorMaterialCmd->SetToBeBroadcasted(false);

  fScintillatorMatPropVectorCmd = new G4UIcmdWithAString("/mini_spt/scint/property", this);
  fScintillatorMatPropVectorCmd->SetGuidance("Set material property vector for ");
  fScintillatorMatPropVectorCmd->SetGuidance("the scintillator.");
  fScintillatorMatPropVectorCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintillatorMatPropVectorCmd->SetToBeBroadcasted(false);

  fScintillatorMatPropConstCmd = new G4UIcmdWithAString("/mini_spt/scint/constProperty", this);
  fScintillatorMatPropConstCmd->SetGuidance("Set material constant property ");
  fScintillatorMatPropConstCmd->SetGuidance("for the scintillator. ");
  fScintillatorMatPropConstCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintillatorMatPropConstCmd->SetToBeBroadcasted(false);

  fScintillatorHalfXCmd = new G4UIcmdWithADouble("/mini_spt/scint/halfx", this);
  fScintillatorHalfXCmd->SetGuidance("Set x half-lenght of scintillator.");
  fScintillatorHalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintillatorHalfXCmd->SetToBeBroadcasted(false);

  fScintillatorHalfYCmd = new G4UIcmdWithADouble("/mini_spt/scint/halfy", this);
  fScintillatorHalfYCmd->SetGuidance("Set y half-lenght of scintillator.");
  fScintillatorHalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintillatorHalfYCmd->SetToBeBroadcasted(false);

  fScintillatorHalfZCmd = new G4UIcmdWithADouble("/mini_spt/scint/halfz", this);
  fScintillatorHalfZCmd->SetGuidance("Set z half-lenght of scintillator.");
  fScintillatorHalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintillatorHalfZCmd->SetToBeBroadcasted(false);

  fWorldMaterialCmd = new G4UIcmdWithAString("/mini_spt/world/material", this);
  fWorldMaterialCmd->SetGuidance("Set material of world.");
  fWorldMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldMaterialCmd->SetToBeBroadcasted(false);

  fWorldMatPropVectorCmd = new G4UIcmdWithAString("/mini_spt/world/property", this);
  fWorldMatPropVectorCmd->SetGuidance("Set material property vector ");
  fWorldMatPropVectorCmd->SetGuidance("for the world.");
  fWorldMatPropVectorCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldMatPropVectorCmd->SetToBeBroadcasted(false);

  fWorldMatPropConstCmd = new G4UIcmdWithAString("/mini_spt/world/constProperty", this);
  fWorldMatPropConstCmd->SetGuidance("Set material constant property ");
  fWorldMatPropConstCmd->SetGuidance("for the world.");
  fWorldMatPropConstCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldMatPropConstCmd->SetToBeBroadcasted(false);

  fWorldHalfXCmd = new G4UIcmdWithADouble("/mini_spt/world/halfx", this);
  fWorldHalfXCmd->SetGuidance("Set x half-lenght of world.");
  fWorldHalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldHalfXCmd->SetToBeBroadcasted(false);

  fWorldHalfYCmd = new G4UIcmdWithADouble("/mini_spt/world/halfy", this);
  fWorldHalfYCmd->SetGuidance("Set y half-lenght of world.");
  fWorldHalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldHalfYCmd->SetToBeBroadcasted(false);

  fWorldHalfZCmd = new G4UIcmdWithADouble("/mini_spt/world/halfz", this);
  fWorldHalfZCmd->SetGuidance("Set z half-lenght of world.");
  fWorldHalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldHalfZCmd->SetToBeBroadcasted(false);

  fSIPMnBottomCmd = new G4UIcmdWithAnInteger("/mini_spt/sipmarray/numBottom", this);
  fSIPMnBottomCmd->SetGuidance("Set number of SIPMs in the bottom matrix.");
  fSIPMnBottomCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSIPMnBottomCmd->SetToBeBroadcasted(false);

  fSIPMnSideCmd = new G4UIcmdWithAnInteger("/mini_spt/sipmarray/numSide", this);
  fSIPMnSideCmd->SetGuidance("Set number of SIPMs in the side matrix.");
  fSIPMnSideCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSIPMnSideCmd->SetToBeBroadcasted(false);

////////////////////////////////////////////////////////
//upper scint.
  fScintupHalfXCmd = new G4UIcmdWithADouble("/mini_spt/scintupper/halfx", this);
  fScintupHalfXCmd->SetGuidance("Set x half-lenght of scintillator in top.");
  fScintupHalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintupHalfXCmd->SetToBeBroadcasted(false);

  fScintupHalfYCmd = new G4UIcmdWithADouble("/mini_spt/scintupper/halfy", this);
  fScintupHalfYCmd->SetGuidance("Set y half-lenght of scintillator in top.");
  fScintupHalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintupHalfYCmd->SetToBeBroadcasted(false);

  fScintupHalfZCmd = new G4UIcmdWithADouble("/mini_spt/scintupper/halfz", this);
  fScintupHalfZCmd->SetGuidance("Set z half-lenght of scintillator in top.");
  fScintupHalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintupHalfZCmd->SetToBeBroadcasted(false);

  fScintupZposCmd = new G4UIcmdWithADouble("/mini_spt/scintupper/zpos", this);
  fScintupZposCmd->SetGuidance("Set z position of scintillator in top.");
  fScintupZposCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintupZposCmd->SetToBeBroadcasted(false);

/////////////////////////////////////////////////////////////////
//Lower scint.
fScintlowHalfXCmd = new G4UIcmdWithADouble("/mini_spt/scintlower/halfx", this);
  fScintlowHalfXCmd->SetGuidance("Set x half-lenght of scintillator in down.");
  fScintlowHalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintlowHalfXCmd->SetToBeBroadcasted(false);

  fScintlowHalfYCmd = new G4UIcmdWithADouble("/mini_spt/scintlower/halfy", this);
  fScintlowHalfYCmd->SetGuidance("Set y half-lenght of scintillator in down.");
  fScintlowHalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintlowHalfYCmd->SetToBeBroadcasted(false);

  fScintlowHalfZCmd = new G4UIcmdWithADouble("/mini_spt/scintlower/halfz", this);
  fScintlowHalfZCmd->SetGuidance("Set z half-lenght of scintillator in down.");
  fScintlowHalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintlowHalfZCmd->SetToBeBroadcasted(false);

  fScintlowZposCmd = new G4UIcmdWithADouble("/mini_spt/scintlower/zpos", this);
  fScintlowZposCmd->SetGuidance("Set z position of scintillator in down.");
  fScintlowZposCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScintlowZposCmd->SetToBeBroadcasted(false);

  /////////////////////////////////////////////////////////////////
//SPD1
  fSPD1HalfXCmd = new G4UIcmdWithADouble("/mini_spt/SPD1/halfx", this);
  fSPD1HalfXCmd->SetGuidance("Set x half-lenght of SPD1.");
  fSPD1HalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD1HalfXCmd->SetToBeBroadcasted(false);

  fSPD1HalfYCmd = new G4UIcmdWithADouble("/mini_spt/SPD1/halfy", this);
  fSPD1HalfYCmd->SetGuidance("Set y half-lenght of SPD1.");
  fSPD1HalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD1HalfYCmd->SetToBeBroadcasted(false);

  fSPD1HalfZCmd = new G4UIcmdWithADouble("/mini_spt/SPD1/halfz", this);
  fSPD1HalfZCmd->SetGuidance("Set z half-lenght of SPD1.");
  fSPD1HalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD1HalfZCmd->SetToBeBroadcasted(false);

  fSPD1ZposCmd = new G4UIcmdWithADouble("/mini_spt/SPD1/zpos", this);
  fSPD1ZposCmd->SetGuidance("Set z position of SPD1.");
  fSPD1ZposCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD1ZposCmd->SetToBeBroadcasted(false);

    /////////////////////////////////////////////////////////////////
//SPD2
  fSPD2HalfXCmd = new G4UIcmdWithADouble("/mini_spt/SPD2/halfx", this);
  fSPD2HalfXCmd->SetGuidance("Set x half-lenght of SPD2.");
  fSPD2HalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD2HalfXCmd->SetToBeBroadcasted(false);

  fSPD2HalfYCmd = new G4UIcmdWithADouble("/mini_spt/SPD2/halfy", this);
  fSPD2HalfYCmd->SetGuidance("Set y half-lenght of SPD2.");
  fSPD2HalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD2HalfYCmd->SetToBeBroadcasted(false);

  fSPD2HalfZCmd = new G4UIcmdWithADouble("/mini_spt/SPD2/halfz", this);
  fSPD2HalfZCmd->SetGuidance("Set z half-lenght of SPD2.");
  fSPD2HalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD2HalfZCmd->SetToBeBroadcasted(false);

  fSPD2ZposCmd = new G4UIcmdWithADouble("/mini_spt/SPD2/zpos", this);
  fSPD2ZposCmd->SetGuidance("Set z position of SPD2.");
  fSPD2ZposCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD2ZposCmd->SetToBeBroadcasted(false);

     /////////////////////////////////////////////////////////////////
//SPD3
  fSPD3HalfXCmd = new G4UIcmdWithADouble("/mini_spt/SPD3/halfx", this);
  fSPD3HalfXCmd->SetGuidance("Set x half-lenght of SPD3.");
  fSPD3HalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD3HalfXCmd->SetToBeBroadcasted(false);

  fSPD3HalfYCmd = new G4UIcmdWithADouble("/mini_spt/SPD3/halfy", this);
  fSPD3HalfYCmd->SetGuidance("Set y half-lenght of SPD3.");
  fSPD3HalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD3HalfYCmd->SetToBeBroadcasted(false);

  fSPD3HalfZCmd = new G4UIcmdWithADouble("/mini_spt/SPD3/halfz", this);
  fSPD3HalfZCmd->SetGuidance("Set z half-lenght of SPD3.");
  fSPD3HalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD3HalfZCmd->SetToBeBroadcasted(false);

  fSPD3ZposCmd = new G4UIcmdWithADouble("/mini_spt/SPD3/zpos", this);
  fSPD3ZposCmd->SetGuidance("Set z position of SPD3.");
  fSPD3ZposCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD3ZposCmd->SetToBeBroadcasted(false);

   /////////////////////////////////////////////////////////////////
//MLI
  fSPD4HalfXCmd = new G4UIcmdWithADouble("/mini_spt/SPD4/halfx", this);
  fSPD4HalfXCmd->SetGuidance("Set x half-lenght of SPD4.");
  fSPD4HalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD4HalfXCmd->SetToBeBroadcasted(false);

  fSPD4HalfYCmd = new G4UIcmdWithADouble("/mini_spt/SPD4/halfy", this);
  fSPD4HalfYCmd->SetGuidance("Set y half-lenght of SPD4.");
  fSPD4HalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD4HalfYCmd->SetToBeBroadcasted(false);

  fSPD4HalfZCmd = new G4UIcmdWithADouble("/mini_spt/SPD4/halfz", this);
  fSPD4HalfZCmd->SetGuidance("Set z half-lenght of SPD4.");
  fSPD4HalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD4HalfZCmd->SetToBeBroadcasted(false);

  fSPD4ZposCmd = new G4UIcmdWithADouble("/mini_spt/SPD4/zpos", this);
  fSPD4ZposCmd->SetGuidance("Set z position of SPD4.");
  fSPD4ZposCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSPD4ZposCmd->SetToBeBroadcasted(false);
  ////////////////////////
    fPCBHalfZCmd = new G4UIcmdWithADouble("/mini_spt/ROC/halfz", this);
  fPCBHalfZCmd->SetGuidance("Set z half-lenght of ROC.");
  fPCBHalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fPCBHalfZCmd->SetToBeBroadcasted(false);
   /////////////////////////////////////////////////////////////////
//MLI
  fMLIHalfXCmd = new G4UIcmdWithADouble("/mini_spt/MLI/halfx", this);
  fMLIHalfXCmd->SetGuidance("Set x half-lenght of MLI.");
  fMLIHalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fMLIHalfXCmd->SetToBeBroadcasted(false);

  fMLIHalfYCmd = new G4UIcmdWithADouble("/mini_spt/MLI/halfy", this);
  fMLIHalfYCmd->SetGuidance("Set y half-lenght of MLI.");
  fMLIHalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fMLIHalfYCmd->SetToBeBroadcasted(false);

  fMLIHalfZCmd = new G4UIcmdWithADouble("/mini_spt/MLI/halfz", this);
  fMLIHalfZCmd->SetGuidance("Set z half-lenght of MLI.");
  fMLIHalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fMLIHalfZCmd->SetToBeBroadcasted(false);

  fMLIZposCmd = new G4UIcmdWithADouble("/mini_spt/MLI/zpos", this);
  fMLIZposCmd->SetGuidance("Set z position of MLI.");
  fMLIZposCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fMLIZposCmd->SetToBeBroadcasted(false);


    /////////////////////////////////////////////////////////////////
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{

  delete fSurfaceFinishCmd;
  delete fSurfaceTypeCmd;
  delete fSurfaceModelCmd;
  delete fSurfaceSigmaAlphaCmd;
  delete fSurfacePolishCmd;
  delete fSurfaceMatPropVectorCmd;
  delete fSurfaceMatPropConstCmd;

  delete fScintillatorMatPropVectorCmd;
  delete fScintillatorMatPropConstCmd;
  delete fScintillatorMaterialCmd;
  delete fScintillatorHalfXCmd;
  delete fScintillatorHalfYCmd;
  delete fScintillatorHalfZCmd;
  
  delete fWorldMatPropVectorCmd;
  delete fWorldMatPropConstCmd;
  delete fWorldMaterialCmd;
  delete fWorldHalfXCmd;
  delete fWorldHalfYCmd;
  delete fWorldHalfZCmd;

  delete fSIPMnBottomCmd;
  delete fSIPMnSideCmd;
///////////////////////////
  delete fScintupHalfXCmd;
  delete fScintupHalfYCmd;
  delete fScintupHalfZCmd;
  delete fScintupZposCmd;
  
  delete   fScintlowHalfXCmd;
  delete fScintlowHalfYCmd;
  delete fScintlowHalfZCmd;
  delete fScintlowZposCmd;
  
  delete   fSPD1HalfXCmd;
  delete fSPD1HalfYCmd;
  delete fSPD1HalfZCmd;
  delete fSPD1ZposCmd;
  
  delete   fSPD2HalfXCmd;
  delete fSPD2HalfYCmd;
  delete fSPD2HalfZCmd;
  delete fSPD2ZposCmd;

  delete   fSPD3HalfXCmd;
  delete fSPD3HalfYCmd;
  delete fSPD3HalfZCmd;
  delete fSPD3ZposCmd;

  delete   fSPD4HalfXCmd;
  delete fSPD4HalfYCmd;
  delete fSPD4HalfZCmd;
  delete fSPD4ZposCmd;

  delete   fMLIHalfXCmd;
  delete fMLIHalfYCmd;
  delete fMLIHalfZCmd;
  delete fMLIZposCmd;

delete fPCBHalfZCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // fScintupHalfXCmd
  // fScintupHalfYCmd
  // fScintupHalfZCmd
  // fScintupZposCmd
  
  //   fScintlowHalfXCmd
  // fScintlowHalfYCmd
  // fScintlowHalfZCmd
  // fScintlowZposCmd
  
  //   fSPD1HalfXCmd
  // fSPD1HalfYCmd
  // fSPD1HalfZCmd
  // fSPD1ZposCmd
  
  //   fSPD2HalfXCmd
  // fSPD2HalfYCmd
  // fSPD2HalfZCmd
  // fSPD2ZposCmd

  //   fSPD3HalfXCmd
  // fSPD3HalfYCmd
  // fSPD3HalfZCmd
  // fSPD3ZposCmd

  //   fSPD4HalfXCmd
  // fSPD4HalfYCmd
  // fSPD4HalfZCmd
  // fSPD4ZposCmd

  //   fMLIHalfXCmd
  // fMLIHalfYCmd
  // fMLIHalfZCmd
  // fMLIZposCmd



void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == fPCBHalfZCmd){
   fDetector->SetPCBHalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
   else if (command == fScintupHalfXCmd) {
    fDetector->SetScintupHalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fScintupHalfYCmd) {
    fDetector->SetScintupHalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    else if (command == fScintupHalfZCmd) {
    fDetector->SetScintupHalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fScintupZposCmd) {
    fDetector->SetScintupZPos(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  ///////
  else if (command == fScintlowHalfXCmd) {
    fDetector->SetScintlowHalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fScintlowHalfYCmd) {
    fDetector->SetScintlowHalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    else if (command == fScintlowHalfZCmd) {
    fDetector->SetScintlowHalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fScintlowZposCmd) {
    fDetector->SetScintlowZPos(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  ///////
  else if (command == fSPD1HalfXCmd) {
    fDetector->SetSPD1HalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fSPD1HalfYCmd) {
    fDetector->SetSPD1HalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    else if (command == fSPD1HalfZCmd) {
    fDetector->SetSPD1HalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
   else if (command == fSPD1ZposCmd) {
    fDetector->SetSPD1ZPos(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  ///////
  else if (command == fSPD2HalfXCmd) {
    fDetector->SetSPD2HalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fSPD2HalfYCmd) {
    fDetector->SetSPD2HalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    else if (command == fSPD2HalfZCmd) {
    fDetector->SetSPD2HalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
   else if (command == fSPD2ZposCmd) {
    fDetector->SetSPD2ZPos(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }

  ///////
  else if (command == fSPD3HalfXCmd) {
    fDetector->SetSPD3HalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fSPD3HalfYCmd) {
    fDetector->SetSPD3HalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    else if (command == fSPD3HalfZCmd) {
    fDetector->SetSPD3HalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
   else if (command == fSPD3ZposCmd) {
    fDetector->SetSPD3ZPos(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  ///////
  else if (command == fSPD4HalfXCmd) {
    fDetector->SetSPD4HalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fSPD4HalfYCmd) {
    fDetector->SetSPD4HalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    else if (command == fSPD4HalfZCmd) {
    fDetector->SetSPD4HalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
   else if (command == fSPD4ZposCmd) {
    fDetector->SetSPD4ZPos(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    ///////
  else if (command == fMLIHalfXCmd) {
    fDetector->SetMLIHalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fMLIHalfYCmd) {
    fDetector->SetMLIHalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
    else if (command == fMLIHalfZCmd) {
    fDetector->SetMLIHalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
   else if (command == fMLIZposCmd) {
    fDetector->SetMLIZPos(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }

  ///////////////////////////////////////////////////////////
 else if (command == fScintillatorMaterialCmd) {
    fDetector->SetScintMaterial(newValue);
  }
  else if (command == fScintillatorHalfXCmd) {
    fDetector->SetScintillatorHalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fScintillatorHalfYCmd) {
    fDetector->SetScintillatorHalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fScintillatorHalfZCmd) {
    fDetector->SetScintillatorHalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fSIPMnBottomCmd) {
    fDetector->SetNumBottomSIPM(G4UIcmdWithAnInteger::GetNewIntValue(newValue));
  }
  else if (command == fSIPMnSideCmd) {
    fDetector->SetNumSideSIPM(G4UIcmdWithAnInteger::GetNewIntValue(newValue));
  }
  //FINISH
  else if (command == fSurfaceFinishCmd) {
    if (newValue == "polished") {
      fDetector->SetSurfaceFinish(polished);
    }
    else if (newValue == "polishedfrontpainted") {
      fDetector->SetSurfaceFinish(polishedfrontpainted);
    }
    else if (newValue == "polishedbackpainted") {
      fDetector->SetSurfaceFinish(polishedbackpainted);
    }
    else if (newValue == "ground") {
      fDetector->SetSurfaceFinish(ground);
    }
    else if (newValue == "groundfrontpainted") {
      fDetector->SetSurfaceFinish(groundfrontpainted);
    }
    else if (newValue == "groundbackpainted") {
      fDetector->SetSurfaceFinish(groundbackpainted);
    }
    else if (newValue == "polishedlumirrorair") {
      fDetector->SetSurfaceFinish(polishedlumirrorair);
    }
    else if (newValue == "polishedlumirrorglue") {
      fDetector->SetSurfaceFinish(polishedlumirrorglue);
    }
    else if (newValue == "polishedair") {
      fDetector->SetSurfaceFinish(polishedair);
    }
    else if (newValue == "polishedteflonair") {
      fDetector->SetSurfaceFinish(polishedteflonair);
    }
    else if (newValue == "polishedtioair") {
      fDetector->SetSurfaceFinish(polishedtioair);
    }
    else if (newValue == "polishedtyvekair") {
      fDetector->SetSurfaceFinish(polishedtyvekair);
    }
    else if (newValue == "polishedvm2000air") {
      fDetector->SetSurfaceFinish(polishedvm2000air);
    }
    else if (newValue == "polishedvm2000glue") {
      fDetector->SetSurfaceFinish(polishedvm2000glue);
    }
    else if (newValue == "etchedlumirrorair") {
      fDetector->SetSurfaceFinish(etchedlumirrorair);
    }
    else if (newValue == "etchedlumirrorglue") {
      fDetector->SetSurfaceFinish(etchedlumirrorglue);
    }
    else if (newValue == "etchedair") {
      fDetector->SetSurfaceFinish(etchedair);
    }
    else if (newValue == "etchedteflonair") {
      fDetector->SetSurfaceFinish(etchedteflonair);
    }
    else if (newValue == "etchedtioair") {
      fDetector->SetSurfaceFinish(etchedtioair);
    }
    else if (newValue == "etchedtyvekair") {
      fDetector->SetSurfaceFinish(etchedtyvekair);
    }
    else if (newValue == "etchedvm2000air") {
      fDetector->SetSurfaceFinish(etchedvm2000air);
    }
    else if (newValue == "etchedvm2000glue") {
      fDetector->SetSurfaceFinish(etchedvm2000glue);
    }
    else if (newValue == "groundlumirrorair") {
      fDetector->SetSurfaceFinish(groundlumirrorair);
    }
    else if (newValue == "groundlumirrorglue") {
      fDetector->SetSurfaceFinish(groundlumirrorglue);
    }
    else if (newValue == "groundair") {
      fDetector->SetSurfaceFinish(groundair);
    }
    else if (newValue == "groundteflonair") {
      fDetector->SetSurfaceFinish(groundteflonair);
    }
    else if (newValue == "groundtioair") {
      fDetector->SetSurfaceFinish(groundtioair);
    }
    else if (newValue == "groundtyvekair") {
      fDetector->SetSurfaceFinish(groundtyvekair);
    }
    else if (newValue == "groundvm2000air") {
      fDetector->SetSurfaceFinish(groundvm2000air);
    }
    else if (newValue == "groundvm2000glue") {
      fDetector->SetSurfaceFinish(groundvm2000glue);
    }
    //         for Davis model
    else if (newValue == "Rough_LUT") {
      fDetector->SetSurfaceFinish(Rough_LUT);
    }
    else if (newValue == "RoughTeflon_LUT") {
      fDetector->SetSurfaceFinish(RoughTeflon_LUT);
    }
    else if (newValue == "RoughESR_LUT") {
      fDetector->SetSurfaceFinish(RoughESR_LUT);
    }
    else if (newValue == "RoughESRGrease_LUT") {
      fDetector->SetSurfaceFinish(RoughESRGrease_LUT);
    }
    else if (newValue == "Polished_LUT") {
      fDetector->SetSurfaceFinish(Polished_LUT);
    }
    else if (newValue == "PolishedTeflon_LUT") {
      fDetector->SetSurfaceFinish(PolishedTeflon_LUT);
    }
    else if (newValue == "PolishedESR_LUT") {
      fDetector->SetSurfaceFinish(PolishedESR_LUT);
    }
    else if (newValue == "PolishedESRGrease_LUT") {
      fDetector->SetSurfaceFinish(PolishedESRGrease_LUT);
    }
    else if (newValue == "Detector_LUT") {
      fDetector->SetSurfaceFinish(Detector_LUT);
    }
    else {
      G4ExceptionDescription ed;
      ed << "Invalid surface finish: " << newValue;
      G4Exception("DetectorMessenger", "GAMCAM3D_003", FatalException,ed);
    }
  }

  //  MODEL
  else if (command == fSurfaceModelCmd) {
    if (newValue == "glisur") {
      fDetector->SetSurfaceModel(glisur);
    }
    else if (newValue == "unified") {
      fDetector->SetSurfaceModel(unified);
    }
    else if (newValue == "LUT") {
      fDetector->SetSurfaceModel(LUT);
    }
    else if (newValue == "DAVIS") {
      fDetector->SetSurfaceModel(DAVIS);
    }
    else if (newValue == "dichroic") {
      fDetector->SetSurfaceModel(dichroic);
    }
    else {
      G4ExceptionDescription ed;
      ed << "Invalid surface model: " << newValue;
      G4Exception("DetectorMessenger", "GAMCAM3D_001", FatalException,ed);
    }
  }

  // TYPE
  else if (command == fSurfaceTypeCmd) {
    if (newValue == "dielectric_metal") {
      fDetector->SetSurfaceType(dielectric_metal);
    }
    else if (newValue == "dielectric_dielectric") {
      fDetector->SetSurfaceType(dielectric_dielectric);
    }
    else if (newValue == "dielectric_LUT") {
      fDetector->SetSurfaceType(dielectric_LUT);
    }
    else if (newValue == "dielectric_LUTDAVIS") {
      fDetector->SetSurfaceType(dielectric_LUTDAVIS);
    }
    else {
      G4ExceptionDescription ed;
      ed << "Invalid surface type: " << newValue;
      G4Exception("DetectorMessenger", "GAMCAM3D_002", FatalException,ed);
    }
  }
  else if (command == fSurfaceSigmaAlphaCmd) {
    fDetector->SetSurfaceSigmaAlpha(
      G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fSurfacePolishCmd) {
    fDetector->SetSurfacePolish(
      G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fScintillatorMatPropVectorCmd) {
    // got a string. need to convert it to physics vector.
    // string format is property name, then pairs of energy, value
    // specify units for each value, eg 3.0*eV
    // space delimited
    G4MaterialPropertyVector* mpv = new G4MaterialPropertyVector();
    std::istringstream instring(newValue);
    G4String prop;
    instring >> prop;
    while (instring) {
      G4String tmp;
      instring >> tmp;
      if (tmp == "") { break; }
      G4double en = G4UIcommand::ConvertToDouble(tmp);
      instring >> tmp;
      G4double val;
      val = G4UIcommand::ConvertToDouble(tmp);
      mpv->InsertValues(en, val);
    }
    const char* c = prop.c_str();

    fDetector->AddScintMPV(c, mpv);
  }
  else if (command == fWorldMatPropVectorCmd) {
    // Convert string to physics vector
    // string format is property name, then pairs of energy, value
    G4MaterialPropertyVector* mpv = new G4MaterialPropertyVector();
    std::istringstream instring(newValue);
    G4String prop;
    instring >> prop;
    while (instring) {
      G4String tmp;
      instring >> tmp;
      if (tmp == "") { break; }
      G4double en = G4UIcommand::ConvertToDouble(tmp);
      instring >> tmp;
      G4double val;
      val = G4UIcommand::ConvertToDouble(tmp);
      mpv->InsertValues(en, val);
    }
    const char* c = prop.c_str();
    fDetector->AddWorldMPV(c, mpv);
  }
  else if (command == fSurfaceMatPropVectorCmd) {
    // Convert string to physics vector
    // string format is property name, then pairs of energy, value
    // space delimited
    G4MaterialPropertyVector* mpv = new G4MaterialPropertyVector();
    G4cout << newValue << G4endl;
    std::istringstream instring(newValue);
    G4String prop;
    instring >> prop;
    while (instring) {
      G4String tmp;
      instring >> tmp;
      if (tmp == "") { break; }
      G4double en = G4UIcommand::ConvertToDouble(tmp);
      instring >> tmp;
      G4double val;
      val = G4UIcommand::ConvertToDouble(tmp);
      mpv->InsertValues(en, val);
    }
    const char* c = prop.c_str();
    fDetector->AddSurfaceMPV(c, mpv);
  }

  else if (command == fScintillatorMatPropConstCmd) {
    // Convert string to physics vector
    // string format is property name, then value
    // space delimited
    std::istringstream instring(newValue);
    G4String prop;
    G4String tmp;
    instring >> prop;
    instring >> tmp;
    G4double val = G4UIcommand::ConvertToDouble(tmp);
    const char* c = prop.c_str();
    fDetector->AddScintMPC(c, val);
  }
  else if (command == fWorldMatPropConstCmd) {
    // Convert string to physics vector
    // string format is property name, then value
    // space delimited
    std::istringstream instring(newValue);
    G4String prop;
    G4String tmp;
    instring >> prop;
    instring >> tmp;
    G4double val = G4UIcommand::ConvertToDouble(tmp);
    const char* c = prop.c_str();
    fDetector->AddScintMPC(c, val);
  }
  else if (command == fSurfaceMatPropConstCmd) {
    // Convert string to physics vector
    // string format is property name, then value
    // space delimited
    std::istringstream instring(newValue);
    G4String prop;
    G4String tmp;
    instring >> prop;
    instring >> tmp;
    G4double val = G4UIcommand::ConvertToDouble(tmp);
    const char* c = prop.c_str();
    fDetector->AddSurfaceMPC(c, val);
  }
 else if (command == fWorldMaterialCmd) {
    fDetector->SetWorldMaterial(newValue);
  }
  else if (command == fWorldHalfXCmd) {
    fDetector->SetWorldHalfX(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fWorldHalfYCmd) {
    fDetector->SetWorldHalfY(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
  else if (command == fWorldHalfZCmd) {
    fDetector->SetWorldHalfZ(G4UIcmdWithADouble::GetNewDoubleValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
