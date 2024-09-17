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
#include "TLine.h"
#include "TF1.h"
#include "TMath.h"

#include <cmath>
#include <iostream>
#include <fstream>

double gauss_fitf(double *val, double *par) {
  double x = val[0];
  double a = par[0];
  double b = par[1];
  double c = par[2];
  return par[0] * TMath::Exp(-(x-b)*(x-b)/(2*c*c));
}

double dad_fitf(double *val, double *par) {
  double x = val[0];
  return (par[0]*TMath::Power(x,par[1]))/(par[2] + par[3]*TMath::Power(10,par[5])*TMath::Power(x,par[4]));
  //return par[0] + par[1]*x + par[2]*x*x + par[3]*x*x*x;
}

void get_max_hist(TH1F* h, double &max, double &x) {
  int n = h->GetNbinsX();
  int index = 0;
  for (int i=0; i<n; i++) {
    if (h->GetBinContent(i) > max) {
      max = h->GetBinContent(i);
      index = i;
    }
  }
  x = h->GetBinCenter(index);
}

double pulse_end(TGraph* g, bool neg) {
  int n = g->GetN();
  double* xvals = g->GetX();
  double* yvals = g->GetY();
  bool pulse = false;
  for (int i=0; i<n; i++) {
    if (abs(yvals[i]) > 4) {
      pulse = true;
    }
    if (pulse) {
      if (neg) {
	if (yvals[i] > -2) {
	  return xvals[i];
	}
      } else {
	if (yvals[i] < 2) {
	  return xvals[i];
	}
      }
    }
  }
  return 0.0;
}

double center(TH1F* h, int channel) {
  int n = h->GetNbinsX();
  TF1 *func = new TF1("fit",dad_fitf,5,555,6);   //fit with first function
  func->SetParameters(500,1.2,1000,0.001,2.5,-2);
  func->SetParNames("Num_Mult","Num_Power","Den_Adder","Den_Mult","Den_Power");
  
  //TF1 *func = new TF1("fit",gauss_fitf,5,555,3);   //fit with gaussian function
  //func->SetParameters(150,15,5);
  //func->SetParNames("Magnitude","x_offset","width");
  
  h->Fit("fit");
  double max_X = func->GetMaximumX();

  double x = 0;
  double max = 0;
  get_max_hist(h,max,x);
  std::cout << "max: " << max << std::endl;
  std::cout << "x: " << x << std::endl;
  
  auto c = new TCanvas();
  c->cd();
  TLine* l = new TLine(max_X,-2,max_X,1000);
  TLine* l2 = new TLine(x, -2, x, 1000);
  l->SetLineColor(4);
  h->SetLineColor(2);
  func->SetLineColor(4);
  h->Draw();
  func->Draw("same");
  l->Draw("same");
  l2->Draw("same");
  return max_X;
}

TH1F* Cut(int run, int channel, int low, int high, bool LG) {
  TreeReader tree(TString::Format("DESY_DATA/run_%d/outfile_LG.root",run));

  UInt_t n = tree.num_entries();

  TGraph* ROOTg = new TGraph();
  ROOTg->Set(N_Samples);
  KGraph* g = new KGraph();

  TString title = TString::Format("Event Maxima for Channel %d;Maximum [mv];Count", channel);
  
  TH1F* h = new TH1F("maxHist",title,700,low,high);

  for (unsigned int event=0; event<n; event++) {
    tree.get_entry(event);

    double* t = tree.time().data();
    double* volt = tree.voltages(channel).data();
    for (unsigned int i=0; i<N_Samples; i++) ROOTg->SetPoint(i,t[i],volt[i]);
    g->Init(ROOTg,event);
    g->y_shift(-volt[70]);
    bool no_filter = false;

    if (LG) {
      //std::cout << "g->ped_rms()*5: " << g->ped_rms()*5 <<  std::endl;
      //std::cout << "-g->Min(): : " << -g->Min() <<  std::endl;
      if (g->ped_rms()*10 < -g->Min() || no_filter){
	h->Fill(-g->Min());
      } else {
	//std::cout << "did not fill" << std::endl;
      }
    } else {
      if (g->ped_rms()*10 < g->Max()){
	h->Fill(g->Max());
      }
    }
    
  }

  //auto c = new TCanvas();
  //c->cd();
  //h->SetLineColor(2);
  //h->Draw();
  return h;
  //h->SaveAs(TString::Format("run_%d/maxHist_ch%d.csv",run,channel));
  //c->SaveAs(TString::Format("run_%d/maxHist_ch%d.jpeg",run,channel));
  
}

