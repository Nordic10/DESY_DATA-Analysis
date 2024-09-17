#ifndef RECOTRG_H
#define RECOTRG_H

#include "KGraph.h"

class RecoTRG {
  
 public:
  RecoTRG(KGraph *gr, int event);
  double ped() { return ped_; }
  double rms() { return rms_; }
  double amp() { return amp_; }
  double time() { return time_; }
  
 private:
  double ped_;
  double rms_;
  double amp_;
  double time_;
};

#endif // RECOTRG_H
