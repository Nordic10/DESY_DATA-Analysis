#include "TGraph.h"
#include "KGraph.h"
#include <iostream>

using namespace std;

KGraph::KGraph(TGraph *gr, int e) {
  g = gr;
  n = gr->GetN();
  xvals = gr->GetX();
  yvals = gr->GetY();
  event = e;
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

void KGraph::y_scale(double s) {
  TGraph* new_g = new TGraph();
    
  for (int i=0; i<n; i++) {
    new_g->SetPoint(i,xvals[i],yvals[i]*s);
  }
  
  g = new_g;
  
  xvals = g->GetX();
  yvals = g->GetY();
}

void KGraph::print() const {
  for (int i=0; i<n; i++) {
    cout << "Point #:" << i << "     X: " << xvals[i] << "     Y: " << yvals[i] << endl;
  }
}
