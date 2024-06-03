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
  mg->SaveAs(TString::Format("firstSix_ch%d.csv", channel));
}

void sum(TString filename) {
  
  TreeReader tree(filename);

  auto mg_avg = new TMultiGraph();
  mg_avg->SetTitle("Average Pulse;Time [ns];Voltage [mV]");

  auto mg_max = new TMultiGraph();
  mg_max->SetTitle("Normailzed to Peak Pulse;Time [ns];Voltage [mv]");

  auto mg_rise = new TMultiGraph();
  mg_max->SetTitle("Normailzed to Rising Edge Pulse;Time [ns];Voltage [mv]");
  
  auto leg_avg = new TLegend(0.88,0.6,.95,0.95);
  leg_avg->SetHeader("Channel #", "C");

  auto leg_max = new TLegend(0.88,0.6,.95,0.95);
  leg_max->SetHeader("Channel #", "C");

  auto leg_rise = new TLegend(0.88,0.6,.95,0.95);
  leg_rise->SetHeader("Channel #", "C");
  
  UInt_t n = tree.num_entries();
  TString title;

  KGraph* sum_g;

  TGraph* avg_g;
  TGraph* max_g;
  TGraph* rise_g;
  KGraph* g;
  KGraph* tg;
  RecoTRG* reco_tg;
  
  for (int channel=0; channel<8; channel++) {
    double zero = 0.0;
    for (unsigned int event=0; event<n; event++) {
      tree.get_entry(event);
      g = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.voltages(channel).data()), event);
      tg = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.trigger(0).data()), event);
      reco_tg = new RecoTRG(tg);
      
      double* yvals = g->TG()->GetY();
      if (event == 0) {
	g->y_shift(-yvals[95]);
	zero = reco_tg->time();
	sum_g = g;
      } else {
	double difference = reco_tg->time() - zero;
	g->y_shift(-yvals[95]);
	g->x_shift(-difference);
	tg->x_shift(-difference);
	sum_g->add(g);
      }
    }
    avg_g = sum_g->TG();
    max_g = sum_g->TG();
    rise_g = sum_g->TG();
    title = TString::Format("%d", channel);
    
    avg_g->Scale(1/(double)n);
    avg_g->SetLineColor(channel+1);
    leg_avg->AddEntry(avg_g, title);
    mg_avg->Add(avg_g);

    max_g->Scale(1/avg_g->GetMaximum());
    max_g->SetLineColor(channel+1);
    leg_max->AddEntry(max_g, title);
    mg_max->Add(max_g);

    rise_g->SetLineColor(channel+1);
    leg_rise->AddEntry(rise_g, title);
    mg_max->Add(max_g);

    std::cout << "Channel " << channel << " finished" << std::endl;
  }

  std::cout << "plotting..." << std::endl;
  
  auto c_avg = new TCanvas();
  auto c_max = new TCanvas();
  auto c_rise = new TCanvas();
  
  c_avg->cd();
  mg_avg->Draw("al");
  leg_avg->Draw();
  
  c_max->cd();
  mg_max->Draw("al");
  leg_max->Draw();

  c_rise->cd();
  mg_rise->Draw("al");
  leg_rise->Draw();
}

int main(int argc, char **argv){

  TApplication theApp("App", &argc, argv);

  for (int ch=0; ch<8; ch++){
    firstSix("DESY_DATA/run_311/outfile_LG.root",ch);
  }

  //sum("DESY_DATA/run_311/outfile_LG.root");

  std::cout << "Press ^c to exit" << std::endl;
  theApp.SetIdleTimer(60,".q");  
  theApp.Run();
  
  return 0;

}


