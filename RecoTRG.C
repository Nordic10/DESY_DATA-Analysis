#include "RecoTRG.h"
#include "KGraph.h"
#include <iostream>

RecoTRG::RecoTRG(KGraph *gr, int event) {
  int N = gr->TG()->GetN();
  double *X = gr->TG()->GetX();
  double *Y = gr->TG()->GetY();
  double ped0 = 0;
  double ped1 = 0;
  double ped2 = 0;
  double amp0 = 0;
  double amp1 = 0;
  double amp2 = 0;
  double amin = 1e+9;
  double amax = -1e+9;
  for (int i=0; i<N; i++) {
    if (Y[i] < -400.) { // Pedestal
      ped0 += 1.;
      ped1 += Y[i];
      ped2 += Y[i] * Y[i];
    } else if (Y[i] > 0.) { // Plateau
      amp0 += 1.;
      amp1 += Y[i];
      amp2 += Y[i] * Y[i];
    }
    if (Y[i] <= amin) amin = Y[i];
    if (Y[i] >= amax) amax = Y[i];
  }
  ped_ = 0;
  if (ped0 > 1.) {
    ped_ = ped1 / ped0;
    rms_ = sqrt( ped2 / ped0 - ped_ * ped_);
  }
  amp_ = 0;
  if (amp0 > 1.) {
    amp_ = amp1 / amp0;
    amp_ -= ped_;
  }
  
  // Threshold timing using linear interpolation
  double threshold = ped_ + 0.5 * amp_;
  time_ = 0;
  for (int i=1; i<N; i++) {
    if (Y[i-1] <= threshold && Y[i] > threshold) {
      time_ = X[i-1] + (threshold - Y[i-1])/(Y[i] - Y[i-1]) * (X[i] - X[i-1]);
    }
  }
}
