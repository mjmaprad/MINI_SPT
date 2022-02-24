/// \file RunData.cc
/// \brief Implementation of the RunData class

#include "RunData.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunData::RunData()
        : G4Run()
{

  Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunData::~RunData()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunData::FillPerEvent()
{
  G4int counter = 0;

    // get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
//if(ent_prname=="e-"||ent_prname=="e+")
//G4cout<<ent_prname<<G4endl;
 if(ent_prname==1) analysisManager->FillNtupleDColumn(counter++,initE); else analysisManager->FillNtupleDColumn(counter++,0);
if(ent_prname==2) analysisManager->FillNtupleDColumn(counter++,initE); else analysisManager->FillNtupleDColumn(counter++,0);
if(ent_prname==3) analysisManager->FillNtupleDColumn(counter++,initE); else analysisManager->FillNtupleDColumn(counter++,0);
if(ent_prname==4) analysisManager->FillNtupleDColumn(counter++,initE); else analysisManager->FillNtupleDColumn(counter++,0);



    analysisManager->FillNtupleDColumn(counter++,ent_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_ekin);
    analysisManager->FillNtupleDColumn(counter++,ent_theta);
    analysisManager->FillNtupleDColumn(counter++,ent_phi);
   // G4cout<<ent_edep<<G4endl;
       analysisManager->FillNtupleDColumn(counter++,ent_edep);
    analysisManager->FillNtupleDColumn(counter++,ent_edose);
     //
    analysisManager->FillNtupleDColumn(counter++,exit_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_ekin);
    analysisManager->FillNtupleDColumn(counter++,exit_theta);
    analysisManager->FillNtupleDColumn(counter++,exit_phi);
       analysisManager->FillNtupleDColumn(counter++,exit_edep);
    analysisManager->FillNtupleDColumn(counter++,exit_edose);

     //////////////////////////////////////////////////////////////////
    analysisManager->FillNtupleDColumn(counter++,ent_SPD1_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD1_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD1_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD1_ekin);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD1_theta);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD1_phi);
     //
    analysisManager->FillNtupleDColumn(counter++,exit_SPD1_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD1_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD1_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD1_ekin);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD1_theta);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD1_phi);
       analysisManager->FillNtupleDColumn(counter++,SPD1_edep);
    analysisManager->FillNtupleDColumn(counter++,SPD1_edose);
         //////////////////////////////////////////////////////////////////
    analysisManager->FillNtupleDColumn(counter++,ent_SPD2_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD2_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD2_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD2_ekin);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD2_theta);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD2_phi);
     //
    analysisManager->FillNtupleDColumn(counter++,exit_SPD2_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD2_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD2_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD2_ekin);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD2_theta);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD2_phi);
       analysisManager->FillNtupleDColumn(counter++,SPD2_edep);
    analysisManager->FillNtupleDColumn(counter++,SPD2_edose);
         //////////////////////////////////////////////////////////////////
    analysisManager->FillNtupleDColumn(counter++,ent_SPD3_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD3_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD3_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD3_ekin);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD3_theta);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD3_phi);
     //
    analysisManager->FillNtupleDColumn(counter++,exit_SPD3_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD3_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD3_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD3_ekin);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD3_theta);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD3_phi);
       analysisManager->FillNtupleDColumn(counter++,SPD3_edep);
    analysisManager->FillNtupleDColumn(counter++,SPD3_edose);
         //////////////////////////////////////////////////////////////////
    analysisManager->FillNtupleDColumn(counter++,ent_SPD4_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD4_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD4_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD4_ekin);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD4_theta);
    analysisManager->FillNtupleDColumn(counter++,ent_SPD4_phi);
     //
    analysisManager->FillNtupleDColumn(counter++,exit_SPD4_pos.x()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD4_pos.y()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD4_pos.z()/cm);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD4_ekin);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD4_theta);
    analysisManager->FillNtupleDColumn(counter++,exit_SPD4_phi);
       analysisManager->FillNtupleDColumn(counter++,SPD4_edep);
    analysisManager->FillNtupleDColumn(counter++,SPD4_edose);
     if(ent_time!=0&&exit_time!=0) analysisManager->FillNtupleDColumn(counter++,exit_time-ent_time); else analysisManager->FillNtupleDColumn(counter++,0);

    // accumulate statistic, but only if scintillation occur
    if (true || scintillationOccurred) {
    
      G4int Nsipm = ((const DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetTotalNumSIPM();
      //G4cout<<"RunData Fill Nsipm = "<<Nsipm<<G4endl;
      //getchar();
      // should use the same order of ntuple columns declarations!!

      analysisManager->FillNtupleDColumn(counter++,eDep);
      analysisManager->FillNtupleDColumn(counter++,dose);
      analysisManager->FillNtupleDColumn(counter++,eq_dose);
      analysisManager->FillNtupleIColumn(counter++,fcount_photon);

      analysisManager->FillNtupleDColumn(counter++,fposx_sci);
      analysisManager->FillNtupleDColumn(counter++,fposy_sci);
      analysisManager->FillNtupleDColumn(counter++,fposz_sci);

      analysisManager->FillNtupleDColumn(counter++,fposx_gun);
      analysisManager->FillNtupleDColumn(counter++,fposy_gun);
      analysisManager->FillNtupleDColumn(counter++,fposz_gun);
      analysisManager->FillNtupleDColumn(counter++,theta_gun);
      analysisManager->FillNtupleDColumn(counter++,phi_gun);

      analysisManager->FillNtupleDColumn(counter++,elecspec);
      analysisManager->FillNtupleDColumn(counter++,gammaspec);
      analysisManager->FillNtupleDColumn(counter++,alphaspec);
      analysisManager->FillNtupleDColumn(counter++,protonspec);
      analysisManager->FillNtupleDColumn(counter++,neutronspec);
      analysisManager->FillNtupleDColumn(counter++,totspec);

      analysisManager->FillNtupleIColumn(counter++,elecnum);
      analysisManager->FillNtupleIColumn(counter++,gammanum);
      analysisManager->FillNtupleIColumn(counter++,neutronnum);
      analysisManager->FillNtupleIColumn(counter++,protonnum);
      analysisManager->FillNtupleIColumn(counter++,alphanum);

      analysisManager->FillNtupleDColumn(counter++,fposx_ent);
      analysisManager->FillNtupleDColumn(counter++,fposy_ent);
      analysisManager->FillNtupleDColumn(counter++,fposz_ent);
      analysisManager->FillNtupleDColumn(counter++,theta_ent);
      analysisManager->FillNtupleDColumn(counter++,phi_ent);

      analysisManager->FillNtupleDColumn(counter++,entelecspec);
      analysisManager->FillNtupleDColumn(counter++,entgammaspec);
      analysisManager->FillNtupleDColumn(counter++,entalphaspec);
      analysisManager->FillNtupleDColumn(counter++,entprotonspec);
      analysisManager->FillNtupleDColumn(counter++,entneutronspec);
      analysisManager->FillNtupleDColumn(counter++,enttotspec);

      analysisManager->FillNtupleIColumn(counter++,entelecnum);
      analysisManager->FillNtupleIColumn(counter++,entgammanum);
      analysisManager->FillNtupleIColumn(counter++,entneutronnum);
      analysisManager->FillNtupleIColumn(counter++,entprotonnum);
      analysisManager->FillNtupleIColumn(counter++,entalphanum);

      analysisManager->FillNtupleDColumn(counter++,fposx_exi);
      analysisManager->FillNtupleDColumn(counter++,fposy_exi);
      analysisManager->FillNtupleDColumn(counter++,fposz_exi);
      analysisManager->FillNtupleDColumn(counter++,theta_exi);
      analysisManager->FillNtupleDColumn(counter++,phi_exi);

      analysisManager->FillNtupleDColumn(counter++,extelecspec);
      analysisManager->FillNtupleDColumn(counter++,extgammaspec);
      analysisManager->FillNtupleDColumn(counter++,extalphaspec);
      analysisManager->FillNtupleDColumn(counter++,extprotonspec);
      analysisManager->FillNtupleDColumn(counter++,extneutronspec);
      analysisManager->FillNtupleDColumn(counter++,exttotspec);

      analysisManager->FillNtupleIColumn(counter++,extelecnum);
      analysisManager->FillNtupleIColumn(counter++,extgammanum);
      analysisManager->FillNtupleIColumn(counter++,extneutronnum);
      analysisManager->FillNtupleIColumn(counter++,extprotonnum);
      analysisManager->FillNtupleIColumn(counter++,extalphanum);

      for (G4int i_sipm = 0; i_sipm < Nsipm; i_sipm++) {
	analysisManager->FillNtupleDColumn(counter++,fen[i_sipm]);
	analysisManager->FillNtupleIColumn(counter++,fcount[i_sipm]);
      }

      analysisManager->AddNtupleRow();

    }

    Reset();//*/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunData::Reset()
{

  scintillationOccurred=false;

  G4int Nsipm = ((const DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetTotalNumSIPM();
  //G4cout<<"RunData Reset Nsipm = "<<Nsipm<<G4endl;
  //getchar();

  for (G4int i_sipm = 0; i_sipm < Nsipm; i_sipm++) {
  fen[i_sipm] = 0;
  fcount[i_sipm] = 0;
  }

  fposx_gun=-99;
  fposy_gun=-99;
  fposz_gun=-99;
  fposx_sci=-99;
  fposy_sci=-99;
  fposz_sci=-99;
  fposx_ent=-99;
  fposy_ent=-99;
  fposz_ent=-99;
  fposx_exi=-99;
  fposy_exi=-99;
  fposz_exi=-99;
  phi_gun=-9;
  theta_gun=-9;
  phi_ent=-9;
  theta_ent=-9;
  phi_exi=-9;
  theta_exi=-9;

  eDep=0;
  dose=0;
  eq_dose=0;
  fcount_photon=0;

  elecspec=0;
  gammaspec=0;
  protonspec=0;
  neutronspec=0;
  alphaspec=0;
  totspec=0;    //

  entelecspec=0;
  entgammaspec=0;
  entprotonspec=0;
  entneutronspec=0;
  entalphaspec=0;
  enttotspec=0;    //
  entelecnum=0;
  entgammanum=0;
  entneutronnum=0;
  entalphanum=0;
  entprotonnum=0;

  extelecspec=0;
  extgammaspec=0;
  extprotonspec=0;
  extneutronspec=0;
  extalphaspec=0;
  exttotspec=0;    //
  extelecnum=0;
  extgammanum=0;
  extneutronnum=0;
  extalphanum=0;
  extprotonnum=0;

  elecnum=0;
  gammanum=0;
  neutronnum=0;
  alphanum=0;
  protonnum=0;
  ////////////////////////////////////////
  /////////////////////////////////////////
      ent_time=0;
     exit_time=0;
ent_edep=0;
ent_edose=0;
SPD1_edep=0;
SPD1_edose=0;
SPD2_edep=0;
SPD2_edose=0;
SPD3_edep=0;
SPD3_edose=0;
SPD4_edep=0;
SPD4_edose=0;
     ent_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     ent_ekin=0;
     ent_theta=-5;
     ent_phi=-5;
  ent_prname=0;
// // /////////////
// // /////////////
      ent_out_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     ent_out_ekin=0;
     ent_out_theta=-5;
     ent_out_phi=-5;
// // ent_out_prname="Unknown";


     exit_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     exit_ekin=0;
     exit_theta=-5;
     exit_phi=-5;
//  //exit_prname="null";
// /////////////
// /////////////
     exit_out_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     exit_out_ekin=0;
     exit_out_theta=-5;
     exit_out_phi=-5;
//  //exit_out_prname="null";
// /////////////
// /////////////
     ent_SPD1_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     ent_SPD1_ekin=0;
     ent_SPD1_theta=-5;
     ent_SPD1_phi=-5;
//  //ent_SPD1_prname="null";
// /////////////
     ent_SPD2_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     ent_SPD2_ekin=0;
     ent_SPD2_theta=-5;
     ent_SPD2_phi=-5;
//  //ent_SPD2_prname="null";
// /////////////
 ent_SPD3_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     ent_SPD3_ekin=0;
     ent_SPD3_theta=-5;
     ent_SPD3_phi=-5;
//  //ent_SPD3_prname="null";
// /////////////
     ent_SPD4_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     ent_SPD4_ekin=0;
     ent_SPD4_theta=-5;
     ent_SPD4_phi=-5;
//  //ent_SPD4_prname="null";
// //////////////////////////////////////////////////////
// /////////////
// /////////////
     exit_SPD1_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     exit_SPD1_ekin=0;
     exit_SPD1_theta=-5;
     exit_SPD1_phi=-5;
//  //exit_SPD1_prname="null";
// /////////////
     exit_SPD2_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     exit_SPD2_ekin=0;
     exit_SPD2_theta=-5;
     exit_SPD2_phi=-5;
//  //exit_SPD2_prname="null";
// /////////////
     exit_SPD3_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     exit_SPD3_ekin=0;
     exit_SPD3_theta=-5;
     exit_SPD3_phi=-5;
// // exit_SPD3_prname="null";
// /////////////
     exit_SPD4_pos=G4ThreeVector(-20*cm,-20*cm,-20*cm);
     exit_SPD4_ekin=0;
     exit_SPD4_theta=-5;
     exit_SPD4_phi=-5;
// // exit_SPD4_prname="null";

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
