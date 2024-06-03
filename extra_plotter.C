
#include "TGraph.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TMultiGraph.h"
#include "TChainElement.h"
#include "TLegend.h"

#include <iostream>

using namespace std;

// simple plotting tool to look at DESY beam events

constexpr unsigned int N_Channels = 2 * 10;
constexpr unsigned int N_Samples = 1024;

class KGraph {
 public:
  KGraph(TGraph *gr, int e) {
    g = gr;
    n = gr->GetN();
    xvals = gr->GetX();
    yvals = gr->GetY();
    event = e;
  }

  
  void x_shift(double o) {
    TGraph* g1 = new TGraph();
    TGraph* g2 = new TGraph();

    for (int i=0; i<n; i++) {
      g1->SetPoint(i,xvals[i]+o,yvals[i]);
    }

    for (int i=0; i<n; i++) {
      g2->SetPoint(i,i,g1->Eval(i));
    }

    g = g2;
    
    n = g->GetN();
    xvals = g->GetX();
    yvals = g->GetY();
    
  }
  

  void y_shift(double o) {
    TGraph* new_g = new TGraph();
    
    for (int i=0; i<n; i++) {
      new_g->SetPoint(i,xvals[i],yvals[i]+o);
    }

    g = new_g;
    
    xvals = g->GetX();
    yvals = g->GetY();
  }

  void add(KGraph* k) {
    int nk = k->TG()->GetN();
    int n_min;
    if (n>nk) {
      n_min=nk;
    } else {
      n_min=n;
    }

    double* xvals2 = k->TG()->GetX();
    double* yvals2 = k->TG()->GetY();
    
    int diff = 0.0;
    bool found = false;

    for (int i=0; i<100; i++) {
      for (int j=0; j<100; j++) {
	if (xvals[i] == xvals2[j]) {
	  diff = i-j;
	  found = true;
	}
      }
    }

    if (!found) {
      cout << "ERROR: Attempting to Add Incompatible Graphs" << endl;
    } else {
      TGraph* new_g = new TGraph();
      for (int i=0; i<n; i++) {
	new_g->SetPoint(i,xvals[i],yvals[i]+yvals2[i+diff]);
      }

      g = new_g;
      
      xvals = g->GetX();
      yvals = g->GetY();
    }
  }

  void y_scale(double s) {
    TGraph* new_g = new TGraph();
    
    for (int i=0; i<n; i++) {
      new_g->SetPoint(i,xvals[i],yvals[i]*s);
    }

    g = new_g;
    
    xvals = g->GetX();
    yvals = g->GetY();
  }

  void printPoints() {
    for (int i=0; i<n; i++) {
      cout << "Point #:" << i << "     X: " << xvals[i] << "     Y: " << yvals[i] << endl;
    }
  }

  TGraph* TG() { return g; }
  
 private:
  TGraph* g;
  int n;
  double* xvals;
  double* yvals;
  int event;
};

class RecoTRG { // by Sasha
public:
  RecoTRG(KGraph *gr) {
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
      if (X[i] < 100.) { // Pedestal
	ped0 += 1.;
	ped1 += Y[i];
	ped2 += Y[i] * Y[i];
      } else if (X[i] > 400.) { // Plateau
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
	break;
      }
    }
  }

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

class TreeReader { // By Professor Hirosky
public:
  TreeReader(TString filename, TString treename="tree") {
    tree_ = new TChain(treename);
    tree_->Add(filename);

    cout << "The following files were added to the TChain:\n";
    const auto& fileElements = *tree_->GetListOfFiles();
    for (TObject const* op : fileElements) {
      auto chainElement = static_cast<const TChainElement*>(op);
      cout << chainElement->GetTitle() << "\n";
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

  const array<Double_t, N_Samples> time() const {
    array<Double_t, N_Samples> t;
    for (int i = 0; i < N_Samples; i++) {
      t[i] = static_cast<Double_t>(i) * horizontal_interval_;
    }
    return t;
  }

  array<Double_t, N_Samples> voltages(UInt_t channel) const {
    array<Double_t, N_Samples> volts;
    for (int i = 0; i < N_Samples; i++) {
      volts[i] = static_cast<Double_t>(channels_[channel][i]);
    }
    return volts;
  }

  array<Double_t, N_Samples> trigger(UInt_t channel) const {
    array<Double_t, N_Samples> volts;
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
  array<array<Float_t, N_Samples>, N_Channels> channels_;
  array<array<Float_t, N_Samples>, 2> trigger_;
  array<Double_t, N_Samples> times_;
};

void quick_plotter(TString filename, int channel) {
  
  TreeReader tree(filename);

  auto mg2 = new TMultiGraph();
  mg2->SetTitle(TString::Format("Sample buffer channel %d (First 6 Events);Time [ns];Voltage [mV]", channel));
  
  auto mg1 = new TMultiGraph();
  mg1->SetTitle(TString::Format("Sample buffer channel %d (Normalized Sum of All Events);Time [ns];Voltage [mV]", channel));

  auto mgt = new TMultiGraph();
  mgt->SetTitle("Sample buffer trigger;Time [ns];Voltage [mV]");
  
  auto leg = new TLegend(0.88,0.6,.95,0.95);
  leg->SetHeader("Event #", "C");
  
  KGraph* sum_g;
  
  double zero = 0.0;
  UInt_t n = tree.num_entries();
  TString title;
  
  for (int event=0; event<n; event++) {
    tree.get_entry(event);
    auto g = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.voltages(channel).data()), event);
    auto tg = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.trigger(0).data()), event);
    auto reco_tg = new RecoTRG(tg);
    
    double* xvals = g->TG()->GetX();
    double* yvals = g->TG()->GetY();
    if (event == 0) {
      g->y_shift(-yvals[95]);
      zero = reco_tg->time();
      sum_g = g;
      sum_g->TG()->SetLineColor(1);
      mg2->Add(sum_g->TG());
      title = TString::Format("%d", event);
      leg->AddEntry(sum_g->TG(), title);
      mgt->Add(tg->TG());
    } else {
      double difference = reco_tg->time() - zero;
      g->y_shift(-yvals[95]);
      g->x_shift(-difference);
      tg->x_shift(-difference);
      sum_g->add(g);
      if (event<5) {
	g->TG()->SetLineColor(event+1);
	mg2->Add(g->TG());
	//std::cout << "---------" << endl << "Event: " << event << endl << "---------" << endl;
	title = TString::Format("%d", event);
	leg->AddEntry(g->TG(), title);
      }
      if (event<100) {
	mgt->Add(tg->TG());
      }
    }
  }
  sum_g->y_scale(1/(double)n);
  sum_g->TG()->SetLineColor(channel+1);
  mg1->Add(sum_g->TG());
  
  auto c1 = new TCanvas();
  auto c2 = new TCanvas();

  c1->cd();
  mg2->Draw("al");
  leg->Draw();
  //mgt->Draw("al");
  
  c2->cd();
  mg1->Draw("ac");
  //mgt->Draw("al");
}


