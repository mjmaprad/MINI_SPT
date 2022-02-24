// global variables and options for CoG

#include "TMinuit.h"

void COGreco();
void COGrecoData();

Bool_t ExactAngle = false, Reflections = true;
Bool_t useSideMatrix = false;
Double_t mx, mz;
//Double_t sipm_xpos[8], sipm_ypos[8], sipm_zpos[8];
const Int_t BotSiPMlenght = 8, SideSiPMlenght = 1;  // only square sipm matrix !!!
const Int_t NsipmBot = BotSiPMlenght*BotSiPMlenght, NsipmSide = SideSiPMlenght*SideSiPMlenght;
Double_t  bot_sipm_xpos[BotSiPMlenght],   bot_sipm_ypos[BotSiPMlenght];
Double_t side_sipm_ypos[SideSiPMlenght], side_sipm_zpos[SideSiPMlenght];
Double_t ekin[128]; //kinetic energy
//Int_t ekin[128]; //FERS ADC counts
//Int_t pixel_map[NsipmBot] = { 45, 47, 46, 44, 43, 41, 40, 42, 37, 39, 38, 36, 35, 33, 32, 34, };
Int_t pixel_map[NsipmBot] = {
			     61, 63, 62, 60, 29, 31, 30, 28,
			     59, 57, 56, 58, 27, 25, 24, 26,
			     53, 55, 54, 52, 21, 23, 22, 20,
			     51, 49, 48, 50, 19, 17, 16, 18,
			     45, 47, 46, 44, 13, 15, 14, 12,
			     43, 41, 40, 42, 11,  9,  8, 10,
			     37, 39, 38, 36,  5,  7,  6,  4,
			     35, 33, 32, 34,  3,  1,  0,  2,
};
Int_t pcount[128]; //
Double_t ph_tot_x = 0, ph_tot_y = 0, ph_tot_z = 0;
Double_t phI2_tot_x = 0, phI2_tot_y = 0, phI2_tot_z = 0;
Double_t Ekin_tot_x = 0, Ekin_tot_y = 0, Ekin_tot_z = 0;
Double_t Ekin2_tot_x = 0, Ekin2_tot_y = 0, Ekin2_tot_z = 0;

Double_t cristal_x_halfwidth = 13, cristal_y_halfwidth = 13, cristal_z_halfwidth = 4;  //cristal dimensions in mm
Double_t sipm_x_width = 3, sipm_y_width = 3, sipm_z_width = 3;                          //sipm dimensions in mm

//Lyso-H12mm-gamma500keV-8x6y-200k
TString gunpos, part_energy, scint_material, z_height;

//gStyle->SetPalette(kGreyScale); //kLightTemperature //kDeepSea //kCool //kViridis //kGreyScale //kInvertedDarkBodyRadiator
//TColor::InvertPalette();

//Int_t energy[] = {500, 1000, 4000, 8000, 12000, 16000, 20000};
//TString GunPositions[] = {"0x0y","0x2y","0x4y","0xn2y","0xn4y", "2x0y","4x0y","n2x0y","n4x0y","n4xn4y","n2xn2y","2x2y","4x4y","n4x4y","n2x2y","2xn2y","4xn4y"}; //,"merge"};
//TString GunPositions[] = {"0x0y", "0x3y", "0x6y", "0x9y", "0x12y", "0xn3y", "0xn6y", "0xn9y", "0xn12y", "3x0y", "3x3y", "3x6y", "3x9y", "3x12y", "3xn3y", "3xn6y", "3xn9y", "3xn12y", "6x0y", "6x3y", "6x6y", "6x9y", "6x12y", "6xn3y", "6xn6y", "6xn9y", "6xn12y", "9x0y", "9x3y", "9x6y", "9x9y", "9x12y", "9xn3y", "9xn6y", "9xn9y", "9xn12y", "12x0y", "12x3y", "12x6y", "12x9y", "12x12y", "12xn3y", "12xn6y", "12xn9y", "12xn12y", "n3x0y", "n3x3y", "n3x6y", "n3x9y", "n3x12y", "n3xn3y", "n3xn6y", "n3xn9y", "n3xn12y", "n6x0y", "n6x3y", "n6x6y", "n6x9y", "n6x12y", "n6xn3y", "n6xn6y", "n6xn9y", "n6xn12y", "n9x0y", "n9x3y", "n9x6y", "n9x9y", "n9x12y", "n9xn3y", "n9xn6y", "n9xn9y", "n9xn12y", "n12x0y", "n12x3y", "n12x6y", "n12x9y", "n12x12y", "n12xn3y", "n12xn6y", "n12xn9y", "n12xn12y"}; // ,"merge"};
//TString GunPositions[] = {"iso20k"}; //"6x6y-20k","planar20k","merge"}; //0x0y-20k","6x6y-20k","n6x6y-20k","planar20k
//TString GunPositions[] = {"gamma500keV-8x6y-200k"}; //"gamma500keV-planar200k", "Na22-iso600k", ""
TFile *f;
TTree *tree;
Int_t Nentries;

