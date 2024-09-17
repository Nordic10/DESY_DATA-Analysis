#include "TGraph.h"
#include "KGraph.h"
#include "TreeReader.h"

#include <iostream>
#include <cmath>

using namespace std;

KGraph::KGraph(){;}

KGraph::KGraph(TGraph *gr, int e) {
  Init(gr,e);
}

void KGraph::Init(TGraph *gr, int e) {
  g = gr;
  n = gr->GetN();
  xvals = gr->GetX();
  yvals = gr->GetY();
  event = e;
}

double KGraph::GetY(double x) const {
  for (int i=0; i<n; i++) {
    if (x>xvals[i]-0.01 && x<xvals[i]+0.01) {
      return yvals[i];
    }
  }
  return 0.0;
}

double KGraph::GetX(double y) const {
  for (int i=0; i<n; i++) {
    double m = (yvals[i+1]-yvals[i])/(xvals[i+1]-xvals[i]);
    double b = yvals[i] - m*xvals[i];
    double x = (y-b)/m;
    if (x>=xvals[i] && x<=xvals[i+1]) {
      return x;
    }
  }
  cout << "DON'T LOOK AT ME!" << endl;
  return 0.0;
}

KGraph* KGraph::derivative() {
  TGraph* ROOTg = new TGraph();
  ROOTg->Set(N_Samples);
  double der = 0.0;
  for (int i=0; i<n-1; i++) {
    der = (yvals[i+1]-yvals[i])/(xvals[i+1]-xvals[i]);
    ROOTg->SetPoint(i,xvals[i],der);
  }
  der = (yvals[n-1]-yvals[n-2])/(xvals[n-1]-xvals[n-2]);
  ROOTg->SetPoint(n-1,xvals[n-1],der);

  KGraph* der_g = new KGraph(ROOTg, event);
  return der_g;
}

void KGraph::x_shift(double o) {
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
  
void KGraph::y_shift(double o) {
  TGraph* new_g = new TGraph();
  
  for (int i=0; i<n; i++) {
    new_g->SetPoint(i,xvals[i],yvals[i]+o);
  }
  
  g = new_g;
  
  xvals = g->GetX();
  yvals = g->GetY();
}

void KGraph::add(KGraph* k) {
    
  double* xvals2 = k->TG()->GetX();
  double* yvals2 = k->TG()->GetY();
  
  int diff = 0.0;
  bool found = false;
  
  for (int i=0; i<100; i++) {
    for (int j=0; j<100; j++) {
      if (xvals[i] == xvals2[j]) {
	diff = i-j;
	found = true;
	break;
      }
    }
  }
  
  if (!found) {
    cout << "ERROR: Attempting to Add Incompatible Graphs" << endl;
  } else {
    //TGraph* new_g = new TGraph();
    //new_g->Set(N_Samples);
    for (int i=0; i<n; i++) {
      double temp = yvals[i]+yvals2[i+diff];
      g->SetPoint(i,xvals[i],temp);
      /*
      if (i == 200){
	std::cout << "yvals[i]: " << yvals[i] << std::endl;
	std::cout << "yvals2[i]: " << yvals2[i] << std::endl;
	std::cout << "expectaions: " << temp << std::endl;
	double x;
	double y;
	g->GetPoint(i,x,y);
	std::cout << "reality: " << y << std::endl;
      }
      */
    }
    
    //g = new_g;
    
    xvals = g->GetX();
    yvals = g->GetY();
  }
}

void KGraph::y_scale(double s) {
  TGraph* new_g = new TGraph();
    
  for (int i=0; i<n; i++) {
    new_g->SetPoint(i,xvals[i],yvals[i]*s);
  }
  
  g = new_g;
  
  xvals = g->GetX();
  yvals = g->GetY();
}

double KGraph::Max() const {
  double max = yvals[0];
  for (int i=0; i<n; i++) {
    if (yvals[i]>max) {
      max = yvals[i];
    }
  }
  return max;
}

double KGraph::Min() const {
  double min = yvals[0];
  for (int i=0; i<n; i++) {
    if (yvals[i]<min) {
      min = yvals[i];
    }
  }
  return min;
}

double KGraph::ped_rms() const {
  double sum = 0;
  for (int i=0; i<35; i++) {
    sum+=yvals[i]*yvals[i];
  }
  double rms = sqrt(sum/35);
  return rms;
}

void KGraph::print() const {
  for (int i=0; i<n; i++) {
    cout << "Point #:" << i << "     X: " << xvals[i] << "     Y: " << yvals[i] << endl;
  }
}
