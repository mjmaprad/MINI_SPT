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
/// \file optical/OpNovice2/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "G4Timer.hh"

#include "G4MTRunManager.hh"

#include "RunAction.hh"
//#include "RunData.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "Run.hh"
#include "G4Run.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(PrimaryGeneratorAction* prim)
 : G4UserRunAction(),
   fTimer(nullptr),
   fRun(nullptr),
   fHistoManager(nullptr),
   fPrimary(prim)
{
  fTimer = new G4Timer;

  auto  analysisManager = G4AnalysisManager::Instance();

  analysisManager->SetVerboseLevel(1);

analysisManager->SetNtupleMerging(true);
  analysisManager->CreateNtuple("cubedoz", "");

  G4int Nsipm = 128; // ((const DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetTotalNumSIPM();
  //G4cout<<"RunAction CreateNtuple Nsipm = "<<Nsipm<<G4endl;
  //getchar();
////////////////////////
//////////////////
 analysisManager->CreateNtupleDColumn("electron_src_MeV");
analysisManager->CreateNtupleDColumn("gamma_src_MeV");
analysisManager->CreateNtupleDColumn("proton_src_MeV");
analysisManager->CreateNtupleDColumn("other_src_MeV");
//////////////////
    analysisManager->CreateNtupleDColumn("scint1x_cm");
    analysisManager->CreateNtupleDColumn("scint1y_cm");
    analysisManager->CreateNtupleDColumn("scint1z_cm");
    analysisManager->CreateNtupleDColumn("scint1ekin_MeV");
    analysisManager->CreateNtupleDColumn("scint1theta");
    analysisManager->CreateNtupleDColumn("scint1phi");
      analysisManager->CreateNtupleDColumn("scint1edep_keV");
    analysisManager->CreateNtupleDColumn("scint1dose_pGy");
//////////////////
    analysisManager->CreateNtupleDColumn("scint2x_cm");
    analysisManager->CreateNtupleDColumn("scint2y_cm");
    analysisManager->CreateNtupleDColumn("scint2z_cm");
    analysisManager->CreateNtupleDColumn("scint2ekin_MeV");
    analysisManager->CreateNtupleDColumn("scint2theta");
    analysisManager->CreateNtupleDColumn("scint2phi");
      analysisManager->CreateNtupleDColumn("scint2edep_keV");
    analysisManager->CreateNtupleDColumn("scint2dose_pGy");
    /////////////////////////////////////////////////////////
    analysisManager->CreateNtupleDColumn("entSPD1x_cm");
    analysisManager->CreateNtupleDColumn("entSPD1y_cm");
    analysisManager->CreateNtupleDColumn("entSPD1z_cm");
    analysisManager->CreateNtupleDColumn("entSPD1ekin_MeV");
    analysisManager->CreateNtupleDColumn("entSPD1theta");
    analysisManager->CreateNtupleDColumn("entSPD1phi");
//////////////////
    analysisManager->CreateNtupleDColumn("exitSPD1x_cm");
    analysisManager->CreateNtupleDColumn("exitSPD1y_cm");
    analysisManager->CreateNtupleDColumn("exitSPD1z_cm");
    analysisManager->CreateNtupleDColumn("exitSPD1ekin_MeV");
    analysisManager->CreateNtupleDColumn("exitSPD1theta");
    analysisManager->CreateNtupleDColumn("exitSPD1phi");
      analysisManager->CreateNtupleDColumn("SPD1edep_keV");
    analysisManager->CreateNtupleDColumn("SPD1dose_pGy");
    /////////////////////////////////////////////////////////
    analysisManager->CreateNtupleDColumn("entSPD2x_cm");
    analysisManager->CreateNtupleDColumn("entSPD2y_cm");
    analysisManager->CreateNtupleDColumn("entSPD2z_cm");
    analysisManager->CreateNtupleDColumn("entSPD2ekin_MeV");
    analysisManager->CreateNtupleDColumn("entSPD2theta");
    analysisManager->CreateNtupleDColumn("entSPD2phi");
//////////////////
    analysisManager->CreateNtupleDColumn("exitSPD2x_cm");
    analysisManager->CreateNtupleDColumn("exitSPD2y_cm");
    analysisManager->CreateNtupleDColumn("exitSPD2z_cm");
    analysisManager->CreateNtupleDColumn("exitSPD2ekin_MeV");
    analysisManager->CreateNtupleDColumn("exitSPD2theta");
    analysisManager->CreateNtupleDColumn("exitSPD2phi");
      analysisManager->CreateNtupleDColumn("SPD2edep_keV");
    analysisManager->CreateNtupleDColumn("SPD2dose_pGy");
        /////////////////////////////////////////////////////////
    analysisManager->CreateNtupleDColumn("entSPD3x_cm");
    analysisManager->CreateNtupleDColumn("entSPD3y_cm");
    analysisManager->CreateNtupleDColumn("entSPD3z_cm");
    analysisManager->CreateNtupleDColumn("entSPD3ekin_MeV");
    analysisManager->CreateNtupleDColumn("entSPD3theta");
    analysisManager->CreateNtupleDColumn("entSPD3phi");
//////////////////
    analysisManager->CreateNtupleDColumn("exitSPD3x_cm");
    analysisManager->CreateNtupleDColumn("exitSPD3y_cm");
    analysisManager->CreateNtupleDColumn("exitSPD3z_cm");
    analysisManager->CreateNtupleDColumn("exitSPD3ekin_MeV");
    analysisManager->CreateNtupleDColumn("exitSPD3theta");
    analysisManager->CreateNtupleDColumn("exitSPD3phi");
      analysisManager->CreateNtupleDColumn("SPD3edep_keV");
    analysisManager->CreateNtupleDColumn("SPD3dose_pGy");
    /////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////
    analysisManager->CreateNtupleDColumn("entSPD4x_cm");
    analysisManager->CreateNtupleDColumn("entSPD4y_cm");
    analysisManager->CreateNtupleDColumn("entSPD4z_cm");
    analysisManager->CreateNtupleDColumn("entSPD4ekin_MeV");
    analysisManager->CreateNtupleDColumn("entSPD4theta");
    analysisManager->CreateNtupleDColumn("entSPD4phi");
//////////////////
    analysisManager->CreateNtupleDColumn("exitSPD4x_cm");
    analysisManager->CreateNtupleDColumn("exitSPD4y_cm");
    analysisManager->CreateNtupleDColumn("exitSPD4z_cm");
    analysisManager->CreateNtupleDColumn("exitSPD4ekin_MeV");
    analysisManager->CreateNtupleDColumn("exitSPD4theta");
    analysisManager->CreateNtupleDColumn("exitSPD4phi");
      analysisManager->CreateNtupleDColumn("SPD4edep_keV");
    analysisManager->CreateNtupleDColumn("SPD4dose_pGy");
    /////////////////////////////////////////////////////////
    analysisManager->CreateNtupleDColumn("TOF_ps");


  analysisManager->CreateNtupleDColumn("Total_Energy_Deposit_keV");
  analysisManager->CreateNtupleDColumn("Dose_Gy");
  analysisManager->CreateNtupleDColumn("Dose_Equivalent_Sv");
  analysisManager->CreateNtupleIColumn("Photons_count");

  analysisManager->CreateNtupleDColumn("Sci_x");
  analysisManager->CreateNtupleDColumn("Sci_y");
  analysisManager->CreateNtupleDColumn("Sci_z");

  analysisManager->CreateNtupleDColumn("Gun_x");
  analysisManager->CreateNtupleDColumn("Gun_y");
  analysisManager->CreateNtupleDColumn("Gun_z");
  analysisManager->CreateNtupleDColumn("Gun_Theta");
  analysisManager->CreateNtupleDColumn("Gun_Phi");

  analysisManager->CreateNtupleDColumn("Incident_electron_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Incident_gamma_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Incident_alpha_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Incident_proton_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Incident_neutron_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Incident_tot_spectrum_keV");

  analysisManager->CreateNtupleIColumn("Incident_electron_count");
  analysisManager->CreateNtupleIColumn("Incident_gamma_count");
  analysisManager->CreateNtupleIColumn("Incident_neutron_count");
  analysisManager->CreateNtupleIColumn("Incident_proton_count");
  analysisManager->CreateNtupleIColumn("Incident_alpha_count");

  analysisManager->CreateNtupleDColumn("Enterance_x");
  analysisManager->CreateNtupleDColumn("Enterance_y");
  analysisManager->CreateNtupleDColumn("Enterance_z");
  analysisManager->CreateNtupleDColumn("Enterance_Theta");
  analysisManager->CreateNtupleDColumn("Enterance_Phi");

  analysisManager->CreateNtupleDColumn("Ent_electron_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Ent_gamma_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Ent_alpha_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Ent_proton_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Ent_neutron_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Ent_tot_spectrum_keV");

  analysisManager->CreateNtupleIColumn("Ent_electron_count");
  analysisManager->CreateNtupleIColumn("Ent_gamma_count");
  analysisManager->CreateNtupleIColumn("Ent_neutron_count");
  analysisManager->CreateNtupleIColumn("Ent_proton_count");
  analysisManager->CreateNtupleIColumn("Ent_alpha_count");

  analysisManager->CreateNtupleDColumn("Exit_x");
  analysisManager->CreateNtupleDColumn("Exit_y");
  analysisManager->CreateNtupleDColumn("Exit_z");
  analysisManager->CreateNtupleDColumn("Exit_Theta");
  analysisManager->CreateNtupleDColumn("Exit_Phi");

  analysisManager->CreateNtupleDColumn("Exit_electron_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Exit_gamma_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Exit_alpha_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Exit_proton_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Exit_neutron_spectrum_keV");
  analysisManager->CreateNtupleDColumn("Exit_tot_spectrum_keV");

  analysisManager->CreateNtupleIColumn("Exit_electron_count");
  analysisManager->CreateNtupleIColumn("Exit_gamma_count");
  analysisManager->CreateNtupleIColumn("Exit_neutron_count");
  analysisManager->CreateNtupleIColumn("Exit_proton_count");
  analysisManager->CreateNtupleIColumn("Exit_alpha_count");

  for (G4int i_sipm = 0; i_sipm < Nsipm; i_sipm++) {
    buffer = "Ekin" + to_string(i_sipm);
    analysisManager->CreateNtupleDColumn(buffer);
    buffer = "Count" + to_string(i_sipm);
    analysisManager->CreateNtupleIColumn(buffer);
  }

  analysisManager->FinishNtuple();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
  //delete analysisManager;
}

G4Run* RunAction::GenerateRun()
{
  fRun = new Run();
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  G4MTRunManager::GetRunManager()->SetRandomNumberStore(true);
  /*/std::vector< const G4long > fEventSeeds{77971663, 31016098};
  const G4long s2 = 77971663;
  const G4long s1 = 31016098;
  G4Random::setTheSeeds(&s1, 1);
  G4Random::setTheSeeds(&s2, 2);
  G4Random::showEngineStatus();
  getchar();//*/

  if (fPrimary) {
    G4ParticleDefinition* particle =
      fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }

  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "gamcam";
  analysisManager->OpenFile(fileName);

  fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;

  if (isMaster) fRun->EndOfRun();

  // save ntuples
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
