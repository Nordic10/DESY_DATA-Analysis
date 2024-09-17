#include "TreeReader.h"
#include "RecoTRG.h"
#include "KGraph.h"

#include "TROOT.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TH1F.h"

#include <iostream>

void validHist(int run, int channel, bool LG) {
  TreeReader tree(TString::Format("DESY_DATA/run_%d/outfile_LG.root",run));

  UInt_t n = tree.num_entries();

  TGraph* ROOTg = new TGraph();
  ROOTg->Set(N_Samples);
  KGraph* g = new KGraph();
  int low = -10;
  int high = 600;

  TString title = TString::Format("Event Maxima for Channel %d;Maximum [mv];Count", channel);
  
  TH1F* h = new TH1F("maxHist",title,700,low,high);

  for (unsigned int event=0; event<n; event++) {
    tree.get_entry(event);

    double* t = tree.time().data();
    double* volt = tree.voltages(channel).data();
    for (unsigned int i=0; i<N_Samples; i++) ROOTg->SetPoint(i,t[i],volt[i]);
    g->Init(ROOTg,event);
    g->y_shift(-volt[70]);

    if (LG) h->Fill(-g->Min()); else h->Fill(g->Max());
  }

  auto c = new TCanvas();

  c->cd();
  h->SetLineColor(2);
  h->Draw();
  h->SaveAs(TString::Format("run_%d/maxHist_ch%d.csv",run,channel));
  c->SaveAs(TString::Format("run_%d/maxHist_ch%d.jpeg",run,channel));
  
}
  

void triggerAlign(TString filename, int channel) {
  TreeReader tree(filename);

  UInt_t n = tree.num_entries();

  auto mg = new TMultiGraph();
  mg->SetTitle(TString::Format("Sample trigger channel %d (First 20 Events);Time [ns];Voltage [mV]", channel));

  double zero = 150.0;

  auto tg = new KGraph();
  auto grTrg = new TGraph();
  grTrg->Set(N_Samples);
  for (int event=0; event<20; event++) {
    tree.get_entry(event);
    
    double* t=tree.time().data();
    double* trg=tree.trigger(0).data();
    for (unsigned int i=0; i<N_Samples; ++i) grTrg->SetPoint(i,t[i],trg[i]);
    
    tg->Init(grTrg, event);
    auto reco_tg = new RecoTRG(tg,event);
    double difference = reco_tg->time() - zero;
    tg->x_shift(-difference);
    mg->Add(tg->TG());
  }

  mg->GetXaxis()->SetLimits(120,180);
  mg->SetMinimum(-500);
  mg->SetMaximum(200);
  
  auto c = new TCanvas();

  c->cd();
  mg->Draw("al");
}

void firstSix(int run, int channel) {
  
  TreeReader tree(TString::Format("DESY_DATA/run_%d/outfile_LG.root",run));

  auto mg = new TMultiGraph();
  mg->SetTitle(TString::Format("Sample buffer channel %d (First 6 Events);Time [ns];Voltage [mV]", channel));
  
  auto leg = new TLegend(0.88,0.6,.95,0.95);
  leg->SetHeader("Event #", "C");
  
  double zero = 150.0;
  TString title;
  
  for (int event=0; event<6; event++) {
    tree.get_entry(event);
    auto g = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.voltages(channel).data()), event);
    auto tg = new KGraph(new TGraph(N_Samples, tree.time().data(), tree.trigger(0).data()), event);
    auto reco_tg = new RecoTRG(tg, event);
    
    double* yvals = g->TG()->GetY();
    /*
    if (event == 0) {
      g->y_shift(-yvals[95]);
      g->TG()->SaveAs(TString::Format("run_%d/firstSix_ch%d_event%d",run,channel,event));
      //zero = reco_tg->time();
      g->TG()->SetLineColor(1);
      mg->Add(g->TG());
      title = TString::Format("%d", event);
      leg->AddEntry(g->TG(), title);
    } else {
    */
    double difference = reco_tg->time() - zero;
    g->y_shift(-yvals[95]);
    g->x_shift(-difference);
    tg->x_shift(-difference);
    g->TG()->SaveAs(TString::Format("run_%d/firstSix_ch%d_event%d",run,channel,event));
    g->TG()->SetLineColor(event+1);
    mg->Add(g->TG());
    title = TString::Format("%d", event);
    leg->AddEntry(g->TG(), title);
      //}
  }
}

void sum(int run, int channel, bool LG) {
  
  TreeReader tree(TString::Format("DESY_DATA/run_%d/outfile_LG.root",run));
  
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
  
  double zero = 150.0;
  bool first = true;

  bool nothing_passes = true;
  
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
    reco_tg = new RecoTRG(tg, event);

    g->y_shift(-volt[95]);

    bool pass;
    pass = true;
    //if (LG) pass = g->Min() < -10; else pass = g->Max() > 10;

    
    if (pass) {
      nothing_passes = false;
    }

    double difference = reco_tg->time() - zero;
    
    if (pass){
      if (first) {
	TGraph* ROOTsum_g = new TGraph(*g->TG());
	sum_g = new KGraph(ROOTsum_g,event);
	sum_g->x_shift(-difference);
	first = false;
      } else {
	g->x_shift(-difference);
	sum_g->add(g);
      }
    }
  }
  if (!nothing_passes) {
    avg_g = sum_g->TG();
    max_g = sum_g->TG();
    //rise_g = sum_g->TG();
    
    avg_g->Scale(1/(double)n);
    avg_g->SaveAs(TString::Format("run_%d/sum_avg_ch%d.csv",run,channel));

    if (LG) max_g->Scale(1/sum_g->Min()); else max_g->Scale(1/sum_g->Max());
    max_g->SaveAs(TString::Format("run_%d/sum_max_ch%d.csv",run,channel));

    /* Rising edge normalization goes here
       rise_g->Scale(1/avg_g->GetMaimum();
    */
  }else{
    std::cout << "run" << run << " channel " << channel << std::endl;
  }
}

int main(int argc, char **argv){

  TApplication theApp("App", &argc, argv);
  int runs[31] = {
		  186, //0
		  189, //1
		  192, //2
		  194, //3
		  196, //4
		  198, //5
		  200, //6
		  175, //7
		  171, //8
		  167, //9
		  150, //10
		  155, //11
		  158, //12
		  163, //13
		  329, //14
		  331, //15
		  333, //16
		  337, //17
		  340, //18
		  343, //19
		  346, //20
		  347, //21
		  350, //22
		  321, //23
		  319, //24
		  317, //25
		  314, //26
		  311, //27
		  176, //28
		  177, //29
		  178, //30
  };

  int i = 27;
  //int ch = 0;
  //for (int i=0; i<28; i++) {
  for (int ch=0; ch<8; ch++){
    //firstSix(runs[i],ch);
    validHist(runs[i],ch,true);
    //sum(runs[i],ch,true);
    std::cout << "run " << runs[i] << " channel " << ch << " is finished" << std::endl;
  }
  //}
  //triggerAlign("DESY_DATA/run_311/outfile_LG.root",0);
  //firstSix("DESY_DATA/run_311/outfile_LG.root",0);
  //sum("DESY_DATA/run_311/outfile_LG.root",0,true);
  

  std::cout << "Press ^c to exit" << std::endl;
  theApp.SetIdleTimer(240,".q");  
  theApp.Run();
  
  return 0;

}


