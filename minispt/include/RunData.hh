//

/// \file RunData.hh
/// \brief Definition of the RunData class

#ifndef RunData_h
#define RunData_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <array>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


const G4int kDim = 36;

///  Run data class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers.
///
/// In order to reduce the number of data members a 2-dimensions array
/// is introduced for each quantity:
/// - fEdep[], fTrackLength[].
///
/// The data are collected step by step in SteppingAction, and
/// the accumulated values are filled in histograms and entuple
/// event by event in B4EventAction.

class RunData : public G4Run {
public:
  RunData();
  virtual ~RunData();

  void FillPerEvent();
  void Reset();

  // inline functions
      inline  void SetInitenergy(G4double *ekinp, G4int prnamep) {
     initE=*ekinp;
                 ent_prname=prnamep;
}
inline void SetEntDose(G4double *edosep, G4double *edepp)
{
    ent_edose += *edosep;
    ent_edep += *edepp;
}
inline void SetExitDose(G4double *edosep, G4double *edepp)
{
    exit_edose +=  *edosep;
    exit_edep += *edepp;
}
inline void SetSPD1Dose(G4double *edosep, G4double *edepp)
{
    SPD1_edose +=  *edosep;
    SPD1_edep += *edepp;
}
inline void SetSPD2Dose(G4double *edosep, G4double *edepp)
{
    SPD2_edose +=  *edosep;
    SPD2_edep+=*edepp;
}
inline void SetSPD3Dose(G4double *edosep, G4double *edepp)
{
    SPD3_edose+= *edosep;
    SPD3_edep+=*edepp;
}
inline void SetSPD4Dose(G4double *edosep, G4double *edepp)
{
    SPD4_edose+= *edosep;
    SPD4_edep+=*edepp;
}
inline void SetEnt(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip, G4double *timep)
{
  // G4cout<<"first"<<G4endl;
    ent_pos= *entp;
    ent_ekin=*ekinp;
        ent_theta=*thetap;
            ent_phi=*phip;
                        ent_time=*timep;


}
inline void SetEntOut(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    ent_out_pos= *entp;
    ent_out_ekin=*ekinp;
        ent_out_theta=*thetap;
            ent_out_phi=*phip;
       //     ent_out_prname=*prnamep;

}
inline void SetExit(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip, G4double *timep)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    exit_pos= *entp;
    exit_ekin=*ekinp;
        exit_theta=*thetap;
            exit_phi=*phip;
                                    exit_time=*timep;
                               //     G4cout<<exit_time<<G4endl;
        //    exit_prname=*prnamep;
    // G4cout<<ent_time<<" "<<exit_time<<" "<<exit_time-ent_time<<G4endl;

}
inline void SetExitOut(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    exit_out_pos= *entp;
    exit_out_ekin=*ekinp;
        exit_out_theta=*thetap;
            exit_out_phi=*phip;
   //         exit_out_prname=*prnamep;

}

inline void SetEntSPD1(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    ent_SPD1_pos= *entp;
    ent_SPD1_ekin=*ekinp;
        ent_SPD1_theta=*thetap;
            ent_SPD1_phi=*phip;
   //         ent_SPD1_prname=*prnamep;
}
inline void SetEntSPD2(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    ent_SPD2_pos= *entp;
    ent_SPD2_ekin=*ekinp;
        ent_SPD2_theta=*thetap;
            ent_SPD2_phi=*phip;
    //        ent_SPD2_prname=*prnamep;
}
inline void SetEntSPD3(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    ent_SPD3_pos= *entp;
    ent_SPD3_ekin=*ekinp;
        ent_SPD3_theta=*thetap;
            ent_SPD3_phi=*phip;
   //         ent_SPD3_prname=*prnamep;
}
inline void SetEntSPD4(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    ent_SPD4_pos= *entp;
    ent_SPD4_ekin=*ekinp;
        ent_SPD4_theta=*thetap;
            ent_SPD4_phi=*phip;
    //        ent_SPD4_prname=*prnamep;
}
/////////////////////////////////////////////////////////////////////