void average(int run, int channel, bool LG, double scaler, bool firstWrite) {
  
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

    bool no_filter = false;
    bool pass;
    if (LG) {
      if (g->ped_rms()*10 < -g->Min() || no_filter){
        pass = true;
      } else {
	pass = false;
      }
    } else {
      if (g->ped_rms()*10 < g->Max() || no_filter){
        pass = true;
      } else {
	pass = false;
      }
    }
    
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
    double normalizer = (LG) ? sum_g->Min() : sum_g->Max();
    sum_g->y_scale(scaler/normalizer);
    KGraph* der = sum_g->derivative();
    KGraph* der2 = der->derivative();
    
    
    avg_g = sum_g->TG();
    TGraph* der_g = der->TG();
    TGraph* der2_g = der2->TG();

    double yline1 = der->Max();   
    double xline1 = der->GetX(yline1);

    double yline2 = sum_g->GetY(xline1) * 2.6137;
    double xline2 = sum_g->GetX(yline2);
    
    TLine* ver1 = new TLine(xline1, 0, xline1, 250);
    TLine* ver2 = new TLine(xline2, 0, xline2, 250);
    ver1->SetLineColor(2);
    ver2->SetLineColor(4);

    avg_g->SaveAs(TString::Format("peComp/run%d_ch%d.csv",run,channel));
    der_g->SaveAs(TString::Format("peComp/der_run%d_ch%d.csv",run,channel));

    std::ofstream MyFile;
    MyFile.open("peComp/maxes.txt", std::ios_base::app);
    if (firstWrite) {
      MyFile << "run " << run << std::endl;
    }
    MyFile << "ch " << channel << std::endl;
    MyFile << "Line 1" << std::endl << "Xend:" << xline1 << std::endl << "Yend:" << yline1 << std::endl;
    MyFile << "Line 2" << std::endl << "Xend:" << xline2 << std::endl << "Yend:" << yline2 << std::endl;
    MyFile.close();
     
    
    auto c = new TCanvas();
    c->Divide(1,2);
    //c->Divide(1,3);
    c->cd(1);
    avg_g->SetLineColor(6);
    avg_g->SetTitle(TString::Format("Average Pulse for Channel %d;Time [ns];Voltage [mV]", channel));
    avg_g->Draw();
    ver1->Draw("same");
    ver2->Draw("same");
    c->cd(2);
    der_g->SetLineColor(8);
    der_g->SetTitle(TString::Format("Derivative of Average Pulse for Channel %d;Time [ns];dV/dt", channel));
    der_g->Draw();
    ver1->Draw("same");
    //c->cd(3);
    //der2_g->SetLineColor(4);
    //der2_g->SetTitle(TString::Format("Second Derivative of Average Pulse for Channel %d;Time [ns];d2V/dt2", channel));
    //der2_g->Draw();
    
    

  }else{
    std::cout << "Nothing passes in run " << run << " channel " << channel << std::endl;
  }
}

int main(int argc, char **argv){

  TApplication theApp("App", &argc, argv);
  int runs[29] = {
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
		  178, //28
  };

  int low = -10;
  int high = 600;
  
  int i = 0;
  //int ch=1;
  bool first = true;
  for (int ch=0; ch<8; ch++){
    if (ch != 5 && ch != 6 && ch!=0 && ch!=3) {
      TH1F* h = Cut(runs[i],ch,low,high,true);
      double max = center(h,ch);
      average(runs[i],ch,true,max,first);
      std::cout << "run " << runs[i] << " channel " << ch << " is finished" << std::endl;
      first = false;
      std::cout << "max scaler: " << max << std::endl;
    }
  }
  
  std::cout << "Press ^c to exit" << std::endl;
  theApp.SetIdleTimer(180,".q");  
  theApp.Run();
  
  return 0;

}
