//definitions of methods needed by the Li Model

Double_t SolidAngle(Double_t sipm_x, Double_t sipm_y, Double_t sipm_z, Double_t scint_x, Double_t scint_y, Double_t scint_z, Bool_t useExactAngle){

  Double_t x = ( scint_x - sipm_x ), y = ( scint_y - sipm_y ), z = ( scint_z - sipm_z );

  if (!useExactAngle) return ( sipm_x_width * sipm_y_width * z / TMath::Power( x*x + y*y + z*z , 1.5 ) );

  Double_t dx = sipm_x_width/2, dy = sipm_y_width/2;
  Double_t t1 = ( x + dx ) * ( y + dy ) / ( z * TMath::Sqrt( (x+dx)*(x+dx) + (y+dy)*(y+dy) + z*z ) );
  Double_t t2 = ( x - dx ) * ( y - dy ) / ( z * TMath::Sqrt( (x-dx)*(x-dx) + (y-dy)*(y-dy) + z*z ) );
  Double_t t3 = ( x - dx ) * ( y + dy ) / ( z * TMath::Sqrt( (x-dx)*(x-dx) + (y+dy)*(y+dy) + z*z ) );
  Double_t t4 = ( x + dx ) * ( y - dy ) / ( z * TMath::Sqrt( (x+dx)*(x+dx) + (y-dy)*(y-dy) + z*z ) );
  return ( TMath::ATan(t1) + TMath::ATan(t2) - TMath::ATan(t3) - TMath::ATan(t4) );

}

Double_t photonNum(Double_t sipm_x, Double_t sipm_y, Double_t sipm_z,
		   Double_t scint_x, Double_t scint_y, Double_t scint_z, Double_t A0, Double_t tau,
		   Bool_t useExactAngle = false, Bool_t useReflections = false){

  /*cout<<"Sipm:  x ="<< sipm_x << ",  y ="<< sipm_y << ",  z ="<< sipm_z <<endl;
  cout<<"Scint: x ="<< scint_x << ", y ="<< scint_y << ", z ="<< scint_z <<endl;
  cout<<"Basic Solid Angle:  "<< SolidAngle(sipm_x, sipm_y, sipm_z, scint_x, scint_y, scint_z, useExactAngle) <<endl;
  cout<<"Reflected Angle +x: "<< SolidAngle(sipm_x, sipm_y, sipm_z, cristal_x_halfwidth*2-scint_x,  scint_y,                        scint_z,                       useExactAngle) <<endl;
  cout<<"Reflected Angle -x: "<< SolidAngle(sipm_x, sipm_y, sipm_z, -cristal_x_halfwidth*2-scint_x, scint_y,                        scint_z,                       useExactAngle) <<endl;
  cout<<"Reflected Angle +y: "<< SolidAngle(sipm_x, sipm_y, sipm_z, scint_x,                        cristal_y_halfwidth*2-scint_y,  scint_z,                       useExactAngle) <<endl;
  cout<<"Reflected Angle -y: "<< SolidAngle(sipm_x, sipm_y, sipm_z, scint_x,                        -cristal_y_halfwidth*2-scint_y, scint_z,                       useExactAngle) <<endl;
  cout<<"Reflected Angle +z: "<< SolidAngle(sipm_x, sipm_y, sipm_z, scint_x,                        scint_y,                        cristal_z_halfwidth*2-scint_z, useExactAngle) <<endl;
  //abort();//*/
  
  Double_t ReflectedAngles = 0;
  if (useReflections) {//cout<<"useReflections option not implemented yet! Continuing without reflections..."<<endl;
    ReflectedAngles += SolidAngle(sipm_x, sipm_y, sipm_z, cristal_x_halfwidth*2-scint_x,  scint_y,                        scint_z,                       useExactAngle);
    ReflectedAngles += SolidAngle(sipm_x, sipm_y, sipm_z, -cristal_x_halfwidth*2-scint_x, scint_y,                        scint_z,                       useExactAngle);
    ReflectedAngles += SolidAngle(sipm_x, sipm_y, sipm_z, scint_x,                        cristal_y_halfwidth*2-scint_y,  scint_z,                       useExactAngle);
    ReflectedAngles += SolidAngle(sipm_x, sipm_y, sipm_z, scint_x,                        -cristal_y_halfwidth*2-scint_y, scint_z,                       useExactAngle);
    //ReflectedAngles += SolidAngle(sipm_x, sipm_y, sipm_z, scint_x,                        scint_y,                        cristal_z_halfwidth*2-scint_z, useExactAngle);
  }

  return ( tau + A0 * ( SolidAngle(sipm_x, sipm_y, sipm_z, scint_x, scint_y, scint_z, useExactAngle) + ReflectedAngles ) );

}

