
#include <TTree.h>
#include <TRandom3.h>

#include "Global.h"
#include "LiModel.h"

void COGMethod4cube(TString run_gunpos="8x6y-20k", TString run_part_energy="gamma500keV", Int_t run_z_height = 10, TString run_material="LYSO", Bool_t run_exactAngle = false) {

  gunpos = run_gunpos;
  part_energy = run_part_energy;
  scint_material = run_material; // BC408 LYSO GAGG

  cristal_z_halfwidth = 0.5*run_z_height;
  z_height = to_string( run_z_height );

  ExactAngle = run_exactAngle;

  COGreco();
  
}

void COGreco() {

  if (useSideMatrix && SideSiPMlenght==0) useSideMatrix=false;

  TRandom3 *r3 = new TRandom3(0);

  Int_t LiModelParN = (useSideMatrix) ? 6 : 5;
  TMinuit *LiModelMN = new TMinuit(LiModelParN);  //initialize TMinuit with its parameters number
  LiModelMN->SetFCN(LiModelLS);
  static TString  parname[6]    = { "li-x", "li-y", "li-z", "li-A0",    "li-tau1",    "li-tau2" };
  static Double_t parstep[6]    = {   0.01,   0.01,   0.01,   0.001,      0.00001,      0.00001 };
  //static Double_t limit_down[6] = {      0,      0,      0,       0,            0,            0 };
  //static Double_t limit_up[6]   = {      0,      0,      0,      10,            1,            1 };
  static Double_t limit_down[6] = {    -cristal_x_halfwidth,    -cristal_y_halfwidth,    -cristal_z_halfwidth,       0,            0,            0 };
  static Double_t limit_up[6]   = {     cristal_x_halfwidth,     cristal_y_halfwidth,     cristal_z_halfwidth,      10,          0.1,          0.1 };

  //for (Int_t en = 0; en < 1; en++) {
  //for (TString gunpos : GunPositions) {

  //cout<<"Processing "<<gunpos<<" simulation"<<endl;

  TString filename = "RootFiles/"+scint_material+"-H"+z_height+"mm-"+part_energy+"-"+gunpos+".root"; // 26mmCube/cube-psReflX
  if (gSystem->AccessPathName(filename,kFileExists)) {cout<<"ERROR! File "<<filename<<" not found!"<<endl; return;}
  cout<<"Running on file "<<filename<<endl;
  f = new TFile(filename);
  tree = (TTree *) f->Get("cubedoz");
  Nentries = tree->GetEntries();
  //cout<<"Nentries = "<< Nentries << endl; //getchar();
  int step = 100; if (Nentries>10000) step = 300; if (Nentries>30000) step = 1000; if (Nentries>100000) step = 3000; if (Nentries>300000) step = 1000; if (Nentries>1000000) step = Nentries/400;
  //cout<<"step = "<< step << endl; getchar();
  TString sArray = "OneArray"; if (useSideMatrix) sArray = "TwoArray";
  //TString plotsdir = "Plots/BC408-26mmCube/"+sArray+"-moreRefl/LS-withRefl-A0Cut-X/"+gunpos;
  TString plotsdir = "Plots/"+scint_material+"-Hscan/"+z_height+"mm/"+part_energy+"/"+gunpos+"-v7";
  system("mkdir -p "+plotsdir);

  //Histogram definitions
  h_rec = new TH2D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) h_rec_yz = new TH2D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  h_recI2 = new TH2D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) h_recI2_yz = new TH2D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  h_recEkin = new TH2D("", "Anger Logic reconstruction; X [mm]; Y [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) h_recEkin_yz = new TH2D("", "Anger Logic reconstruction; Y [mm]; Z [mm]", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  h_recEkin2 = new TH2D("", "Square Charge reconstruction; X [mm]; Y [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) h_recEkin2_yz = new TH2D("", "Square Charge reconstruction; Y [mm]; Z [mm]", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  h_recLi     = new TH2D("", "Li Model reconstruction - "+part_energy+"; X [mm]; Y [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  h_recLi_xz  = new TH2D("", "Li Model reconstruction - "+part_energy+"; X [mm]; Z [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);
  h_recLi_yz  = new TH2D("", "Li Model reconstruction - "+part_energy+"; Y [mm]; Z [mm]", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);
  h_recLi_xyz = new TH3D("", "Li Model reconstruction - "+part_energy+"; X [mm]; Y [mm]; Z [mm]",
			 100, -cristal_x_halfwidth, cristal_x_halfwidth,
			 100, -cristal_y_halfwidth, cristal_y_halfwidth,
			 100, -cristal_z_halfwidth, cristal_z_halfwidth
			 );

  h_gun = new TH2D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  h_gun_yz = new TH2D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  h_scin = new TH2D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  h_scin_xz = new TH2D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);
  h_scin_yz = new TH2D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  //h_entr = new TH2D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  //h_entr_yz = new TH2D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  //h_exit = new TH2D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  //h_exit_yz = new TH2D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  res_X = new TH1D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth);
  res_Y = new TH1D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) res_Z = new TH1D("", "", 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  resI2_X = new TH1D("", "", 100, -cristal_x_halfwidth, cristal_x_halfwidth);
  resI2_Y = new TH1D("", "", 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) resI2_Z = new TH1D("", "", 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  resEkin_X = new TH1D("", "Anger Logic X resolution - "+part_energy+"; x_{scint} - x_{rec} [mm]; Events", 100, -cristal_x_halfwidth, cristal_x_halfwidth);
  resEkin_Y = new TH1D("", "Anger Logic Y resolution - "+part_energy+"; y_{scint} - y_{rec} [mm]; Events", 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) resEkin_Z = new TH1D("", "Anger Logic Z resolution - "+part_energy+"; z_{scint} - z_{rec} [mm]; Events", 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  resEkin2_X = new TH1D("", "Square Charge X resolution - "+part_energy+"; x_{scint} - x_{rec} [mm]; Events", 100, -cristal_x_halfwidth, cristal_x_halfwidth);
  resEkin2_Y = new TH1D("", "Square Charge Y resolution - "+part_energy+"; y_{scint} - y_{rec} [mm]; Events", 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  if (useSideMatrix) resEkin2_Z = new TH1D("", "Square Charge Z resolution - "+part_energy+"; z_{scint} - z_{rec} [mm]; Events", 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  resLi_X = new TH1D("", "Li Model X resolution - "+part_energy+"; x_{scint} - x_{rec} [mm]; Events", 100, -cristal_x_halfwidth, cristal_x_halfwidth);
  resLi_Y = new TH1D("", "Li Model Y resolution - "+part_energy+"; y_{scint} - y_{rec} [mm]; Events", 100, -cristal_y_halfwidth, cristal_y_halfwidth);
  resLi_Z = new TH1D("", "Li Model Z resolution - "+part_energy+"; z_{scint} - z_{rec} [mm]; Events", 100, -cristal_z_halfwidth, cristal_z_halfwidth);

  count_X = new TH1D("", "", 120, 0, 500);
  if (useSideMatrix) count_Y = new TH1D("", "", 120, 0, 4000);
  if (useSideMatrix) count_Z = new TH1D("", "", 120, 0, 4000);

  pixel_energy = new TH1D("", "", 100, 0, 200);
  total_energy = new TH1D("", "", 100, 0, 2000);
  deposited_energy = new TH1D("", "", 100, 0, 200);
  low_en_avg = new TH1D("LowADCavg", "Low ADC Avg", 100, 0, 20);

  h_photonNum = new TH1D("", "", 200, 0, 0.4);
  h_Mj = new TH1D("", "", 200, 0, 0.4);
  truth_cost_LS = new TH1D("", "", 200, 0, 2);
  truth_cost_LS1 = new TH1D("", "", 200, 0, 2);
  truth_cost_LS2 = new TH1D("", "", 200, 0, 2);
  LiFit_cost_LS = new TH1D("", "", 200, 0, 0.2);
  LiFit_cost_LL = new TH1D("", "", 200, 4, 20);

  h_A0 = new TH1D("", "", 100, 0, 1.5);
  h_tau1 = new TH1D("", "", 100, 0, 0.02);
  if (useSideMatrix) h_tau2 = new TH1D("", "", 100, 0, 0.02);

  for (int i=0; i<BotSiPMlenght; i++)   bot_sipm_xpos[i] =  bot_sipm_ypos[i] = 3.2 * (i -  BotSiPMlenght/2. + 0.5);
  for (int i=0; i<SideSiPMlenght; i++) side_sipm_ypos[i] = side_sipm_zpos[i] = 3.2 * (i - SideSiPMlenght/2. + 0.5);

  for (Int_t i_sipm = 0; i_sipm < NsipmBot+NsipmSide; i_sipm++) {
    tree->SetBranchAddress(Form("Ekin%d",i_sipm), &ekin[i_sipm]);
    tree->SetBranchAddress(Form("Count%d", i_sipm), &pcount[i_sipm]);
    /*if (i_sipm < NsipmBot) cout<<"i_sipm = "<<i_sipm<<" , bot_sipm_xpos = "<<bot_sipm_xpos[i_sipm/BotSiPMlenght]<<" , bot_sipm_ypos = "<<bot_sipm_ypos[i_sipm%BotSiPMlenght]<<endl;
      else {
      Int_t j_sipm = i_sipm - NsipmBot;
      cout<<"i_sipm = "<<i_sipm<<" , side_sipm_ypos = "<<side_sipm_ypos[j_sipm%SideSiPMlenght]<<" , side_sipm_zpos = "<<side_sipm_zpos[j_sipm/SideSiPMlenght]<<endl;
      }//*/
  }
  //return;

  tree->SetBranchAddress("Sci_x", &scin_y);
  tree->SetBranchAddress("Sci_y", &scin_z);
  tree->SetBranchAddress("Sci_z", &scin_x);

  tree->SetBranchAddress("Gun_x", &gun_y);
  tree->SetBranchAddress("Gun_y", &gun_z);
  tree->SetBranchAddress("Gun_z", &gun_x);

  //tree->SetBranchAddress("Enterance_x", &entr_x);
  //tree->SetBranchAddress("Enterance_y", &entr_z);
  //tree->SetBranchAddress("Enterance_z", &entr_y);

  tree->SetBranchAddress("Total_Energy_Deposit_keV", &energy_depo);

  for (Int_t i = 0; i < Nentries; i++) {

    if (i % step == 0) cout<<"Processing event "<<i<<" of "<<Nentries<<endl;

    tree->GetEntry(i);

    //cout<<"Processing entry "<<i<<endl; //getchar();

    h_gun->Fill(gun_x, gun_y);
    h_gun_yz->Fill(gun_y, gun_z);

    if (scin_z==-99) continue; //skip no hit events

    h_scin->Fill(scin_x, scin_y);
    h_scin_xz->Fill(scin_x, scin_z);
    h_scin_yz->Fill(scin_y, scin_z);

    //if (scin_z<=-6 || scin_z>2) continue; //scan z positions

    //scin_x *= 10; scin_y *= 10; scin_z *= 10; //convert from cm to mm //not needed anymore
	  
    ph_tot_x = 0;
    ph_tot_y = 0;
    ph_tot_z = 0;
    rec_X = 0;
    rec_Y = 0;
    rec_Z = 0;
    phI2_tot_x = 0;
    phI2_tot_y = 0;
    phI2_tot_z = 0;
    recI2_X = 0;
    recI2_Y = 0;
    recI2_Z = 0;
    Ekin_tot_x = 0;
    Ekin_tot_y = 0;
    Ekin_tot_z = 0;
    recEkin_X = 0;
    recEkin_Y = 0;
    recEkin_Z = 0;
    Ekin2_tot_x = 0;
    Ekin2_tot_y = 0;
    Ekin2_tot_z = 0;
    recEkin2_X = 0;
    recEkin2_Y = 0;
    recEkin2_Z = 0;
    int cxy=0, cz=0;
    int i_maxEn_x=0;
    int i_maxEn_z=NsipmBot;

    for (int i_sipm = 0; i_sipm < NsipmBot+NsipmSide; i_sipm++) {
      //cout<<"DEBUG: i_sipm = "<<i_sipm<<" ,  pcount = "<<pcount[i_sipm]<<" , ekin = "<<ekin[i_sipm]<<endl;
      //pcount[i_sipm]*=pixel_enesf[i_sipm];
      //ekin[i_sipm]*=pixel_enesf[i_sipm];
      //cout<<"DEBUG: After norm:   pcount = "<<pcount[i_sipm]<<" , ekin = "<<ekin[i_sipm]<<endl;
      if (pcount[i_sipm] != 0) pixel_energy->Fill(ekin[i_sipm]);
      if (i_sipm < NsipmBot) {
	if (pcount[i_sipm] > 4) cxy++; //does this make sense with 40% PDE, 1.5 pe treashold?
	Energy_x_sorted[i_sipm]=ekin[i_sipm];
	if (ekin[i_sipm]>ekin[i_maxEn_x]) i_maxEn_x = i_sipm;
	ph_tot_x += pcount[i_sipm];
	ph_tot_y += pcount[i_sipm];
	rec_X += bot_sipm_xpos[i_sipm/BotSiPMlenght] * pcount[i_sipm];
	rec_Y += bot_sipm_ypos[i_sipm%BotSiPMlenght] * pcount[i_sipm];
	//cout<<"DEBUG: i_sipm = "<<i_sipm<<" , pcount = "<<pcount[i_sipm]<<" , ph_tot = "<<ph_tot<<" , rec_X = "<<rec_X<<" , rec_Y = "<<rec_Y<<endl;
	phI2_tot_x += (pcount[i_sipm] * pcount[i_sipm]);
	phI2_tot_y += (pcount[i_sipm] * pcount[i_sipm]);
	recI2_X += bot_sipm_xpos[i_sipm/BotSiPMlenght] * (pcount[i_sipm] * pcount[i_sipm]);
	recI2_Y += bot_sipm_ypos[i_sipm%BotSiPMlenght] * (pcount[i_sipm] * pcount[i_sipm]);
	Ekin_tot_x += ekin[i_sipm];
	Ekin_tot_y += ekin[i_sipm];
	recEkin_X += bot_sipm_xpos[i_sipm/BotSiPMlenght] * ekin[i_sipm];
	recEkin_Y += bot_sipm_ypos[i_sipm%BotSiPMlenght] * ekin[i_sipm];
	Ekin2_tot_x += (ekin[i_sipm] * ekin[i_sipm]);
	Ekin2_tot_y += (ekin[i_sipm] * ekin[i_sipm]);
	recEkin2_X += bot_sipm_xpos[i_sipm/BotSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);
	recEkin2_Y += bot_sipm_ypos[i_sipm%BotSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);
      } else {
	if (useSideMatrix) {
	  int j_sipm = i_sipm-NsipmBot;
	  if (pcount[i_sipm] != 0) cz++;
	  Energy_z_sorted[j_sipm]=ekin[i_sipm];
	  if (ekin[i_sipm]>ekin[i_maxEn_z]) i_maxEn_z = i_sipm;
	  ph_tot_z += pcount[i_sipm];
	  ph_tot_y += pcount[i_sipm];
	  rec_Z += side_sipm_zpos[j_sipm%SideSiPMlenght] * pcount[i_sipm];
	  rec_Y += side_sipm_ypos[j_sipm/SideSiPMlenght] * pcount[i_sipm];
	  //cout<<"DEBUG: j_sipm = "<<j_sipm<<" , pcount = "<<pcount[i_sipm]<<" , ph_tot = "<<ph_tot<<" , rec_z = "<<rec_z<<" , rec_Y = "<<rec_Y<<endl;
	  phI2_tot_z += (pcount[i_sipm] * pcount[i_sipm]);
	  phI2_tot_y += (pcount[i_sipm] * pcount[i_sipm]);
	  recI2_Z += side_sipm_zpos[j_sipm%SideSiPMlenght] * (pcount[i_sipm] * pcount[i_sipm]);
	  recI2_Y += side_sipm_ypos[j_sipm/SideSiPMlenght] * (pcount[i_sipm] * pcount[i_sipm]);
	  Ekin_tot_z += ekin[i_sipm];
	  Ekin_tot_y += ekin[i_sipm];
	  recEkin_Z += side_sipm_zpos[j_sipm%SideSiPMlenght] * ekin[i_sipm];
	  recEkin_Y += side_sipm_ypos[j_sipm/SideSiPMlenght] * ekin[i_sipm];
	  Ekin2_tot_z += (ekin[i_sipm] * ekin[i_sipm]);
	  Ekin2_tot_y += (ekin[i_sipm] * ekin[i_sipm]);
	  recEkin2_Z += side_sipm_zpos[j_sipm%SideSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);
	  recEkin2_Y += side_sipm_ypos[j_sipm/SideSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);//*/
	}
      }
    }

    //if (cxy < 4 || (useSideMatrix && cz < 4) ) continue;

    rec_X = rec_X / ph_tot_x;
    rec_Y = rec_Y / ph_tot_y;
    rec_Z = (useSideMatrix) ? rec_Z / ph_tot_z : 0;
    //cout<<"DEBUG: Results: "<<" rec_X = "<<rec_X<<" , rec_Y = "<<rec_Y<<endl; //getchar();

    recI2_X = recI2_X / phI2_tot_x;
    recI2_Y = recI2_Y / phI2_tot_y;
    recI2_Z = (useSideMatrix) ? recI2_Z / phI2_tot_z : 0; //

    recEkin_X = recEkin_X / Ekin_tot_x;
    recEkin_Y = recEkin_Y / Ekin_tot_y;
    recEkin_Z = (useSideMatrix) ? recEkin_Z / Ekin_tot_z : 0; //

    recEkin2_X = recEkin2_X / Ekin2_tot_x;
    recEkin2_Y = recEkin2_Y / Ekin2_tot_y;

    //cout<<"sumEkin2_Z = "<<recEkin2_Z<<"  ,  Ekin2_tot_z = "<<Ekin2_tot_z<<endl;
    recEkin2_Z = (useSideMatrix) ? recEkin2_Z / Ekin2_tot_z : 0; //
    //cout<<"recEkin2_Z = "<<recEkin2_Z<<endl;

    ////fill histos
    //h_scinX->Fill(scin_x);
    //h_scinY->Fill(scin_y);
    //h_scinZ->Fill(scin_z);

    count_X->Fill(ph_tot_x);

    //total_energy->Fill(Ekin_tot_y);
    deposited_energy->Fill(energy_depo);

    h_rec->Fill(rec_X, rec_Y);
    h_recI2->Fill(recI2_X, recI2_Y);
    h_recEkin->Fill(recEkin_X, recEkin_Y);
    h_recEkin2->Fill(recEkin2_X, recEkin2_Y);

    //cout<<"x : "<< rec_X<< " "<< ele_x<<endl;
    //cout<<"y : "<< rec_Y<< " "<< ele_y<<endl;
    res_X->Fill(scin_x - rec_X);
    res_Y->Fill(scin_y - rec_Y);
    resI2_X->Fill(scin_x - recI2_X);
    resI2_Y->Fill(scin_y - recI2_Y);
    resEkin_X->Fill(scin_x - recEkin_X);
    resEkin_Y->Fill(scin_y - recEkin_Y);
    resEkin2_X->Fill(scin_x - recEkin2_X);
    resEkin2_Y->Fill(scin_y - recEkin2_Y);

    if (useSideMatrix) {
      count_Y->Fill(ph_tot_y);
      count_Z->Fill(ph_tot_z);
      h_rec_yz->Fill(rec_Y, rec_Z);
      h_recI2_yz->Fill(recI2_Y, recI2_Z);
      h_recEkin_yz->Fill(recEkin_Y, recEkin_Z);
      h_recEkin2_yz->Fill(recEkin2_Y, recEkin2_Z);
      res_Z->Fill(scin_z - rec_Z);
      resI2_Z->Fill(scin_z - recI2_Z);
      resEkin_Z->Fill(scin_z - recEkin_Z);
      resEkin2_Z->Fill(scin_z - recEkin2_Z);
    }

    /*/ENERGYMOD (only bottom)
    double dtemp, gmean = 0.2*Ekin_tot_x/NsipmBot, gsig = gmean/10.;
    for (int i_sipm = 0; i_sipm < NsipmBot; i_sipm++) {
      dtemp = r3->Gaus(gmean, gsig);
      ekin[i_sipm] += abs(dtemp);
      Energy_x_sorted[i_sipm]=ekin[i_sipm];
      Ekin_tot_x += dtemp;
    }//*/
    total_energy->Fill(Ekin_tot_x);

    Double_t LS = 0, LS1 = 0, LS2 = 0;
    //bottom matrix
    for (int i_sipm = 0; i_sipm < NsipmBot; i_sipm++) {
      mx = ekin[i_sipm]/Ekin_tot_x;
      Double_t pn = photonNum(bot_sipm_xpos[i_sipm/BotSiPMlenght], bot_sipm_ypos[i_sipm%BotSiPMlenght], -cristal_z_halfwidth, scin_x, scin_y, scin_z, 0.2, 0.004, ExactAngle, Reflections);
      h_photonNum->Fill(pn);
      h_Mj->Fill(mx);
      LS1 += (pn - mx) * (pn - mx);
    }
    if (useSideMatrix) truth_cost_LS1->Fill(LS1);
    //side matrix
    for (int i_sipm = NsipmBot; useSideMatrix && i_sipm < NsipmBot+NsipmSide; i_sipm++) {
      mz = ekin[i_sipm]/Ekin_tot_z;
      Double_t pn = photonNum(side_sipm_zpos[i_sipm%SideSiPMlenght], side_sipm_ypos[(i_sipm-NsipmBot)/SideSiPMlenght], -cristal_x_halfwidth, scin_z, scin_y, scin_x, 1, 0, ExactAngle, Reflections);
      h_photonNum->Fill(pn); 
      h_Mj->Fill(mz); 
      LS2 += (pn - mz) * (pn - mz);
    }//*/
    if (useSideMatrix) truth_cost_LS2->Fill(LS2);
    LS = LS1+LS2;
    truth_cost_LS->Fill(LS);//*/

    //position of simps with max energy
    max_x = bot_sipm_xpos[i_maxEn_x/BotSiPMlenght];
    max_y_x = bot_sipm_ypos[i_maxEn_x%BotSiPMlenght];
    max_y_z = (useSideMatrix) ? side_sipm_ypos[(i_maxEn_z-NsipmBot)/SideSiPMlenght] : max_y_x;
    max_y = 0.5*(max_y_x+max_y_z);
    max_z = (useSideMatrix) ? side_sipm_zpos[i_maxEn_z%SideSiPMlenght] : 0; //

    //sort energy to determine low energy background across the two matrixes
    int temp;
    for (int j=0; j<NsipmBot; j++) for (int k=j+1; k<NsipmBot; k++) {
	if (Energy_x_sorted[j]>Energy_x_sorted[k]) {
	  temp = Energy_x_sorted[j];
	  Energy_x_sorted[j] = Energy_x_sorted[k];
	  Energy_x_sorted[k] = temp;
	}
	if (useSideMatrix && Energy_z_sorted[j]>Energy_z_sorted[k]) {
	  temp = Energy_z_sorted[j];
	  Energy_z_sorted[j] = Energy_z_sorted[k];
	  Energy_z_sorted[k] = temp;
	}
      }
    Int_t N_low_bot = NsipmBot/4;
    low_en_avg_1 = 0;
    for (int j=0; j<N_low_bot; j++) low_en_avg_1 += Energy_x_sorted[j];
    low_en_avg_1 /= (1.*Ekin_tot_x*N_low_bot);
    low_en_avg_1 = max(0.002,low_en_avg_1);
    if (useSideMatrix) {
      low_en_avg_2 = 0;
      for (int j=0; j<15; j++) low_en_avg_2 += Energy_z_sorted[j];
      low_en_avg_2 /= (Ekin_tot_z*15.); 
      low_en_avg_2 = max(0.01,low_en_avg_2);
    }

    Double_t arglist[10];
    Int_t argnum = 0, ierflg = 0;
    vector<Int_t> mndebugier; mndebugier.clear();

    //SET PRIntout <level>
    arglist[0] = -1;
    argnum = 1;
    LiModelMN->mnexcm("SET PRI", arglist , argnum, ierflg);
    mndebugier.push_back(ierflg);

    //SET NOWarnings
    argnum = 0;
    LiModelMN->mnexcm("SET NOW", arglist , argnum, ierflg);
    mndebugier.push_back(ierflg);

    //Set 1 sigma tolerance for fcn change to determine minimum
    arglist[0] = 1;
    argnum = 1;
    LiModelMN->mnexcm("SET ERR", arglist , argnum, ierflg);
    mndebugier.push_back(ierflg);

    //SET STRategy  <level>
    arglist[0] = 2;
    argnum = 1;
    LiModelMN->mnexcm("SET STR", arglist , argnum, ierflg);
    mndebugier.push_back(ierflg);

    //start_x = max_x; start_y = max_y; start_z = max_z;
    start_x = recEkin2_X; start_y = recEkin2_Y; start_z = recEkin2_Z;
    Double_t vstart[6]     = {  start_x,  start_y,  start_z,     0.8, low_en_avg_1, low_en_avg_2 };

    // Set names, starting values, step sizes and limits for parameters
    for (int ipar=0; ipar<LiModelParN; ipar++) {
      LiModelMN->mnparm(ipar, parname[ipar], vstart[ipar], parstep[ipar], limit_down[ipar], limit_up[ipar], ierflg);
      mndebugier.push_back(ierflg);
    }
    //getchar();

    // Now ready for minimization step
    arglist[0] = 500; //maxcalls
    arglist[1] = 0.1; //tolerance
    argnum = 2;
    //LiModelMN->mnexcm("MIGRAD", arglist ,argnum, ierflg);
    ierflg = LiModelMN->Migrad(); // migrad minimization with default settings (maxcalls, tolerance, etc...)
    mndebugier.push_back(ierflg);
    //getchar();

    // Print results
    Double_t amin,edm,errdef;
    Int_t nvpar,nparx,icstat;
    LiModelMN->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
    //getchar();
    //LiModelMN->mnprin(3,amin); 

    //for (int k=0; k<mndebugier.size(); k++) cout<<"ier"<<k<<" "<<mndebugier[k]<<endl;

    //get parameters values
    Double_t li_x, li_y, li_z, li_A0, li_tau1, li_tau2;
    Double_t li_x_err, li_y_err, li_z_err, li_A0_err, li_tau1_err, li_tau2_err;
    LiModelMN->GetParameter(0, li_x, li_x_err);
    LiModelMN->GetParameter(1, li_y, li_y_err);
    LiModelMN->GetParameter(2, li_z, li_z_err);
    LiModelMN->GetParameter(3, li_A0, li_A0_err);
    LiModelMN->GetParameter(4, li_tau1, li_tau1_err);
    if (useSideMatrix) LiModelMN->GetParameter(5, li_tau2, li_tau2_err);

    //if (i<66) continue;
    /*cout<<endl
      <<"Event "<<i<<endl
      <<"Max sipm positions:           max_x      = "<<max_x     <<" , max_y_x    = "<<max_y_x   <<" , max_y_z    = "<<max_y_z   <<" , max_y      = "<<max_y     <<" , max_z      = "<<max_z
      <<" , i_maxEn_x = "<<i_maxEn_x<<" , i_maxEn_z = "<<i_maxEn_z<<endl
      <<"Low Energy Average:           low_en_avg = "<<low_en_avg_1<<endl
      //<<"Anger Logic count results:    rec_X      = "<<rec_X     <<" , rec_Y      = "<<rec_Y     <<" , rec_Z      = "<<rec_Z<<endl
      //<<"Square Charge count results:  recI2_X    = "<<recI2_X   <<" , recI2_Y    = "<<recI2_Y   <<" , recI2_Z    = "<<recI2_Z<<endl
      //<<"Anger Logic energy results:   recEkin_X  = "<<recEkin_X <<" , recEkin_Y  = "<<recEkin_Y <<" , recEkin_Z  = "<<recEkin_Z<<endl
      <<"Square Charge energy results: recEkin2_X = "<<recEkin2_X<<" , recEkin2_Y = "<<recEkin2_Y<<" , recEkin2_Z = "<<recEkin2_Z<<endl
      <<"Li Model fit results:         li_x       = "<<li_x      <<" , li_y       = "<<li_y      <<" , li_z       = "<<li_z<<endl
      //<<"Li Model fit outputs:         amin       = "<<amin      <<" , edm        = "<<edm       <<" , icstat     = "<<icstat<<endl
      <<"Truth scintillation position: scin_x     = "<<scin_x    <<" , scin_y     = "<<scin_y    <<" , scin_z     = "<<scin_z<<endl
      <<endl;
      //if ( abs(li_z) < 0.1 )
      getchar(); //*/

    /*/draw event distributions
      h_event_xy = new TH2D("", "", BotSiPMlenght, 0, BotSiPMlenght, BotSiPMlenght, 0, BotSiPMlenght);
      if (useSideMatrix) h_event_yz = new TH2D("", "", SideSiPMlenght, 0, SideSiPMlenght, SideSiPMlenght, 0, SideSiPMlenght);
      for (int i_sipm=0; i_sipm<NsipmBot; i_sipm++) {
      h_event_xy->SetBinContent(1+i_sipm/BotSiPMlenght, 1+i_sipm%BotSiPMlenght, ekin[i_sipm]);
      if (useSideMatrix) h_event_yz->SetBinContent(1+i_sipm/SideSiPMlenght, 1+i_sipm%SideSiPMlenght, ekin[NsipmBot+i_sipm]);
      }

      TCanvas *c_event_xy = new TCanvas("cevent_xy", "event_xy");
      h_event_xy->SetStats(false);
      h_event_xy->Draw("colz");
      //gPad->Modified(); gPad->Update();
      if (useSideMatrix) {
      TCanvas *c_event_yz = new TCanvas("cevent_yz", "event_yz");
      h_event_yz->SetStats(false);
      h_event_yz->Draw("colz");
      //gPad->Modified(); gPad->Update();
      }
      TString evtplotsdir = "Plots/Lyso12mmCube/OneArray/LS/"+gunpos+"/evtplots/";
      system("mkdir -p "+evtplotsdir);
      c_event_xy->SaveAs(evtplotsdir+Form("%d.pdf",i));
      delete c_event_xy;
      //if (useSideMatrix) delete c_event_yz;
      if (i>200) return;//*/

    Bool_t li_limits_ok = true;

    Double_t v_limit = cristal_x_halfwidth-0.3; //-0.3; //-1.
    if (li_x < -v_limit) li_limits_ok = false; //continue; //li_x = -cristal_x_halfwidth;
    if (li_x > v_limit)  li_limits_ok = false; //continue; //li_x = cristal_x_halfwidth;

    v_limit = cristal_y_halfwidth-0.3;
    if (li_y < -v_limit) li_limits_ok = false; //continue; //li_y = -cristal_y_halfwidth;
    if (li_y > v_limit)  li_limits_ok = false; //continue; //li_y = cristal_y_halfwidth;

    v_limit = cristal_z_halfwidth-0.3;
    if (li_z > v_limit)  li_limits_ok = false; //continue; //li_z = cristal_z_halfwidth;
    v_limit = cristal_z_halfwidth-0.6; //0.6
    if (li_z < -v_limit) li_limits_ok = false; //continue; //li_z = -cristal_z_halfwidth;

    //if (li_z == 0)  li_limits_ok = false; //continue;  //*/
    //if ( abs(li_z+0.06) < 0.1 )  li_limits_ok = false;  
    if ( li_A0 < 0.03 )  li_limits_ok = false;  

    low_en_avg->Fill(low_en_avg_1*Ekin_tot_x);
    if (li_limits_ok) {
      h_recLi->Fill(li_x, li_y);
      h_recLi_xz->Fill(li_x, li_z);
      h_recLi_yz->Fill(li_y, li_z);
      h_recLi_xyz->Fill(li_x, li_y, li_z);
      resLi_X->Fill(scin_x - li_x);
      resLi_Y->Fill(scin_y - li_y);
      resLi_Z->Fill(scin_z - li_z);
      LiFit_cost_LS->Fill(amin);
      LiFit_cost_LL->Fill(amin);
      h_A0->Fill(li_A0);
      h_tau1->Fill(li_tau1);
      if (useSideMatrix) h_tau2->Fill(li_tau2);
    }

  } // end event loop

  cout<<"End of event loop"<<endl;

  TString plotname;

  TCanvas *c_rec = new TCanvas("crec", "rec");
  c_rec->SetLogz(false);
  h_rec->Draw("colz");
  plotname = plotsdir+"/reconstructed";
  c_rec->SaveAs(plotname+".pdf");
  c_rec->SaveAs(plotname+".C");
  delete c_rec;

  TCanvas *c_recI2 = new TCanvas("crecI2", "recI2");
  c_recI2->SetLogz(false);
  h_recI2->Draw("colz");
  plotname = plotsdir+"/reconstructed_I2";
  c_recI2->SaveAs(plotname+".pdf");
  c_recI2->SaveAs(plotname+".C");
  delete c_recI2;
	
  TCanvas *c_recEkin = new TCanvas("crecEkin", "recEkin");
  c_recEkin->SetLogz(false);
  h_recEkin->Draw("colz");
  plotname = plotsdir+"/reconstructed_Ekin";
  c_recEkin->SaveAs(plotname+".pdf");
  c_recEkin->SaveAs(plotname+".C");
  delete c_recEkin;

  TCanvas *c_recEkin2 = new TCanvas("crecEkin2", "recEkin2");
  c_recEkin2->SetLogz(false);
  h_recEkin2->Draw("colz");
  plotname = plotsdir+"/reconstructed_Ekin2";
  c_recEkin2->SaveAs(plotname+".pdf");
  c_recEkin2->SaveAs(plotname+".C");
  delete c_recEkin2;
	
  TCanvas *c_recLi = new TCanvas("crecLi", "recLi");
  c_recLi->SetLogz(false);
  h_recLi->Draw("colz");
  plotname = plotsdir+"/reconstructed_LiModel_XY";
  c_recLi->SaveAs(plotname+".pdf");
  c_recLi->SaveAs(plotname+".C");
  delete c_recLi;

  TCanvas *c_recLi_xz = new TCanvas("crecLi_xz", "recLi_xz");
  c_recLi_xz->SetLogz(false);
  h_recLi_xz->Draw("colz");
  plotname = plotsdir+"/reconstructed_LiModel_XZ";
  c_recLi_xz->SaveAs(plotname+".pdf");
  c_recLi_xz->SaveAs(plotname+".C");
  delete c_recLi_xz;

  TCanvas *c_recLi_yz = new TCanvas("crecLi_yz", "recLi_yz");
  c_recLi_yz->SetLogz(false);
  h_recLi_yz->Draw("colz");
  plotname = plotsdir+"/reconstructed_LiModel_YZ";
  c_recLi_yz->SaveAs(plotname+".pdf");
  c_recLi_yz->SaveAs(plotname+".C");
  delete c_recLi_yz;

  /*TCanvas *c_recLi_filt = new TCanvas("crecLi_filt", "recLi_filt");
    c_recLi_filt->SetLogz(false);
    TH2D *h_recLi_filt = new TH2D(*h_recLi);
    double maxbinvalue = h_recLi_filt->GetMaximum();
    int h2nbins = (2+h_recLi_filt->GetNbinsX())*(2+h_recLi_filt->GetNbinsY());
    for (int ibin=0; ibin<h2nbins; ibin++) {
    double binvalue = h_recLi_filt->GetBinContent(ibin);
    if (binvalue!=0 && binvalue<sqrt(maxbinvalue)) h_recLi_filt->SetBinContent(ibin,0);
    }
    h_recLi_filt->SetEntries(h_recLi_filt->Integral());
    h_recLi_filt->Draw("colz");
    plotname = plotsdir+"/reconstructed_LiModel_XY_filtered";
    c_recLi->SaveAs(plotname+".pdf");
    c_recLi->SaveAs(plotname+".C");
    delete c_recLi;//*/

  TCanvas *c_gun = new TCanvas("cgun", "gun");
  c_gun->SetLogz(false);
  h_gun->Draw("colz");
  plotname = plotsdir+"/gun";
  c_gun->SaveAs(plotname+".pdf");
  c_gun->SaveAs(plotname+".C");
  delete c_gun;

  TCanvas *c_gun_yz = new TCanvas("cgun_yz", "gun_yz");
  c_gun_yz->SetLogz(false);
  h_gun_yz->Draw("colz");
  plotname = plotsdir+"/gun_yz";
  c_gun_yz->SaveAs(plotname+".pdf");
  c_gun_yz->SaveAs(plotname+".C");
  delete c_gun_yz;

  TCanvas *c_scin = new TCanvas("cscin", "scin");
  c_scin->SetLogz(false);
  h_scin->Draw("colz");
  plotname = plotsdir+"/scin";
  c_scin->SaveAs(plotname+".pdf");
  c_scin->SaveAs(plotname+".C");
  delete c_scin;

  TCanvas *c_scin_xz = new TCanvas("cscin_xz", "scin_xz");
  c_scin_xz->SetLogz(false);
  h_scin_xz->Draw("colz");
  plotname = plotsdir+"/scin_xz";
  c_scin_xz->SaveAs(plotname+".pdf");
  c_scin_xz->SaveAs(plotname+".C");
  delete c_scin_xz;

  TCanvas *c_scin_yz = new TCanvas("cscin_yz", "scin_yz");
  c_scin_yz->SetLogz(false);
  h_scin_yz->Draw("colz");
  plotname = plotsdir+"/scin_yz";
  c_scin_yz->SaveAs(plotname+".pdf");
  c_scin_yz->SaveAs(plotname+".C");
  delete c_scin_yz;

  TCanvas *c_scinx = new TCanvas("scinx", "scin_x");
  h_scin->ProjectionX()->Draw("hist");
  plotname = plotsdir+"/scinX";
  c_scinx->SaveAs(plotname+".pdf");
  c_scinx->SaveAs(plotname+".C");
  delete c_scinx;

  TCanvas *c_sciny = new TCanvas("sciny", "scin_y");
  h_scin->ProjectionY()->Draw("hist");
  plotname = plotsdir+"/scinY";
  c_sciny->SaveAs(plotname+".pdf");
  c_sciny->SaveAs(plotname+".C");
  delete c_sciny;

  TCanvas *c_scinz = new TCanvas("scinz", "scin_z");
  h_scin_yz->ProjectionY()->Draw("hist");
  plotname = plotsdir+"/scinZ";
  c_scinz->SaveAs(plotname+".pdf");
  c_scinz->SaveAs(plotname+".C");
  delete c_scinz;

  TCanvas *c_photonNum = new TCanvas("photonNum", "photonNum");
  c_photonNum->SetLogy(true);
  h_photonNum->Draw("hist");
  plotname = plotsdir+"/photonNum";
  c_photonNum->SaveAs(plotname+".pdf");
  c_photonNum->SaveAs(plotname+".C");
  delete c_photonNum;

  TCanvas *c_Mj = new TCanvas("Mj", "Mj");
  c_Mj->SetLogy(true);
  h_Mj->Draw("hist");
  plotname = plotsdir+"/Mj";
  c_Mj->SaveAs(plotname+".pdf");
  c_Mj->SaveAs(plotname+".C");
  delete c_Mj;

  TCanvas *c_cost_LS = new TCanvas("costLS", "truth_cost_LS");
  //c_cost_LS->SetLogy(true);
  truth_cost_LS->Draw("hist");
  plotname = plotsdir+"/truth_cost_LS";
  c_cost_LS->SaveAs(plotname+".pdf");
  c_cost_LS->SaveAs(plotname+".C");
  delete c_cost_LS;

  TCanvas *c_LiFit_LS = new TCanvas("LiFitLS", "LiFit_cost_LS");
  //c_LiFit_LS->SetLogy(true);
  LiFit_cost_LS->Draw("hist");
  plotname = plotsdir+"/LiFit_cost_LS";
  c_LiFit_LS->SaveAs(plotname+".pdf");
  c_LiFit_LS->SaveAs(plotname+".C");
  delete c_LiFit_LS;

  TCanvas *c_LiFit_LL = new TCanvas("LiFitLL", "LiFit_cost_LL");
  //c_LiFit_LL->SetLogy(true);
  LiFit_cost_LL->Draw("hist");
  plotname = plotsdir+"/LiFit_cost_LL";
  c_LiFit_LL->SaveAs(plotname+".pdf");
  c_LiFit_LL->SaveAs(plotname+".C");
  delete c_LiFit_LL;

  TCanvas *c_A0 = new TCanvas("A0", "A0");
  //c_A0->SetLogy(true);
  h_A0->Draw("hist");
  plotname = plotsdir+"/A0";
  c_A0->SaveAs(plotname+".pdf");
  c_A0->SaveAs(plotname+".C");
  delete c_A0;

  TCanvas *c_tau1 = new TCanvas("tau1", "tau1");
  //c_tau1->SetLogy(true);
  h_tau1->Draw("hist");
  plotname = plotsdir+"/tau1";
  c_tau1->SaveAs(plotname+".pdf");
  c_tau1->SaveAs(plotname+".C");
  delete c_tau1;

  TCanvas *c_recLix = new TCanvas("recLix", "recLi_x");
  h_recLi->ProjectionX()->Draw("hist");
  plotname = plotsdir+"/recLi_X";
  c_recLix->SaveAs(plotname+".pdf");
  c_recLix->SaveAs(plotname+".C");
  delete c_recLix;

  TCanvas *c_recLiy = new TCanvas("recLiy", "recLi_y");
  h_recLi->ProjectionY()->Draw("hist");
  plotname = plotsdir+"/recLi_Y";
  c_recLiy->SaveAs(plotname+".pdf");
  c_recLiy->SaveAs(plotname+".C");
  delete c_recLiy;

  TCanvas *c_recLiz = new TCanvas("recLiz", "recLi_z");
  h_recLi_yz->ProjectionY()->Draw("hist");
  plotname = plotsdir+"/recLi_Z";
  c_recLiz->SaveAs(plotname+".pdf");
  c_recLiz->SaveAs(plotname+".C");
  delete c_recLiz;

  int bin1;
  int bin2;
  double fwhm;
  TPaveText *fwhmtext = new TPaveText(0.64, 0.64, 0.86, 0.72, "blNDC");
  fwhmtext->SetBorderSize(0);
  fwhmtext->SetFillColor(0);

  /*TCanvas *c_resx = new TCanvas("resx", "res_x");
    res_X->SetMarkerStyle(2);;
    res_X->SetFillColor(11);
    res_X->Draw();
    plotname = plotsdir+"/resx";
    c_resx->SaveAs(plotname+".pdf");
    c_resx->SaveAs(plotname+".C");
    delete c_resx;

    TCanvas *c_resy = new TCanvas("resy", "res_y");
    res_Y->SetMarkerStyle(7);;
    res_Y->SetFillColor(3);
    res_Y->Draw();
    plotname = plotsdir+"/resy";
    c_resy->SaveAs(plotname+".pdf");
    c_resy->SaveAs(plotname+".C");
    delete c_resy;

    TCanvas *c_resz = new TCanvas("resz", "res_z");
    res_Z->SetMarkerStyle(7);;
    res_Z->SetFillColor(4);
    res_Z->Draw();
    plotname = plotsdir+"/resz";
    c_resz->SaveAs(plotname+".pdf");
    c_resz->SaveAs(plotname+".C");
    delete c_resz;

    TCanvas *c_resI2x = new TCanvas("resI2x", "resI2_x");
    resI2_X->SetMarkerStyle(2);;
    resI2_X->SetFillColor(11);
    resI2_X->Draw();
    plotname = plotsdir+"/resI2_x";
    c_resI2x->SaveAs(plotname+".pdf");
    c_resI2x->SaveAs(plotname+".C");
    delete c_resI2x;

    TCanvas *c_resI2y = new TCanvas("resI2y", "resI2_y");
    resI2_Y->SetMarkerStyle(7);;
    resI2_Y->SetFillColor(3);
    resI2_Y->Draw();
    plotname = plotsdir+"/resI2_y";
    c_resI2y->SaveAs(plotname+".pdf");
    c_resI2y->SaveAs(plotname+".C");
    delete c_resI2y;

    TCanvas *c_resI2z = new TCanvas("resI2z", "resI2_z");
    resI2_Z->SetMarkerStyle(7);;
    resI2_Z->SetFillColor(4);
    resI2_Z->Draw();
    plotname = plotsdir+"/resI2_z";
    c_resI2z->SaveAs(plotname+".pdf");
    c_resI2z->SaveAs(plotname+".C");
    delete c_resI2z;//*/

  TCanvas *c_resEkinx = new TCanvas("resEkinx", "resEkin_x");
  resEkin_X->SetMarkerStyle(2);;
  resEkin_X->SetFillColor(11);
  resEkin_X->Draw();
  bin1 = resEkin_X->FindFirstBinAbove(resEkin_X->GetMaximum()/2.1);
  bin2 = resEkin_X->FindLastBinAbove(resEkin_X->GetMaximum()/2.1);
  fwhm = resEkin_X->GetBinCenter(bin2) - resEkin_X->GetBinCenter(bin1);
  fwhmtext->Clear();
  fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
  fwhmtext->Draw("same");
  plotname = plotsdir+"/resEkin_x";
  c_resEkinx->SaveAs(plotname+".pdf");
  c_resEkinx->SaveAs(plotname+".C");
  delete c_resEkinx;

  TCanvas *c_resEkiny = new TCanvas("resEkiny", "resEkin_y");
  resEkin_Y->SetMarkerStyle(7);;
  resEkin_Y->SetFillColor(3);
  resEkin_Y->Draw();
  bin1 = resEkin_Y->FindFirstBinAbove(resEkin_Y->GetMaximum()/2.1);
  bin2 = resEkin_Y->FindLastBinAbove(resEkin_Y->GetMaximum()/2.1);
  fwhm = resEkin_Y->GetBinCenter(bin2) - resEkin_Y->GetBinCenter(bin1);
  fwhmtext->Clear();
  fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
  fwhmtext->Draw("same");
  plotname = plotsdir+"/resEkin_y";
  c_resEkiny->SaveAs(plotname+".pdf");
  c_resEkiny->SaveAs(plotname+".C");
  delete c_resEkiny;

  TCanvas *c_resEkin2x = new TCanvas("resEkin2x", "resEkin2_x");
  resEkin2_X->SetMarkerStyle(2);;
  resEkin2_X->SetFillColor(11);
  resEkin2_X->Draw();
  bin1 = resEkin2_X->FindFirstBinAbove(resEkin2_X->GetMaximum()/2.1);
  bin2 = resEkin2_X->FindLastBinAbove(resEkin2_X->GetMaximum()/2.1);
  fwhm = resEkin2_X->GetBinCenter(bin2) - resEkin2_X->GetBinCenter(bin1);
  fwhmtext->Clear();
  fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
  fwhmtext->Draw("same");
  plotname = plotsdir+"/resEkin2_x";
  c_resEkin2x->SaveAs(plotname+".pdf");
  c_resEkin2x->SaveAs(plotname+".C");
  delete c_resEkin2x;

  TCanvas *c_resEkin2y = new TCanvas("resEkin2y", "resEkin2_y");
  resEkin2_Y->SetMarkerStyle(7);;
  resEkin2_Y->SetFillColor(3);
  resEkin2_Y->Draw();
  bin1 = resEkin_Y->FindFirstBinAbove(resEkin_Y->GetMaximum()/2.1);
  bin2 = resEkin_Y->FindLastBinAbove(resEkin_Y->GetMaximum()/2.1);
  fwhm = resEkin_Y->GetBinCenter(bin2) - resEkin_Y->GetBinCenter(bin1);
  fwhmtext->Clear();
  fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
  fwhmtext->Draw("same");
  plotname = plotsdir+"/resEkin2_y";
  c_resEkin2y->SaveAs(plotname+".pdf");
  c_resEkin2y->SaveAs(plotname+".C");
  delete c_resEkin2y;

  TCanvas *c_resLix = new TCanvas("resLix", "resLi_x");
  resLi_X->SetMarkerStyle(2);;
  resLi_X->SetFillColor(11);
  resLi_X->Draw();
  bin1 = resLi_X->FindFirstBinAbove(resLi_X->GetMaximum()/2.1);
  bin2 = resLi_X->FindLastBinAbove(resLi_X->GetMaximum()/2.1);
  fwhm = resLi_X->GetBinCenter(bin2) - resLi_X->GetBinCenter(bin1);
  fwhmtext->Clear();
  fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
  fwhmtext->Draw("same");
  plotname = plotsdir+"/resolution_LiModel_x";
  c_resLix->SaveAs(plotname+".pdf");
  c_resLix->SaveAs(plotname+".C");
  delete c_resLix;

  TCanvas *c_resLiy = new TCanvas("resLiy", "resLi_y");
  resLi_Y->SetMarkerStyle(7);;
  resLi_Y->SetFillColor(3);
  resLi_Y->Draw();
  bin1 = resLi_Y->FindFirstBinAbove(resLi_Y->GetMaximum()/2.1);
  bin2 = resLi_Y->FindLastBinAbove(resLi_Y->GetMaximum()/2.1);
  fwhm = resLi_Y->GetBinCenter(bin2) - resLi_Y->GetBinCenter(bin1);
  fwhmtext->Clear();
  fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
  fwhmtext->Draw("same");
  plotname = plotsdir+"/resolution_LiModel_y";
  c_resLiy->SaveAs(plotname+".pdf");
  c_resLiy->SaveAs(plotname+".C");
  delete c_resLiy;

  TCanvas *c_resLiz = new TCanvas("resLiz", "resLi_z");
  resLi_Z->SetMarkerStyle(7);;
  resLi_Z->SetFillColor(4);
  resLi_Z->Draw();
  bin1 = resLi_Z->FindFirstBinAbove(resLi_Z->GetMaximum()/2.1);
  bin2 = resLi_Z->FindLastBinAbove(resLi_Z->GetMaximum()/2.1);
  fwhm = resLi_Z->GetBinCenter(bin2) - resLi_Z->GetBinCenter(bin1);
  fwhmtext->Clear();
  fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
  fwhmtext->Draw("same");
  plotname = plotsdir+"/resolution_LiModel_z";
  c_resLiz->SaveAs(plotname+".pdf");
  c_resLiz->SaveAs(plotname+".C");
  delete c_resLiz;

  TCanvas *c_countx = new TCanvas("countx", "count_x");
  //c_countx->SetLogy(true);
  count_X->Draw();
  plotname = plotsdir+"/countx";
  c_countx->SaveAs(plotname+".pdf");
  c_countx->SaveAs(plotname+".C");
  delete c_countx;

  TCanvas *c_pixel_ene = new TCanvas("pixel_ene", "pixel_energy");
  //c_pixel_ene->SetLogy(true);
  pixel_energy->Draw();
  plotname = plotsdir+"/pixel_energy";
  c_pixel_ene->SaveAs(plotname+".pdf");
  c_pixel_ene->SaveAs(plotname+".C");
  delete c_pixel_ene;

  TCanvas *c_total_ene = new TCanvas("total_ene", "total_energy");
  //c_total_ene->SetLogy(true);
  total_energy->Draw();
  plotname = plotsdir+"/total_energy";
  c_total_ene->SaveAs(plotname+".pdf");
  c_total_ene->SaveAs(plotname+".C");
  delete c_total_ene;

  TCanvas *c_deposited_ene = new TCanvas("deposited_ene", "deposited_energy");
  //c_deposited_ene->SetLogy(true);
  deposited_energy->Draw();
  plotname = plotsdir+"/deposited_energy";
  c_deposited_ene->SaveAs(plotname+".pdf");
  c_deposited_ene->SaveAs(plotname+".C");
  delete c_deposited_ene;

  TCanvas *c_lowene = new TCanvas("lowene", "low_en");
  //c_lowene->SetLogy(true);
  low_en_avg->Draw("hist");
  //plotname = plotsdir+"/lowene";
  plotname = plotsdir+"/low_energy_avg";
  c_lowene->SaveAs(plotname+".pdf");
  c_lowene->SaveAs(plotname+".C");
  delete c_lowene;

  if (useSideMatrix) {

    TCanvas *c_tau2 = new TCanvas("tau2", "tau2");
    //c_tau2->SetLogy(true);
    h_tau2->Draw("hist");
    plotname = plotsdir+"/tau2";
    c_tau2->SaveAs(plotname+".pdf");
    c_tau2->SaveAs(plotname+".C");
    delete c_tau2;

    TCanvas *c_rec_yz = new TCanvas("crec_yz", "rec_yz");
    c_rec_yz->SetLogz(false);
    h_rec_yz->Draw("colz");
    plotname = plotsdir+"/reconstructed_yz";
    c_rec_yz->SaveAs(plotname+".pdf");
    c_rec_yz->SaveAs(plotname+".C");
    delete c_rec_yz;

    TCanvas *c_recI2_yz = new TCanvas("crecI2_yz", "recI2_yz");
    c_recI2_yz->SetLogz(false);
    h_recI2_yz->Draw("colz");
    plotname = plotsdir+"/reconstructed_I2_yz";
    c_recI2_yz->SaveAs(plotname+".pdf");
    c_recI2_yz->SaveAs(plotname+".C");
    delete c_recI2_yz;

    TCanvas *c_recEkin_yz = new TCanvas("crecEkin_yz", "recEkin_yz");
    c_recEkin_yz->SetLogz(false);
    h_recEkin_yz->Draw("colz");
    plotname = plotsdir+"/reconstructed_Ekin_yz";
    c_recEkin_yz->SaveAs(plotname+".pdf");
    c_recEkin_yz->SaveAs(plotname+".C");
    delete c_recEkin_yz;

    TCanvas *c_recEkin2_yz = new TCanvas("crecEkin2_yz", "recEkin2_yz");
    c_recEkin2_yz->SetLogz(false);
    h_recEkin2_yz->Draw("colz");
    plotname = plotsdir+"/reconstructed_Ekin2_yz";
    c_recEkin2_yz->SaveAs(plotname+".pdf");
    c_recEkin2_yz->SaveAs(plotname+".C");
    delete c_recEkin2_yz;

    TCanvas *c_resEkinz = new TCanvas("resEkinz", "resEkin_z");
    resEkin_Z->SetMarkerStyle(7);;
    resEkin_Z->SetFillColor(4);
    resEkin_Z->Draw();
    bin1 = resEkin_Z->FindFirstBinAbove(resEkin_Z->GetMaximum()/2.1);
    bin2 = resEkin_Z->FindLastBinAbove(resEkin_Z->GetMaximum()/2.1);
    fwhm = resEkin_Z->GetBinCenter(bin2) - resEkin_Z->GetBinCenter(bin1);
    fwhmtext->Clear();
    fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
    fwhmtext->Draw("same");
    plotname = plotsdir+"/resEkin_z";
    c_resEkinz->SaveAs(plotname+".pdf");
    c_resEkinz->SaveAs(plotname+".C");
    delete c_resEkinz;

    TCanvas *c_resEkin2z = new TCanvas("resEkin2z", "resEkin2_z");
    resEkin2_Z->SetMarkerStyle(7);;
    resEkin2_Z->SetFillColor(4);
    resEkin2_Z->Draw();
    bin1 = resEkin_Z->FindFirstBinAbove(resEkin_Z->GetMaximum()/2.1);
    bin2 = resEkin_Z->FindLastBinAbove(resEkin_Z->GetMaximum()/2.1);
    fwhm = resEkin_Z->GetBinCenter(bin2) - resEkin_Z->GetBinCenter(bin1);
    fwhmtext->Clear();
    fwhmtext->AddText(Form("FWHM = %.1f",fwhm));
    fwhmtext->Draw("same");
    plotname = plotsdir+"/resEkin2_z";
    c_resEkin2z->SaveAs(plotname+".pdf");
    c_resEkin2z->SaveAs(plotname+".C");
    delete c_resEkin2z;

    TCanvas *c_county = new TCanvas("county", "count_y");
    //c_county->SetLogy(true);
    count_Y->Draw();
    plotname = plotsdir+"/county";
    c_county->SaveAs(plotname+".pdf");
    c_county->SaveAs(plotname+".C");
    delete c_county;

    TCanvas *c_countz = new TCanvas("countz", "count_z");
    //c_countz->SetLogy(true);
    count_Z->Draw();
    plotname = plotsdir+"/countz";
    c_countz->SaveAs(plotname+".pdf");
    c_countz->SaveAs(plotname+".C");
    delete c_countz;

    TCanvas *c_cost_LS1 = new TCanvas("costLS1", "truth_cost_LS1");
    //c_cost_LS1->SetLogy(true);
    truth_cost_LS1->Draw("hist");
    plotname = plotsdir+"/truth_cost_LS1";
    c_cost_LS1->SaveAs(plotname+".pdf");
    c_cost_LS1->SaveAs(plotname+".C");
    delete c_cost_LS1;

    TCanvas *c_cost_LS2 = new TCanvas("costLS2", "truth_cost_LS2");
    //c_cost_LS2->SetLogy(true);
    truth_cost_LS2->Draw("hist");
    plotname = plotsdir+"/truth_cost_LS2";
    c_cost_LS2->SaveAs(plotname+".pdf");
    c_cost_LS2->SaveAs(plotname+".C");
    delete c_cost_LS2;//*/

  }

  //save histos to root file
  TFile *fout = new TFile(plotsdir+"/histos.root", "recreate");
  fout->cd();
  h_rec->Write();
  h_recI2->Write();
  h_recLi->Write();
  h_recLi_xz->Write();
  h_recLi_yz->Write();
  h_recLi_xyz->Write();
  resLi_X->Write();
  resLi_Y->Write();
  resLi_Z->Write();
  count_X->Write();
  pixel_energy->Write();
  fout->Close();

  //} // end gun loop

} //end main function