inline void SetExitSPD1(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    exit_SPD1_pos= *entp;
    exit_SPD1_ekin=*ekinp;
        exit_SPD1_theta=*thetap;
            exit_SPD1_phi=*phip;
    //        exit_SPD1_prname=*prnamep;
}
inline void SetExitSPD2(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    exit_SPD2_pos= *entp;
    exit_SPD2_ekin=*ekinp;
        exit_SPD2_theta=*thetap;
            exit_SPD2_phi=*phip;
     //       exit_SPD2_prname=*prnamep;
}
inline void SetExitSPD3(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    exit_SPD3_pos= *entp;
    exit_SPD3_ekin=*ekinp;
        exit_SPD3_theta=*thetap;
            exit_SPD3_phi=*phip;
    //        exit_SPD3_prname=*prnamep;
}
inline void SetExitSPD4(G4ThreeVector *entp, G4double *ekinp,G4double *thetap, G4double *phip)
{
  //  G4cout<<"first"<<fentr<<G4endl;
    exit_SPD4_pos= *entp;
    exit_SPD4_ekin=*ekinp;
        exit_SPD4_theta=*thetap;
            exit_SPD4_phi=*phip;
          //  exit_SPD4_prname=*prnamep;
}
  inline void sipm(G4double energy, G4int number){
    fen[number] += energy;
    fcount[number]++;
  };
  inline void SetGunPoint(G4double dx, G4double dy, G4double dz) {
    fposx_gun = dx;
    fposy_gun = dy;
    fposz_gun = dz;
  }
  inline void SetGunAngle(G4double theta, G4double phi) {
    theta_gun=theta;
    phi_gun=phi;
  }
  inline void SetSciPoint(G4double dx, G4double dy, G4double dz) {
    scintillationOccurred=true;
    fposx_sci = dx;
    fposy_sci = dy;
    fposz_sci = dz;
  }
  inline void SetEntPoint(G4double dx, G4double dy, G4double dz) {
    fposx_ent = dx;
    fposy_ent = dy;
    fposz_ent = dz;
  }
  inline void SetExitPoint(G4double dx, G4double dy, G4double dz) {
    fposx_exi = dx;
    fposy_exi = dy;
    fposz_exi = dz;
  }
  inline void SetEntAngle(G4double theta, G4double phi) {
    theta_ent=theta;
    phi_ent=phi;
  }
  inline void SetExitAngle(G4double theta, G4double phi) {
    theta_exi=theta;
    phi_exi=phi;
  }
  //
  inline void SetDepEnergy(G4double depen) {
    eDep += depen;
  }
  inline void SetNumPhoton() {
    fcount_photon ++;
  }
  inline void SetDose(G4double fdose) {
    dose += fdose;
  }
  inline void SetEqDose(G4double eqdose) {
    eq_dose+=eqdose;
  }
  //
  inline void SetElectronenergy(G4double eenergy) {
    elecspec = eenergy;
    elecnum ++;
    
  }
  inline void SetGammaenergy(G4double genergy) {
    gammaspec = genergy;
    gammanum ++;
  }
  inline void SetProtonenergy(G4double penergy) {
    protonspec = penergy;
    protonnum ++;
  }
  inline void SetNeutronenergy(G4double nenergy) {
    neutronspec = nenergy;
    neutronnum ++;
  }
  inline void SetAlphaenergy(G4double aenergy) {
    alphaspec = aenergy;
    alphanum ++;
  }
  inline void SetTotenergy(G4double totenergy) {
    totspec = totenergy;
  }
  //
  inline void SetEntElectronenergy(G4double eenergy) {
    entelecspec = eenergy;
    entelecnum ++;
  }
  inline void SetEntGammaenergy(G4double genergy) {
    entgammaspec = genergy;
    entgammanum ++;
  }
  inline void SetEntProtonenergy(G4double penergy) {
    entprotonspec = penergy;
    entprotonnum ++;
  }
  inline void SetEntNeutronenergy(G4double nenergy) {
    entneutronspec = nenergy;
    entneutronnum ++;
  }
  inline void SetEntAlphaenergy(G4double aenergy) {
    entalphaspec = aenergy;
    entalphanum ++;
  }
  inline void SetEntTotenergy(G4double totenergy) {
    enttotspec = totenergy;
  }
  //
  inline void SetExitElectronenergy(G4double eenergy) {
    extelecspec = eenergy;
    extelecnum ++;
  }
  inline void SetExitGammaenergy(G4double genergy) {
    extgammaspec = genergy;
    extgammanum ++;
  }
  inline void SetExitProtonenergy(G4double penergy) {
    extprotonspec = penergy;
    extprotonnum ++;
  }
  inline void SetExitNeutronenergy(G4double nenergy) {
    extneutronspec = nenergy;
    extneutronnum ++;
  }
  inline void SetExitAlphaenergy(G4double aenergy) {
    extalphaspec = aenergy;
    extalphanum ++;
  }
  inline void SetExitTotenergy(G4double totenergy) {
    exttotspec = totenergy;
  }