void LiModelLS(Int_t &npar, Double_t *deriv, Double_t &f, Double_t *par, Int_t iflag){

  f = 0;
  Double_t pn, delta;
  //bottom matrix
  for (int i_sipm = 0; i_sipm < NsipmBot; i_sipm++) {
    mx = ekin[i_sipm]/Ekin_tot_x;
    pn = photonNum(bot_sipm_xpos[i_sipm/BotSiPMlenght], bot_sipm_ypos[i_sipm%BotSiPMlenght], -cristal_z_halfwidth, par[0], par[1], par[2], par[3], par[4], ExactAngle, Reflections);
    delta = ( mx - pn );
    f += ( delta*delta );
  }
  //side matrix
  for (int i_sipm = NsipmBot; useSideMatrix && i_sipm < NsipmBot+NsipmSide; i_sipm++) {
    mz = ekin[i_sipm]/Ekin_tot_z;
    pn = photonNum(side_sipm_zpos[i_sipm%SideSiPMlenght], side_sipm_ypos[(i_sipm-NsipmBot)/SideSiPMlenght], -cristal_x_halfwidth, par[2], par[1], par[0], par[3], par[5], ExactAngle, Reflections);
    //NB: ^ this symmetry works only for cube geometry (more specifically for x = z symmetry)
    delta = ( mz - pn );
    f += ( delta*delta );
  }

}


void LiModelWLS(Int_t &npar, Double_t *deriv, Double_t &f, Double_t *par, Int_t iflag){

  f = 0;
  Double_t pn, delta;
  //bottom matrix
  for (int i_sipm = 0; i_sipm < NsipmBot; i_sipm++) {
    mx = ekin[i_sipm]/Ekin_tot_x;
    pn = photonNum(bot_sipm_xpos[i_sipm/BotSiPMlenght], bot_sipm_ypos[i_sipm%BotSiPMlenght], -cristal_z_halfwidth, par[0], par[1], par[2], par[3], par[4], ExactAngle, Reflections);
    delta = ( mx - pn );
    f += ( delta*delta ) / pn;
  }
  //side matrix
  for (int i_sipm = NsipmBot; useSideMatrix && i_sipm < NsipmBot+NsipmSide; i_sipm++) {
    mz = ekin[i_sipm]/Ekin_tot_z;
    pn = photonNum(side_sipm_zpos[i_sipm%SideSiPMlenght], side_sipm_ypos[(i_sipm-NsipmBot)/SideSiPMlenght], -cristal_x_halfwidth, par[2], par[1], par[0], par[3], par[5], ExactAngle, Reflections);
    //NB: ^ this symmetry works only for cube geometry (more specifically for x = z symmetry)
    delta = ( mz - pn );
    f += ( delta*delta ) / pn;
  }

}


void LiModelLL(Int_t &npar, Double_t *deriv, Double_t &f, Double_t *par, Int_t iflag){
  
  double pn, pdf, lnL = 0.0;
  //bottom matrix
  for (int i_sipm = 0; i_sipm < NsipmBot; i_sipm++) {
    mx = ekin[i_sipm]/Ekin_tot_x;
    pn = photonNum(bot_sipm_xpos[i_sipm/BotSiPMlenght], bot_sipm_ypos[i_sipm%BotSiPMlenght], -cristal_z_halfwidth, par[0], par[1], par[2], par[3], par[4], ExactAngle, Reflections);
    if (pn<0) pdf = 0;
    else pdf = TMath::Poisson(mx, pn);
    if ( pdf > 0.0 ) {
      lnL += log(pdf);    // need positive pdf value
    }
    else if ( pdf == 0.0 ) {
      lnL += -999;    // disadvantage null-pdf values (minimum value of non-zero pdf log is -744)
    }
    else {
      cout << "WARNING -- pdf is negative!!!" << endl;
      cout << "mx = " << mx << "  ,  pn = " << pn << "  ,  pdf = "<<pdf<<endl;
      cout << "sipm_x = "<<bot_sipm_xpos[i_sipm/BotSiPMlenght]<<"  ,  sipm_y = "<<bot_sipm_ypos[i_sipm%BotSiPMlenght]<<"  ,  sipm_z = "<<-cristal_z_halfwidth<<endl;
      cout << "scin_x = "<<par[0]<<"  ,  scin_y = "<<par[1]<<"  ,  scin_z = "<<par[2]<<endl;
      getchar();
    }
  }
  //side matrix
  for (int i_sipm = NsipmBot; useSideMatrix && i_sipm < NsipmBot+NsipmBot; i_sipm++) {
    mz = ekin[i_sipm]/Ekin_tot_z;
    pn = photonNum(side_sipm_zpos[i_sipm%SideSiPMlenght], side_sipm_ypos[(i_sipm-NsipmBot)/SideSiPMlenght], -cristal_x_halfwidth, par[2], par[1], par[0], par[3], par[5], ExactAngle, Reflections);
    //NB: ^ this symmetry works only for cube geometry (more specifically for x = z symmetry)
    if (pn<0) pdf = 0;
    else pdf = TMath::Poisson(mz, pn);
    if ( pdf > 0.0 ) {
      lnL += log(pdf);    // need positive f
    }
    else if ( pdf == 0.0 ) {
      lnL += -999;    // disadvantage null-pdf values (minimum value of non-zero pdf log is -744)
    }
    else {
      cout << "WARNING -- pdf is negative!!!" << endl;
      cout << "mz = " << mz << "  ,  pn = " << pn << "  ,  pdf = "<<pdf<<endl;
      getchar();
    }
  }//*/

  f = -2.0 * lnL;         // factor of -2 so minuit gets the errors right
 
}//*/
