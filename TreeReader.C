#include "TreeReader.h"
#include "TChain.h"
#include "TChainElement.h"
#include <iostream>


TreeReader::TreeReader(TString filename, TString treename) {
  tree_ = new TChain(treename);
  tree_->Add(filename);
  
  std::cout << "The following files were added to the TChain:\n";
  const auto& fileElements = *tree_->GetListOfFiles();
  for (TObject const* op : fileElements) {
    auto chainElement = static_cast<const TChainElement*>(op);
    std::cout << chainElement->GetTitle() << "\n";
  }
  
  tree_->Print();
  tree_->SetBranchAddress("horizontal_interval", &horizontal_interval_);
  tree_->SetBranchAddress("channels", channels_[0].data());
  tree_->SetBranchAddress("trigger", trigger_[0].data());
  
}

UInt_t TreeReader::num_entries() const {
  //return 5000; // std::min(5000, tree_->GetEntries());
  return tree_->GetEntries();
}

void TreeReader::get_entry(UInt_t i) {
  tree_->GetEntry(i);
}

std::array<Double_t, N_Samples> TreeReader::time() const {
  std::array<Double_t, N_Samples> t;
  for (unsigned int i = 0; i < N_Samples; i++) {
    t[i] = static_cast<Double_t>(i) * horizontal_interval_;
  }
  return t;
}

std::array<Double_t, N_Samples> TreeReader::voltages(UInt_t channel) const {
  std::array<Double_t, N_Samples> volts;
  for (unsigned int i = 0; i < N_Samples; i++) {
    volts[i] = static_cast<Double_t>(channels_[channel][i]);
  }
  return volts;
}

std::array<Double_t, N_Samples> TreeReader::trigger(UInt_t channel) const {
  std::array<Double_t, N_Samples> volts;
  for (unsigned int i = 0; i < N_Samples; i++) {
    volts[i] = static_cast<Double_t>(trigger_[channel][i]);
  }
  return volts;
}

UInt_t TreeReader::get_bin(Double_t t) const {
  return static_cast<UInt_t>(t / horizontal_interval_);
}
