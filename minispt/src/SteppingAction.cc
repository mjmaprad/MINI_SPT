

#include "SteppingAction.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4ProcessManager.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"

#include "RunData.hh"
#include "DetectorConstruction.hh"
//#include "EventAction.hh"
//#include "HistoManager.hh"
#include "TrackInformation.hh"
#include "Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SteppingAction::SteppingAction():
  G4UserSteppingAction(),fVerbose(0),checktrack(-1),checkent(-1),check_evt(-1)
{
    for (G4int i=0;i<12;i++) check_trk[i]=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SteppingAction::UserSteppingAction(const G4Step* step)
{

  //G4cout<<"DEBUG: Begin of User stepping action"<<G4endl;
  
  G4Track* track = step->GetTrack();
  G4StepPoint* endPoint   = step->GetPostStepPoint();
  G4StepPoint* startPoint = step->GetPreStepPoint();

  G4String particleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

  G4String volume = track->GetVolume()->GetName();
  G4String nextvolume = ( track->GetNextVolume() != 0) ? track->GetNextVolume()->GetName() : "OutOfWorld" ;
  G4ThreeVector pos = track->GetPosition();
  G4double energy = startPoint->GetKineticEnergy();
    G4double ekin = startPoint->GetKineticEnergy() / MeV;
  G4int trackID=track-> GetTrackID();
    G4double time_p = track->GetLocalTime()/second/1e-12;// change to nanosecond

  G4double theta = track->GetMomentumDirection().theta();
  G4double phi = track->GetMomentumDirection().phi();
  RunData *runData = static_cast<RunData*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  if(check_evt!=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID()) 
  {
 
     if (track->GetCurrentStepNumber ()==1&&trackID==1) 
     {  
          if (particleName=="e-"||particleName=="e+")runData->SetInitenergy(&ekin,1);
       else if(particleName=="gamma")runData->SetInitenergy(&ekin,2);
              else if(particleName=="proton")runData->SetInitenergy(&ekin,3);
       else runData->SetInitenergy(&ekin,4);
       
        check_evt=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  for (G4int i=0;i<12;i++) check_trk[i]=1;
     }




  }
//G4cout<<nextvolume <<G4endl; getchar();
    //G4cout<<"check_trk "<<check_trk[0]<<" trackid "<<trackID<<" "<<volume<<" "<<nextvolume<<G4endl;

  if (check_trk[0]==trackID&&volume != "scint1_PV" && nextvolume== "scint1_PV")   {runData->SetEnt(&pos,&ekin,&theta,&phi,&time_p); check_trk[0]=0;}
    if (check_trk[1]==trackID&&volume == "scint1_PV" && nextvolume!= "scint1_PV")   {runData->SetEntOut(&pos,&ekin,&theta,&phi); check_trk[1]=0;}
    //
       if (check_trk[2]==trackID&&volume != "scint2_PV" && nextvolume== "scint2_PV")   {runData->SetExit(&pos,&ekin,&theta,&phi,&time_p); check_trk[2]=0;}
    if (check_trk[3]==trackID&&volume == "scint2_PV" && nextvolume!= "scint2_PV")   {runData->SetExitOut(&pos,&ekin,&theta,&phi); check_trk[3]=0;}
    // //
       if (check_trk[4]==trackID&&volume != "SPD1_PV" && nextvolume== "SPD1_PV")   {runData->SetEntSPD1(&pos,&ekin,&theta,&phi); check_trk[4]=0;}
            if (check_trk[5]==trackID&&volume == "SPD1_pcb_PV" && nextvolume!= "SPD1_pcb_PV")   {runData->SetExitSPD1(&pos,&ekin,&theta,&phi); check_trk[5]=0;}
//
       if (check_trk[6]==trackID&&volume != "SPD2_PV" && nextvolume== "SPD2_PV")   {runData->SetEntSPD2(&pos,&ekin,&theta,&phi); check_trk[6]=0;}
            if (check_trk[7]==trackID&&volume == "SPD2_pcb_PV" && nextvolume!= "SPD2_pcb_PV")   {runData->SetExitSPD2(&pos,&ekin,&theta,&phi); check_trk[7]=0;}
            //
                   if (check_trk[8]==trackID&&volume != "SPD3_PV" && nextvolume== "SPD3_PV")   {runData->SetEntSPD3(&pos,&ekin,&theta,&phi); check_trk[8]=0;}
            if (check_trk[9]==trackID&&volume == "SPD3_pcb_PV" && nextvolume!= "SPD3_pcb_PV")   {runData->SetExitSPD3(&pos,&ekin,&theta,&phi); check_trk[9]=0;}
            
            //
                   if (check_trk[10]==trackID&&volume != "SPD4_PV" && nextvolume== "SPD4_PV")   {runData->SetEntSPD4(&pos,&ekin,&theta,&phi); check_trk[10]=0;}
            if (check_trk[11]==trackID&&volume == "SPD4_pcb_PV" && nextvolume!= "SPD4_pcb_PV")   {runData->SetExitSPD4(&pos,&ekin,&theta,&phi); check_trk[11]=0;}
//  }



G4double ffdep=step->GetTotalEnergyDeposit()/keV;
    G4double ffdose=1e12*(step->GetTotalEnergyDeposit()/joule)/
       (startPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetMass()/kg);
  // if ( track->GetVolume()->GetName() == "scint1_PV")    G4cout<<track->GetVolume()->GetName()<<" "<<ffdep<<G4endl;
   if ( track->GetVolume()->GetName() == "scint1_PV")
   {
     runData->SetEntDose(&ffdose,&ffdep);
    }
      if (track->GetVolume()->GetName() == "scint2_PV")
   {
          runData->SetExitDose(&ffdose,&ffdep);

    }
      if ( track->GetVolume()->GetName() == "SPD1_PV")
   {
          runData->SetSPD1Dose( &ffdose,&ffdep);

    }
      if (track->GetVolume()->GetName() == "SPD2_PV")
   {
          runData->SetSPD2Dose( &ffdose,&ffdep);

    }
     if ( track->GetVolume()->GetName() == "SPD3_PV")
   {
          runData->SetSPD3Dose(&ffdose,&ffdep);

    }
      if ( track->GetVolume()->GetName() == "SPD4_PV")
   {
          runData->SetSPD4Dose(&ffdose,&ffdep);

    }
  //G4cout<<"DEBUG: particleName = "<<particleName<<G4endl;
  //G4cout<<"DEBUG: pos = "<<pos.x()<<" , "<<pos.y()<<" , "<<pos.z()<<G4endl;
  //G4cout<<"DEBUG: startVolume = "<<volume<<G4endl;
  //G4cout<<"DEBUG: endVolume = "<<nextvolume<<G4endl;

  /////////////////////////////////////////////////////////////
  // primary particle actions
  if (track->GetTrackID()==1) {

    // gun data //
    if (track->GetCurrentStepNumber()==1) {
      auto gunPos = track->GetVertexPosition();
      runData->SetGunPoint(gunPos.x() /mm, gunPos.y() /mm, gunPos.z() /mm);
      runData->SetGunAngle(track->GetVertexMomentumDirection().theta(),track->GetVertexMomentumDirection().phi());
      //runData->SetGunEnergy(track->GetVertexKineticEnergy());
      runData->SetTotenergy(energy/keV);
      if(particleName =="e-"||particleName =="e+") runData->SetElectronenergy(energy/keV);
      else if(particleName =="gamma") runData->SetGammaenergy(energy/keV);
      else if(particleName =="alpha") runData->SetAlphaenergy(energy/keV);
      else if(particleName =="neutron") runData->SetNeutronenergy(energy/keV);
      else if(particleName =="proton") runData->SetProtonenergy(energy/keV);
    }

    // entrance position and energy//
    if(checkent!=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID()) if (volume != "Scint" && nextvolume == "Scint") {
      runData->SetEntPoint(pos.x() /mm, pos.y() /mm, pos.z() /mm);
      runData->SetEntAngle(track->GetMomentumDirection().theta(),track->GetMomentumDirection().phi());
      runData->SetEntTotenergy(energy/keV);
      if(particleName =="e-"||particleName =="e+") runData->SetEntElectronenergy(energy/keV);
      else if(particleName =="gamma") runData->SetEntGammaenergy(energy/keV);
      else if(particleName =="alpha") runData->SetEntAlphaenergy(energy/keV);
      else if(particleName =="neutron") runData->SetEntNeutronenergy(energy/keV);
      else if(particleName =="proton") runData->SetEntProtonenergy(energy/keV);
      checkent=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    }

    // exit position and energy //
    if ( volume == "Scint" && nextvolume != "Scint" ) {
      runData->SetExitPoint(pos.x() /mm, pos.y() /mm, pos.z() /mm);
      runData->SetExitAngle(track->GetMomentumDirection().theta(), track->GetMomentumDirection().phi());
      runData->SetExitTotenergy(energy/keV);
      if(particleName =="e-"||particleName =="e+") runData->SetExitElectronenergy(energy/keV);
      else if(particleName =="gamma") runData->SetExitGammaenergy(energy/keV);
      else if(particleName =="alpha") runData->SetExitAlphaenergy(energy/keV);
      else if(particleName =="neutron") runData->SetExitNeutronenergy(energy/keV);
      else if(particleName =="proton") runData->SetExitProtonenergy(energy/keV);
    }

  }

  //G4cout<<"DEBUG: stepping action scint"<<G4endl;

  //////////////////////////////////////////////////////////////
  // energy deposit, dose and scintillation position
  if ( volume == "Scint") {

    if( particleName !="opticalphoton") {
      runData->SetDepEnergy(step->GetTotalEnergyDeposit()/keV);
      G4double fdose=(step->GetTotalEnergyDeposit()/joule)/(startPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetMass()/kg);
      runData->SetDose(fdose);//Gy
      G4double feq_dose=0;
      if(particleName=="proton") feq_dose=fdose*5.0;
      else if(particleName=="alpha") feq_dose=fdose*20.0;
      else if(particleName=="neutron") {
	if (energy/keV<10) feq_dose=fdose*5.0;
	else if (energy/keV>=10 && energy/keV<100) feq_dose=fdose*10.0;
	else if (energy/keV>=100 && energy/MeV<2) feq_dose=fdose*20.0;
	else if (energy/MeV>=2 && energy/MeV<20) feq_dose=fdose*10.0;
	else if (energy/MeV>=20) feq_dose=fdose*5.0;
      }
      else feq_dose=fdose;
      runData->SetEqDose(feq_dose);
    }

    const std::vector<const G4Track*>* secondary  = step->GetSecondaryInCurrentStep();
    for (size_t lp=0; lp<(*secondary).size(); lp++) {
      G4ParticleDefinition*  particle = (*secondary)[lp]->GetDefinition();
      G4String name   = particle->GetParticleName();
      if (name="opticalphoton"){
	runData->SetNumPhoton();
	if(checktrack!=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID()) {
	  runData->SetSciPoint(pos.x() /mm, pos.y() /mm, pos.z() /mm);
	  checktrack=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
	}
      }
    }

  }

  //G4cout<<"DEBUG: stepping action photon hit"<<G4endl;

  ////////////////////////////////////////////////////////////////
  //// photon hit to SiPMs
  if (particleName == "opticalphoton") {

    if (endPoint->GetStepStatus() == fGeomBoundary) {

      G4int Nsipm = ((const DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetTotalNumSIPM();
      //G4cout<<"StepAction Nsipm = "<<Nsipm<<G4endl;
      //getchar();

      for (int i_sipm = 0; i_sipm < Nsipm; i_sipm++) {
	sipm_name = "sipmPV" + to_string(i_sipm);
	grease_name = "greasePV" + to_string(i_sipm);
	if (nextvolume == sipm_name && volume == grease_name) {
	  fEkin = energy/eV;
	  runData->sipm(fEkin,i_sipm);
	}
      }

    }
  }

  //G4cout<<"DEBUG: End of User stepping action"<<G4endl;

  return;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
