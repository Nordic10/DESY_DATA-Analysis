#include "TGraph2D.h"
#include "TCanvas.h"

#include <iostream>

// simple plotting tool to look at DESY beam events

constexpr unsigned int N_Channels = 2 * 10;
constexpr unsigned int N_Samples = 1024;

class TreeReader {
public:
  TreeReader(TString filename, TString treename="tree") {
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
  
  UInt_t num_entries() const {
    //return 5000; // std::min(5000, tree_->GetEntries());
    return tree_->GetEntries();
  }

  void get_entry(UInt_t i) {
    tree_->GetEntry(i);
  }

  const std::array<Double_t, N_Samples> time() const {
    std::array<Double_t, N_Samples> t;
    for (int i = 0; i < N_Samples; i++) {
      t[i] = static_cast<Double_t>(i) * horizontal_interval_;
    }
    return t;
  }

  std::array<Double_t, N_Samples> voltages(UInt_t channel) const {
    std::array<Double_t, N_Samples> volts;
    for (int i = 0; i < N_Samples; i++) {
      volts[i] = static_cast<Double_t>(channels_[channel][i]);
    }
    return volts;
  }

  std::array<Double_t, N_Samples> trigger(UInt_t channel) const {
    std::array<Double_t, N_Samples> volts;
    for (int i = 0; i < N_Samples; i++) {
      volts[i] = static_cast<Double_t>(trigger_[channel][i]);
    }
    return volts;
  }

  
  Float_t horizontal_interval() const { return horizontal_interval_; }

  UInt_t get_bin(Double_t t) const {
    return static_cast<UInt_t>(t / horizontal_interval_);
  }

private:
  TChain* tree_;

  Float_t horizontal_interval_;
  std::array<std::array<Float_t, N_Samples>, N_Channels> channels_;
  std::array<std::array<Float_t, N_Samples>, 2> trigger_;
  std::array<Double_t, N_Samples> times_;
};


void quick_plotter(TString filename, int event=0){
  TreeReader tree(filename);

  auto canvas = new TCanvas();
  canvas->Divide(1,2);
  canvas->cd(1);
  tree.get_entry(event);
  auto mg = new TMultiGraph();
  mg->SetTitle(TString::Format("Sample buffer event %d;Time [ns];Voltage [mV]", event));
  for (int ch =0; ch<8; ++ch){
    auto g = new TGraph(N_Samples, tree.time().data(), tree.voltages(ch).data());
    g->SetLineColor(ch+1);
    mg->Add(g);
  }
  mg->Draw("al");

  canvas->cd(2);
  auto mgt = new TMultiGraph();
  mgt->SetTitle(TString::Format("Sample buffer trigger %d;Time [ns];Voltage [mV]", event));
  auto g = new TGraph(N_Samples, tree.time().data(), tree.trigger(0).data());
  mgt->Add(g);
  mgt->Draw("al");
}