private:
  std::array<G4String, kDim>  fVolumeNames;
  std::array<G4double, kDim>  fEkin;
  std::array<G4double, kDim>  fWavelength;

  G4double fen[128];
  G4int fcount[128];

  G4double fposx_gun;
  G4double fposy_gun;
  G4double fposz_gun;
  G4double phi_gun;
  G4double theta_gun;

  G4bool scintillationOccurred;
  G4double fposx_sci;
  G4double fposy_sci;
  G4double fposz_sci;

  G4double fposx_ent;
  G4double fposy_ent;
  G4double fposz_ent;
  G4double phi_ent;
  G4double theta_ent;

  G4double fposx_exi;
  G4double fposy_exi;
  G4double fposz_exi;
  G4double phi_exi;
  G4double theta_exi;

  G4double eDep;
  G4double dose;
  G4double eq_dose;
  G4int fcount_photon;

  G4double elecspec;
  G4double protonspec;
  G4double gammaspec;
  G4double neutronspec;
  G4double alphaspec;
  G4double totspec;

  G4int elecnum;
  G4int protonnum;
  G4int gammanum;
  G4int neutronnum;
  G4int alphanum;

  G4double entelecspec;
  G4double entprotonspec;
  G4double entgammaspec;
  G4double entneutronspec;
  G4double entalphaspec;
  G4double enttotspec;

  G4int entelecnum;
  G4int entprotonnum;
  G4int entgammanum;
  G4int entneutronnum;
  G4int entalphanum;

  G4double extelecspec;
  G4double extprotonspec;
  G4double extgammaspec;
  G4double extneutronspec;
  G4double extalphaspec;
  G4double exttotspec;

  G4int extelecnum;
  G4int extprotonnum;
  G4int extgammanum;
  G4int extneutronnum;
  G4int extalphanum;
//////

    G4ThreeVector ent_pos;
    G4double ent_ekin;
        G4double ent_dose;
    G4double ent_theta;
    G4double ent_phi;
G4int ent_prname;
        G4double initE;
                G4double ent_edep;
                                G4double ent_edose;
          G4double ent_time;

///////////////////////////
    G4ThreeVector ent_out_pos;
    G4double ent_out_ekin;
        G4double ent_out_dose;
    G4double ent_out_theta;
    G4double ent_out_phi;
G4String ent_out_prname;
// /////////////
    G4ThreeVector exit_pos;
    G4double exit_ekin;
    G4double exit_theta;
    G4double exit_phi;
G4String exit_prname;
                G4double exit_edep;
                G4double exit_edose;
                          G4double exit_time;

/////////////
    G4ThreeVector exit_out_pos;
    G4double exit_out_ekin;
    G4double exit_out_theta;
    G4double exit_out_phi;
//G4String exit_out_prname;
/////////////
    G4ThreeVector ent_SPD1_pos;
    G4double ent_SPD1_ekin;
    G4double ent_SPD1_theta;
    G4double ent_SPD1_phi;
//G4String ent_SPD1_prname;
                G4double SPD1_edep;
                G4double SPD1_edose;
/////////////
    G4ThreeVector ent_SPD2_pos;
    G4double ent_SPD2_ekin;
    G4double ent_SPD2_theta;
    G4double ent_SPD2_phi;
//G4String ent_SPD2_prname;
                G4double SPD2_edep;
                G4double SPD2_edose;

/////////////
    G4ThreeVector ent_SPD3_pos;
    G4double ent_SPD3_ekin;
    G4double ent_SPD3_theta;
    G4double ent_SPD3_phi;
//G4String ent_SPD3_prname;
                G4double SPD3_edep;
                                G4double SPD3_edose;

/////////////
    G4ThreeVector ent_SPD4_pos;
    G4double ent_SPD4_ekin;
    G4double ent_SPD4_theta;
    G4double ent_SPD4_phi;
//G4String ent_SPD4_prname;
                G4double SPD4_edep;
                                G4double SPD4_edose;

//////////////////////////////////////////////////////
/////////////
    G4ThreeVector exit_SPD1_pos;
    G4double exit_SPD1_ekin;
    G4double exit_SPD1_theta;
    G4double exit_SPD1_phi;
//G4String exit_SPD1_prname;
/////////////
    G4ThreeVector exit_SPD2_pos;
    G4double exit_SPD2_ekin;
    G4double exit_SPD2_theta;
    G4double exit_SPD2_phi;
//G4String exit_SPD2_prname;
/////////////
    G4ThreeVector exit_SPD3_pos;
    G4double exit_SPD3_ekin;
    G4double exit_SPD3_theta;
    G4double exit_SPD3_phi;
//G4String exit_SPD3_prname;
/////////////
    G4ThreeVector exit_SPD4_pos;
    G4double exit_SPD4_ekin;
    G4double exit_SPD4_theta;
    G4double exit_SPD4_phi;
//G4String exit_SPD4_prname;
/////////////////////////////////////////////////////
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
