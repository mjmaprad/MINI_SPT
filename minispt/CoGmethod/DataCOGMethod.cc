
#include <TTree.h>

#include "LiModel.h"

void DataCOGMethod(TString jobname = "Na22source") {

  //gStyle->SetPalette(kGreyScale); //kLightTemperature //kDeepSea //kCool //kViridis //kGreyScale //kInvertedDarkBodyRadiator
  //TColor::InvertPalette();

  vector<TString> FileString = {""}; //
  if (jobname=="subtraction") FileString = {"Na22source", "noSource"};
  else if (jobname!="") FileString = {jobname};
  TFile *f;
  TTree *tree;
  Int_t Nentries;

  //position of primary particle hit (in millimeters)
  //Double_t scin_x=0, scin_y=0, scin_z=0;
  //Double_t energy_depo=0;
  Float_t timeus=0;

  Double_t rec_X = 0, rec_Y = 0, rec_Z = 0, weight=1;
  Double_t recI2_X = 0, recI2_Y = 0, recI2_Z = 0;
  //Double_t recEkin_X = 0, recEkin_Y = 0, recEkin_Z = 0;
  //Double_t recEkin2_X = 0, recEkin2_Y = 0, recEkin2_Z = 0;

  Double_t Energy_x_sorted[64], Energy_z_sorted[64];
  Double_t max_x, max_y, max_y_x, max_y_z, max_z;
  Double_t start_x=0, start_y=0, start_z=0;
  Double_t low_en_avg_1, low_en_avg_2;
  Double_t pixel_enesf[NsipmBot] = { 1102./1028, 1102./1064, 1102./1071, 1102./1020, 1102./1088, 1102./1101, 1102./1096, 1102./1062, 1102./1077, 1102./1102, 1102./1076, 1102./1037, 1102./1068, 1102./1056, 1102./1050, 1102./1004,  };

  TH2D *h_event_xy;
  TH2D *h_event_xz;
  TH2D *h_event_yz;

  TH2D *h_rec;
  TH2D *h_recI2;
  //TH2D *h_recEkin;
  //TH2D *h_recEkin2;
  TH2D *h_recLi;

  TH2D *h_rec_xz;
  TH2D *h_recI2_xz;
  //TH2D *h_recEkin_xz;
  //TH2D *h_recEkin2_xz;
  TH2D *h_recLi_xz;

  TH2D *h_rec_yz;
  TH2D *h_recI2_yz;
  //TH2D *h_recEkin_yz;
  //TH2D *h_recEkin2_yz;
  TH2D *h_recLi_yz;
  TH3D *h_recLi_xyz;

  TH1D *count_X;
  //TH1D *count_Y;
  //TH1D *count_Z;

  TH1D *pixel_energy;
  TH1D *pixel_ene[NsipmBot];
  TH1D *total_energy;
  TH1D *low_en_avg;
  TH1D *timestamp;

  //Li Model
  //TH1D *h_photonNum;
  //TH1D *h_Mj;
  //TH1D *truth_cost_LS;
  //TH1D *truth_cost_LS1;
  //TH1D *truth_cost_LS2;
  TH1D *LiFit_cost_LS;
  TH1D *LiFit_cost_LL;
  TH1D *h_A0;
  TH1D *h_tau1;
  TH1D *h_tau2;

  TMinuit *LiModelMN = new TMinuit(LiModelParN);  //initialize TMinuit with its parameters number
  LiModelMN->SetFCN(LiModelLS);

  Int_t c=0;

  //for (Int_t en = 0; en < 1; en++) {
  for (TString filestring : FileString) {

    if (jobname=="subtraction") {
      if (c==0) cout<<"Using subtraction method"<<endl;
      weight*=TMath::Power(-1.,c);
      c++;
    }
    cout<<"Processing "<<filestring<<" data"<<endl;
    //cout<<"weight = "<<weight<<endl; continue;

    TString filename = "DataFiles/fers_bc408cube26mm_"+filestring+"_5min.root";
    //cout<<"FF: "<<gSystem->AccessPathName(filename,kFileExists)<<endl;
    if (gSystem->AccessPathName(filename,kFileExists)) {cout<<"ERROR! File "<<filename<<" not found!"<<endl; return;}
    f = new TFile(filename);
    tree = (TTree *) f->Get("thedata");
    Nentries = tree->GetEntries();
    //cout<<"Nentries = "<< Nentries << endl; //getchar();
    int step = 100; if (Nentries>10000) step = 300; if (Nentries>30000) step = 1000; if (Nentries>100000) step = 3000; if (Nentries>300000) step = 10000; if (Nentries>1000000) step = Nentries/100;
    //cout<<"step = "<< step << endl; getchar();

     if (weight>0) {
      h_rec = new TH2D("rec", "Anger Logic reconstruction; X [mm]; Y [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
      if (useSideMatrix) h_rec_xz = new TH2D("recxz", "Anger Logic reconstruction; X [mm]; Z [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);
      if (useSideMatrix) h_rec_yz = new TH2D("recyz", "Anger Logic reconstruction; Y [mm]; Z [mm]", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

      h_recI2 = new TH2D("recI2", "Square Charge reconstruction; X [mm]; Y [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
      if (useSideMatrix) h_recI2_xz = new TH2D("recI2xz", "Square Charge reconstruction; X [mm]; Z [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);
      if (useSideMatrix) h_recI2_yz = new TH2D("recI2yz", "Square Charge reconstruction; Y [mm]; Z [mm]", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);

      h_recLi = new TH2D("recLi", "Li Model reconstruction; X [mm]; Y [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_y_halfwidth, cristal_y_halfwidth);
      h_recLi_xz = new TH2D("recLixz", "Li Model reconstruction; X [mm]; Z [mm]", 100, -cristal_x_halfwidth, cristal_x_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);
      h_recLi_yz = new TH2D("recLiyz", "Li Model reconstruction; Y [mm]; Z [mm]", 100, -cristal_y_halfwidth, cristal_y_halfwidth, 100, -cristal_z_halfwidth, cristal_z_halfwidth);
      h_recLi_xyz = new TH3D("recLixyz", "Li Model reconstruction; X [mm]; Y [mm]; Z [mm]",
			    40, -cristal_x_halfwidth, cristal_x_halfwidth,
			    40, -cristal_y_halfwidth, cristal_y_halfwidth,
			    40, -cristal_z_halfwidth, cristal_z_halfwidth
			   );

      count_X = new TH1D("TotalADC", "Total ADC", 200, 0, 500000);
      //count_Y = new TH1D("", "", 400, 0, 40000);
      //count_Z = new TH1D("", "", 400, 0, 40000);
      pixel_energy = new TH1D("PixelADC", "Pixel ADC", 400, 0, 2800);
      low_en_avg = new TH1D("LowADCavg", "Low ADC Avg", 100, 0, 2000);
      for (Int_t i_sipm = 0; i_sipm < NsipmBot; i_sipm++) pixel_ene[i_sipm] = new TH1D(Form("pixel%dadc",i_sipm), Form("Pixel %d ADC",i_sipm), 200, 0, 8000);
      timestamp = new TH1D("timestamp", "Time Stamp; t [#mus]; [Events/(2 s)]", 200, 0, 400e6);

      //h_photonNum = new TH1D("", "", 200, 0, 0.2);
      //h_Mj = new TH1D("", "", 200, 0, 0.2);
      //truth_cost_LS = new TH1D("", "", 200, 0, 2);
      //truth_cost_LS1 = new TH1D("", "", 200, 0, 2);
      //truth_cost_LS2 = new TH1D("", "", 200, 0, 2);
      LiFit_cost_LS = new TH1D("cost_LS", "", 200, 0, 0.004);
      LiFit_cost_LL = new TH1D("cost_LL", "", 200, 5, 7);

      h_A0 = new TH1D("A0", "A0", 100, 0, 1.5);
      h_tau1 = new TH1D("tau1", "tau1", 100, 0, 0.02);
      h_tau2 = new TH1D("tau2", "tau2", 100, 0, 0.02);

      for (int i=0; i<BotSiPMlenght; i++)   bot_sipm_xpos[i] =  bot_sipm_ypos[i] = 3.2 * (i -  BotSiPMlenght/2. + 0.5);
      if (useSideMatrix && SideSiPMlenght==0) useSideMatrix=false;
      for (int i=0; i<SideSiPMlenght; i++) side_sipm_ypos[i] = side_sipm_zpos[i] = 3.2 * (i - SideSiPMlenght/2. + 0.5);

    }

    for (Int_t i_sipm = 0; i_sipm < NsipmBot+NsipmSide; i_sipm++) {
      if (i_sipm < NsipmBot) {
	tree->SetBranchAddress(Form("CH%02d_HG",pixel_map[i_sipm]), &ekin[i_sipm]);
	//cout<<"i_sipm = "<<i_sipm<<" -> CH"<<pixel_map[i_sipm]<<" , bot_sipm_xpos = "<<bot_sipm_xpos[i_sipm/BotSiPMlenght]<<" , bot_sipm_ypos = "<<bot_sipm_ypos[i_sipm%BotSiPMlenght]<<endl;
      }
      else if (useSideMatrix){
	Int_t j_sipm = i_sipm - NsipmBot;
	//tree->SetBranchAddress(Form("CH%d_HG",j_sipm+32), &ekin[i_sipm]);
	//cout<<"i_sipm = "<<i_sipm<<" , side_sipm_ypos = "<<side_sipm_ypos[j_sipm%SideSiPMlenght]<<" , side_sipm_zpos = "<<side_sipm_zpos[j_sipm/SideSiPMlenght]<<endl;
      }//*/
    }
    //return;

    tree->SetBranchAddress("TimeStamp_us", &timeus);

    for (Int_t i = 0; i < Nentries; i++) {

      if (i % step == 0) cout<<"Processing event "<<i<<" of "<<Nentries<<endl; //<<"timestamp = "<<timeus<<endl;

      tree->GetEntry(i);
      //cout<<"Processing entry "<<i<<endl; getchar();
      if (timeus > 340e6) continue;
	  
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
      int cxy=0, cz=0;
      int i_maxEn_x=0;
      int i_maxEn_z=NsipmBot;

      for (int i_sipm = 0; i_sipm < NsipmBot+NsipmSide; i_sipm++) {
	if (ekin[i_sipm] == 0) continue;
	//ekin[i_sipm]*=pixel_enesf[i_sipm];
	if (ekin[i_sipm] > 60000) ekin[i_sipm] = 0;
	if (ekin[i_sipm] > 8000) ekin[i_sipm] = 8000;
	pixel_energy->Fill(ekin[i_sipm], weight);
	if (i_sipm < NsipmBot) {
	  if (ekin[i_sipm] != 0) cxy++;
	  pixel_ene[i_sipm]->Fill(ekin[i_sipm], weight);
	  Energy_x_sorted[i_sipm]=ekin[i_sipm];
	  if (ekin[i_sipm]>ekin[i_maxEn_x]) i_maxEn_x = i_sipm;
	  ph_tot_x += ekin[i_sipm];
	  ph_tot_y += ekin[i_sipm];
	  rec_X += bot_sipm_xpos[i_sipm/BotSiPMlenght] * ekin[i_sipm];
	  rec_Y += bot_sipm_ypos[i_sipm%BotSiPMlenght] * ekin[i_sipm];
	  //cout<<"DEBUG: i_sipm = "<<i_sipm<<" , ADC count = "<<ekin[i_sipm]<<" , ph_tot = "<<ph_tot_y<<" , rec_X = "<<rec_X<<" , rec_Y = "<<rec_Y<<endl;
	  phI2_tot_x += (ekin[i_sipm] * ekin[i_sipm]);
	  phI2_tot_y += (ekin[i_sipm] * ekin[i_sipm]);
	  recI2_X += bot_sipm_xpos[i_sipm/BotSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);
	  recI2_Y += bot_sipm_ypos[i_sipm%BotSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);
	} else {
	  if (useSideMatrix) {
	    int j_sipm = i_sipm-NsipmBot;
	    if (ekin[i_sipm] != 0) cz++;
	    Energy_z_sorted[j_sipm]=ekin[i_sipm];
	    if (ekin[i_sipm]>ekin[i_maxEn_z]) i_maxEn_z = i_sipm;
	    ph_tot_z += ekin[i_sipm];
	    ph_tot_y += ekin[i_sipm];
	    rec_Z += side_sipm_zpos[j_sipm%SideSiPMlenght] * ekin[i_sipm];
	    rec_Y += side_sipm_ypos[j_sipm/SideSiPMlenght] * ekin[i_sipm];
	    //cout<<"DEBUG: j_sipm = "<<j_sipm<<" , ADC count = "<<ekin[i_sipm]<<" , ph_tot = "<<ph_tot<<" , rec_z = "<<rec_z<<" , rec_Y = "<<rec_Y<<endl;
	    phI2_tot_z += (ekin[i_sipm] * ekin[i_sipm]);
	    phI2_tot_y += (ekin[i_sipm] * ekin[i_sipm]);
	    recI2_Z += side_sipm_zpos[j_sipm%SideSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);
	    recI2_Y += side_sipm_ypos[j_sipm/SideSiPMlenght] * (ekin[i_sipm] * ekin[i_sipm]);
	  }
	}
      }

      //if (cxy < 1 || (useSideMatrix && cz < 1) ) continue;
      //if ( ph_tot_x > 18e3 || ph_tot_x < 14e3 ) continue;

      rec_X = rec_X / ph_tot_x;
      rec_Y = rec_Y / ph_tot_y;
      rec_Z = (useSideMatrix) ? rec_Z / ph_tot_z : 0;
      //cout<<"DEBUG: Results: "<<" rec_X = "<<rec_X<<" , rec_Y = "<<rec_Y<<endl; //getchar();

      recI2_X = recI2_X / phI2_tot_x;
      recI2_Y = recI2_Y / phI2_tot_y;
      recI2_Z = (useSideMatrix) ? recI2_Z / phI2_tot_z : 0; //

      // still needed for LiModel fcn:
      Ekin_tot_x = ph_tot_x;
      Ekin_tot_y = ph_tot_y;
      Ekin_tot_z = ph_tot_z;
      //total_energy->Fill(Ekin_tot_y);

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
	  if (useSideMatrix) if (Energy_z_sorted[j]>Energy_z_sorted[k]) {
	    temp = Energy_z_sorted[j];
	    Energy_z_sorted[j] = Energy_z_sorted[k];
	    Energy_z_sorted[k] = temp;
	  }
      }

      //for (int i=0; i<NsipmBot; i++) cout<<Energy_x_sorted[i]<<endl; //getchar();

      Int_t N_low_bot = NsipmBot/5;
      low_en_avg_1 = 0;
      for (int j=0; j<N_low_bot; j++) low_en_avg_1 += Energy_x_sorted[j];
      low_en_avg_1 /= (ph_tot_x*N_low_bot);
      low_en_avg_1 = max(0.0001,low_en_avg_1);
      if (useSideMatrix) {
	Int_t N_low_side = NsipmSide/4;
	low_en_avg_2 = 0;
	for (int j=0; j<N_low_side; j++) low_en_avg_2 += Energy_z_sorted[j];
	low_en_avg_2 /= ph_tot_z; 
	low_en_avg_2 = max(0.0001,low_en_avg_2);
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
      start_x = recI2_X; start_y = recI2_Y; start_z = recI2_Z; //
      Double_t vstart[6]     = {  start_x,  start_y,  start_z,  0.8, low_en_avg_1, low_en_avg_2 };

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
      ierflg = LiModelMN->Migrad(); // run migrad minimization with default settings (maxcalls, tolerance, etc...)
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
      Double_t li_x, li_y, li_z, li_A0, li_tau1, li_tau2 = 0;
      Double_t li_x_err, li_y_err, li_z_err, li_A0_err, li_tau1_err, li_tau2_err;
      LiModelMN->GetParameter(0, li_x, li_x_err);
      LiModelMN->GetParameter(1, li_y, li_y_err);
      LiModelMN->GetParameter(2, li_z, li_z_err);
      LiModelMN->GetParameter(3, li_A0, li_A0_err);
      LiModelMN->GetParameter(4, li_tau1, li_tau1_err);
      if (useSideMatrix) LiModelMN->GetParameter(5, li_tau2, li_tau2_err);

      //if (i<4) continue;
      /*cout<<endl
	  <<"Event "<<i<<endl
	  <<"Max sipm positions:           max_x      = "<<max_x     <<" , max_y_x    = "<<max_y_x   <<" , max_y_z    = "<<max_y_z   <<" , max_y      = "<<max_y     <<" , max_z      = "<<max_z
	  <<" , i_maxEn_x = "<<i_maxEn_x<<" , i_maxEn_z = "<<i_maxEn_z<<endl
	  <<"Energy:                       tot_energy = "<<ph_tot_x  <<" , low_en_avg = "<<low_en_avg_1*ph_tot_x<<" , start_tau1 = "<<low_en_avg_1<<endl
	//<<"Anger Logic count results:    rec_X      = "<<rec_X     <<" , rec_Y      = "<<rec_Y     <<" , rec_Z      = "<<rec_Z<<endl
	  <<"Square Charge count results:  recI2_X    = "<<recI2_X   <<" , recI2_Y    = "<<recI2_Y   <<" , recI2_Z    = "<<recI2_Z<<endl
	//<<"Anger Logic energy results:   recEkin_X  = "<<recEkin_X <<" , recEkin_Y  = "<<recEkin_Y <<" , recEkin_Z  = "<<recEkin_Z<<endl
	//<<"Square Charge energy results: recEkin2_X = "<<recEkin2_X<<" , recEkin2_Y = "<<recEkin2_Y<<" , recEkin2_Z = "<<recEkin2_Z<<endl
	  <<"Li Model fit results:         li_x       = "<<li_x      <<" , li_y       = "<<li_y      <<" , li_z       = "<<li_z<<endl
	  <<"                              li_A0      = "<<li_A0     <<" , li_tau1    = "<<li_tau1   <<" , li_tau2    = "<<li_tau2<<endl
	//<<"Li Model fit outputs:         amin       = "<<amin      <<" , edm        = "<<edm       <<" , icstat     = "<<icstat<<endl
	//<<"Truth scintillation position: scin_x     = "<<scin_x    <<" , scin_y     = "<<scin_y    <<" , scin_z     = "<<scin_z<<endl
	  <<endl;
      //if ( abs(li_z) < 0.1 )
      getchar();//*/

      //draw event distributions
      /*h_event_xy = new TH2D("", "", BotSiPMlenght, 0, BotSiPMlenght, BotSiPMlenght, 0, BotSiPMlenght);
      if (useSideMatrix) h_event_yz = new TH2D("", "", SideSiPMlenght, 0, SideSiPMlenght, SideSiPMlenght, 0, SideSiPMlenght);
      for (int i_sipm=0; i_sipm<NsipmBot; i_sipm++) {
	h_event_xy->SetBinContent(1+i_sipm/BotSiPMlenght, 1+i_sipm%BotSiPMlenght, ekin[i_sipm]);
	if (useSideMatrix) h_event_yz->SetBinContent(1+i_sipm/SideSiPMlenght, 1+i_sipm%SideSiPMlenght, ekin[NsipmBot+i_sipm]);
      }

      TCanvas *c_event_xy = new TCanvas("cevent_xy", "event_xy");
      h_event_xy->SetStats(false);
      h_event_xy->Draw("text");
      //gPad->Modified(); gPad->Update();
      if (useSideMatrix) {
	TCanvas *c_event_yz = new TCanvas("cevent_yz", "event_yz");
	h_event_yz->SetStats(false);
	h_event_yz->Draw("colz");
	//gPad->Modified(); gPad->Update();
      }
      return;
      TString evtplotsdir = "Plots/Lyso12mmCube/DataOneArray/LL/"+filestring+"/evtplots/";
      system("mkdir -p "+evtplotsdir);
      c_event_xy->SaveAs(evtplotsdir+Form("%d.pdf",i));
      delete c_event_xy;
      //if (useSideMatrix) delete c_event_yz;
      if (i>200) return;//*/

      ////fill histos
      count_X->Fill(ph_tot_x, weight);
      low_en_avg->Fill(low_en_avg_1*ph_tot_x, weight);
      //count_Y->Fill(ph_tot_y);
      //count_Z->Fill(ph_tot_z);
      timestamp->Fill(timeus, weight);

      h_rec->Fill(rec_X, rec_Y, weight);
      if (useSideMatrix) h_rec_yz->Fill(rec_Y, rec_Z, weight);

      h_recI2->Fill(recI2_X, recI2_Y, weight);
      if (useSideMatrix) h_recI2_yz->Fill(recI2_Y, recI2_Z, weight);

      //if ( li_tau1 > 0.038 ) continue;
      //if ( amin < 0.00075 ) continue;

      Bool_t li_limits_ok = true;

      Double_t v_limit = cristal_x_halfwidth-0.3; //-1;
      if (li_x < -v_limit) li_limits_ok = false; //continue; //li_x = -cristal_x_halfwidth;
      if (li_x > v_limit)  li_limits_ok = false; //continue; //li_x = cristal_x_halfwidth;

      v_limit = cristal_y_halfwidth-0.3;
      if (li_y < -v_limit) li_limits_ok = false; //continue; //li_y = -cristal_y_halfwidth;
      if (li_y > v_limit)  li_limits_ok = false; //continue; //li_y = cristal_y_halfwidth;

      v_limit = cristal_z_halfwidth-0.3;
      if (li_z > v_limit)  li_limits_ok = false; //continue; //li_z = cristal_z_halfwidth;
      v_limit = cristal_z_halfwidth-1;
      if (li_z < -v_limit) li_limits_ok = false; //continue; //li_z = -cristal_z_halfwidth;

      if ( abs(li_z-0.1) < 0.1 )  li_limits_ok = false;
      //if (li_z < +4) li_limits_ok = false; //continue; //li_z = -cristal_z_halfwidth;
      //if ( abs(li_x-4.75) > 0.25 ) li_limits_ok = false;
      //if ( abs(li_y-4.70) > 0.25 ) li_limits_ok = false;
      //if ( abs(li_y-4.70) < 0.25 && abs(li_x-4.75) < 0.25 ) li_limits_ok = false; //*/
      if ( li_A0 < 0.05 )  li_limits_ok = false;  

      if (li_limits_ok) {
	h_recLi->Fill(li_x, li_y, weight);
	h_recLi_xz->Fill(li_x, li_z, weight);
	h_recLi_yz->Fill(li_y, li_z, weight);
	h_recLi_xyz->Fill(li_x, li_y, li_z, weight);
	LiFit_cost_LS->Fill(amin, weight);
	LiFit_cost_LL->Fill(amin, weight);
	h_A0->Fill(li_A0, weight);
	h_tau1->Fill(li_tau1, weight);
	if (useSideMatrix) h_tau2->Fill(li_tau2, weight);
      }

    } // end event loop 

    cout<<"End of event loop"<<endl;

    if (jobname=="subtraction"){
      if (weight>0) continue;
      else filestring=jobname;
    }

    TString sArray = "OneArray"; if (useSideMatrix) sArray = "TwoArray";
    TString plotsdir = "Plots/BC408-26mmCube/Data"+sArray+"/HG/LS-withRefl-A0Cut-v3/"+filestring;
    system("mkdir -p "+plotsdir);
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
    delete c_tau1;//*/

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

    TCanvas *c_countx = new TCanvas("countx", "count_x");
    //c_countx->SetLogy(true);
    count_X->Draw("hist");
    //plotname = plotsdir+"/countx";
    plotname = plotsdir+"/total_energy";
    c_countx->SaveAs(plotname+".pdf");
    c_countx->SaveAs(plotname+".C");
    delete c_countx;

    //TCanvas *c_county = new TCanvas("county", "count_y");
    //c_county->SetLogy(true);
    //count_Y->Draw();
    //plotname = plotsdir+"/county";
    //c_county->SaveAs(plotname+".pdf");
    //c_county->SaveAs(plotname+".C");
    //delete c_county;

    //TCanvas *c_countz = new TCanvas("countz", "count_z");
    //c_countz->SetLogy(true);
    //count_Z->Draw();
    //plotname = plotsdir+"/countz";
    //c_countz->SaveAs(plotname+".pdf");
    //c_countz->SaveAs(plotname+".C");
    //delete c_countz;

    TCanvas *c_lowene = new TCanvas("lowene", "low_en");
    //c_lowene->SetLogy(true);
    low_en_avg->Draw("hist");
    //plotname = plotsdir+"/lowene";
    plotname = plotsdir+"/low_energy_avg";
    c_lowene->SaveAs(plotname+".pdf");
    c_lowene->SaveAs(plotname+".C");
    delete c_lowene;

    TCanvas *c_pixel_ene = new TCanvas("pixel_ene", "pixel_energy");
    //c_pixel_ene->SetLogy(true);
    pixel_energy->Draw("hist");
    plotname = plotsdir+"/pixel_energy";
    c_pixel_ene->SaveAs(plotname+".pdf");
    c_pixel_ene->SaveAs(plotname+".C");
    /*system("mkdir -p "+plotsdir+"/pixels/");
    for (int i_sipm = 0; i_sipm < NsipmBot; i_sipm++) {
      c_pixel_ene = new TCanvas("pixel_ene", "pixel_energy");
      //c_pixel_ene->SetLogy(true);
      pixel_ene[i_sipm]->Draw("hist");
      plotname = plotsdir+Form("/pixels/energy%d",i_sipm);
      c_pixel_ene->SaveAs(plotname+".pdf");
      c_pixel_ene->SaveAs(plotname+".C");
    }//*/
    delete c_pixel_ene;

    TCanvas *c_timestamp = new TCanvas("timestamp", "time_stamp");
    //c_timestamp->SetLogy(true);
    timestamp->Draw("hist");
    plotname = plotsdir+"/timestamp";
    c_timestamp->SaveAs(plotname+".pdf");
    c_timestamp->SaveAs(plotname+".C");
    delete c_timestamp;

    if (useSideMatrix) {
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

      TCanvas *c_tau2 = new TCanvas("tau2", "tau2");
      //c_tau2->SetLogy(true);
      h_tau2->Draw("hist");
      plotname = plotsdir+"/tau2";
      c_tau2->SaveAs(plotname+".pdf");
      c_tau2->SaveAs(plotname+".C");
      delete c_tau2;
    }

    TFile *fout = new TFile(plotsdir+"/plots.root", "recreate");
    fout->cd();
    h_rec->Write();
    h_recI2->Write();
    h_recLi->Write();
    h_recLi_xz->Write();
    h_recLi_yz->Write();
    h_recLi_xyz->Write();
    count_X->Write();
    pixel_energy->Write();
    fout->Close();
    
  } // end file loop

  
} //end main function
