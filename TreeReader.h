#ifndef TREEREADER_H
#define TREEREADER_H

#include <iostream>
#include "TString.h"
#include "Rtypes.h"
#include "TChain.h"


constexpr unsigned int N_Channels = 2 * 10;
constexpr unsigned int N_Samples = 1024;

class TreeReader {
 public:
  
  TreeReader(TString filename, TString treename="tree");
  UInt_t num_entries() const;
  void get_entry(UInt_t i);

  std::array<Double_t, N_Samples> time() const;
  std::array<Double_t, N_Samples> voltages(UInt_t channel) const;
  std::array<Double_t, N_Samples> trigger(UInt_t channel) const;

  Float_t horizontal_interval() const { return horizontal_interval_; }
  UInt_t get_bin(Double_t t) const;
  
 private:

  TChain* tree_;

  Float_t horizontal_interval_;
  std::array<std::array<Float_t, N_Samples>, N_Channels> channels_;
  std::array<std::array<Float_t, N_Samples>, 2> trigger_;
  std::array<Double_t, N_Samples> times_;
};

#endif // TREEREADER_H
