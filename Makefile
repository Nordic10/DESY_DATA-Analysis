FC	=	gfortran
#LFLAGS = 	-O -L$(COMPHY)/lib -lnumer
LFLAGS 	= 	-g -Llib -lnumer
FFLAGS 	= 	-c -g

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)
ROOTGLIBS  = $(shell root-config --glibs) 
CXXFLAGS  += $(ROOTCFLAGS)
GLIBS      = $(ROOTGLIBS)

ROOTFLAGS   = $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) 
#P5640FLAGS  = -L${P5640LIB}/lib -lP5640  -I${P5640LIB}
GSLFLAGS     = -lgsl -lgslcblas




all: plot_all pe

plot_all: main.C 
	g++ -g -Wall -oplot_all main.C TreeReader.C KGraph.C RecoTRG.C $(ROOTFLAGS) $(GSLFLAGS)

pe: pe_main.C
	g++ -g -Wall -ope pe_main.C TreeReader.C KGraph.C RecoTRG.C $(ROOTFLAGS) $(GSLFLAGS)

clean:
	rm -f pe *.o *.so *.pcm *.d *~
	rm -f plot_all *.o *.so *.pcm *.d *~

