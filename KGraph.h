#ifndef KGRAPH_H
#define KGRAPH_H

#include "TGraph.h"

class KGraph {
 public:
  KGraph();
  KGraph(TGraph* gr, int e);
  void Init(TGraph* gr, int e);

  double GetY(double x) const;
  double GetX(double y) const;
  KGraph* derivative();
  void x_shift(double o);
  void y_shift(double o);
  void add(KGraph* k);
  void y_scale(double s);
  double Max() const;
  double Min() const;
  double ped_rms() const;
  void print() const;
  TGraph* TG() { return g; }
  
 private:
  TGraph* g;
  int n;
  double* xvals;
  double* yvals;
  int event;
};

#endif // KGraph_H
