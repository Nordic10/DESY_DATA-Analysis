#include "TreeReader.h"
#include "RecoTRG.h"
#include "KGraph.h"

#include "TROOT.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"

#include <iostream>

void triggerAlign(TString filename, int channel) {
  TreeReader tree(filename);

  auto mg = new TMultiGraph();
  mg->SetTitle(TString::Format("Sample trigger channel %d (First 50 Events);Time [ns];Voltage [mV]", channel));

  double zero = 0.0;

  auto tg = new KGraph();
  auto grTrg = new TGraph();
  grTrg->Set(N_Samples);
  for (int event=0; event<50; event++) {
    tree.get_entry(event);
    
    double* t=tree.time().data();
    double* trg=tree.trigger(0).data();
    for (unsigned int i=0; i<N_Samples; ++i) grTrg->SetPoint(i,t[i],trg[i]);
    
    tg->Init(grTrg, event);
    auto reco_tg = new RecoTRG(tg);
    if (event == 0) {
      zero = reco_tg->time();
      mg->Add(tg->TG());
    } else {
      double difference = reco_tg->time() - zero;
      tg->x_shift(-difference);
      mg->Add(tg->TG());
    }
  }

  auto c = new TCanvas();

  c->cd();
  mg->Draw("al");
}

void firstSix(TString filename, int channel) {
  
  TreeReader tree(filename);

  auto mg = new TMultiGraph();
  mg->SetTitle(TString::Format("Sample buffer channel %d (First 6 Events);Time [ns];Voltage [mV]", channel));
  
  auto leg = new TLegend(0.88,0.6,.95,0.95);
  leg->SetHeader("Event #", "C");
  
  double zero = 0.0;
  TString title;
  
  for (int event=0; event<6; event++) {
    tree.get_entry(event);
    auto g = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.voltages(channel).data()), event);
    auto tg = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.trigger(0).data()), event);
    auto reco_tg = new RecoTRG(tg);
    
    double* yvals = g->TG()->GetY();
    if (event == 0) {
      g->y_shift(-yvals[95]);
      g->TG()->SaveAs(TString::Format("firstSix/ch%d/event_%d",channel,event));
      zero = reco_tg->time();
      g->TG()->SetLineColor(1);
      mg->Add(g->TG());
      title = TString::Format("%d", event);
      leg->AddEntry(g->TG(), title);
    } else {
      double difference = reco_tg->time() - zero;
      g->y_shift(-yvals[95]);
      g->x_shift(-difference);
      tg->x_shift(-difference);
      g->TG()->SaveAs(TString::Format("firstSix/ch%d/event_%d",channel,event));
      g->TG()->SetLineColor(event+1);
      mg->Add(g->TG());
      title = TString::Format("%d", event);
      leg->AddEntry(g->TG(), title);
    }
  }
  
  auto c = new TCanvas();

  c->cd();
  mg->Draw("al");
  leg->Draw();
}

void sum(TString filename, int channel) {
  
  TreeReader tree(filename);
  
  UInt_t n = tree.num_entries();

  KGraph* sum_g;

  TGraph* avg_g;
  TGraph* max_g;
  TGraph* rise_g;
  
  TGraph* ROOTg = new TGraph();
  ROOTg->Set(N_Samples);
  KGraph* g = new KGraph();
  TGraph* ROOTtg = new TGraph();
  ROOTtg->Set(N_Samples);
  KGraph* tg = new KGraph();

  RecoTRG* reco_tg;
  
  double zero = 0.0;
  for (unsigned int event=0; event<n; event++) {
    tree.get_entry(event);

    double* t = tree.time().data();
    double* volt = tree.voltages(channel).data();
    double* trg = tree.trigger(0).data();
    for (unsigned int i=0; i<N_Samples; i++) {
      ROOTg->SetPoint(i,t[i],volt[i]);
      ROOTtg->SetPoint(i,t[i],trg[i]);
    }

    g->Init(ROOTg,event);
    tg->Init(ROOTtg,event);
    reco_tg = new RecoTRG(tg);
      
    if (event == 0) {
      g->y_shift(-volt[95]);
      zero = reco_tg->time();
      sum_g = g;
    } else {
      double difference = reco_tg->time() - zero;
      g->y_shift(-volt[95]);
      g->x_shift(-difference);
      sum_g->add(g);
    }
  }
  avg_g = sum_g->TG();
  max_g = sum_g->TG();
  rise_g = sum_g->TG();
    
  avg_g->Scale(1/(double)n);
  avg_g->SaveAs(TString::Format("sum/avg_ch%d.csv", channel));

  max_g->Scale(1/avg_g->GetMaximum());
  max_g->SaveAs(TString::Format("sum/max_ch%d.csv", channel));

  /* Rising edge normalization goes here
  rise_g->Scale(1/avg_g->GetMaimum();
  */
}

int main(int argc, char **argv){

  TApplication theApp("App", &argc, argv);

  for (int ch=0; ch<8; ch++){
    firstSix("DESY_DATA/run_311/outfile_LG.root",ch);
  }

  triggerAlign("DESY_DATA/run_311/outfile_LG.root",0);
  sum("DESY_DATA/run_311/outfile_LG.root", 0);

  std::cout << "Press ^c to exit" << std::endl;
  theApp.SetIdleTimer(60,".q");  
  theApp.Run();
  
  return 0;

}