//Double_t ele_x=0, ele_y=0, ele_z=0; //position of primary particle hit (in millimeters)
Double_t gun_x=0, gun_y=0, gun_z=0;
Double_t entr_x=0, entr_y=0, entr_z=0;
Double_t entr_phi=0, entr_theta=0;
Double_t exit_x=0, exit_y=0, exit_z=0;
Double_t exit_phi=0, exit_theta=0;
Double_t scin_x=0, scin_y=0, scin_z=0;
Double_t energy_depo=0;

//Double_t sipm_xpos[8];
//Double_t sipm_ypos[8];
//Double_t sipm_zpos[8];
//Double_t sipm_xypos[64][2];

//Double_t ph_totx = 0, ph_toty = 0;
//Double_t phI2_totx = 0, phI2_toty = 0;
//Double_t Ekin_totx = 0, Ekin_toty = 0;
//Double_t Ekin2_totx = 0, Ekin2_toty = 0;
//Double_t stdDevX = 0, stdDevY = 0;
//Double_t meanX = 0, meanY = 0;

Double_t rec_X = 0, rec_Y = 0, rec_Z = 0, weight;
Double_t recI2_X = 0, recI2_Y = 0, recI2_Z = 0;
Double_t recEkin_X = 0, recEkin_Y = 0, recEkin_Z = 0;
Double_t recEkin2_X = 0, recEkin2_Y = 0, recEkin2_Z = 0;

Double_t Energy_x_sorted[64], Energy_z_sorted[64];
Double_t max_x, max_y, max_y_x, max_y_z, max_z;
Double_t start_x=0, start_y=0, start_z=0;
Double_t low_en_avg_1, low_en_avg_2;
//Double_t pixel_enesf[NsipmBot] = { 9/7.29, 9/8.1, 9/8.1, 9/7.29,
//				     9/8.1 , 9/9  , 9/9  , 9/8.1 ,
//				     9/8.1 , 9/9  , 9/9  , 9/8.1 ,
//				     9/7.29, 9/8.1, 9/8.1, 9/7.29, };

TH2D *h_event_xy; // event display
TH2D *h_event_yz;

TH2D *h_gun;
TH2D *h_gun_yz;

TH2D *h_scin;
TH2D *h_scin_xz;
TH2D *h_scin_yz;

TH2D *h_entr;
TH2D *h_entr_yz;

TH2D *h_exit;
TH2D *h_exit_yz;

TH1D *count_X;
TH1D *count_Y;
TH1D *count_Z;

TH1D *pixel_energy;
TH1D *total_energy;
TH1D *deposited_energy;
TH1D *low_en_avg;

//TH1D *poisson;

TH2D *h_rec;
TH2D *h_rec_yz;
TH1D *res_X;
TH1D *res_Y;
TH1D *res_Z;

TH2D *h_recI2;
TH2D *h_recI2_yz;
TH1D *resI2_X;
TH1D *resI2_Y;
TH1D *resI2_Z;

TH2D *h_recEkin;
TH2D *h_recEkin_yz;
TH1D *resEkin_X;
TH1D *resEkin_Y;
TH1D *resEkin_Z;

TH2D *h_recEkin2;
TH2D *h_recEkin2_yz;
TH1D *resEkin2_X;
TH1D *resEkin2_Y;
TH1D *resEkin2_Z;

//Li Model
TH2D *h_recLi;
TH2D *h_recLi_yz;
TH2D *h_recLi_xz;
TH3D *h_recLi_xyz;
TH1D *resLi_X;
TH1D *resLi_Y;
TH1D *resLi_Z;
TH1D *h_A0;
TH1D *h_tau1;
TH1D *h_tau2;
TH1D *h_photonNum;
TH1D *h_Mj;
TH1D *truth_cost_LS;
TH1D *truth_cost_LS1;
TH1D *truth_cost_LS2;
TH1D *LiFit_cost_LS;
TH1D *LiFit_cost_LL;
