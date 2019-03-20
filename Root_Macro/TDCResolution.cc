#include <Fit_Histogram.cc>

const char* readFileName_TDC = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20150926/AllOn_8days.root";
const char* Parameter = "tdc";
const int HighLow = 1;
const int channel = 1;

const bool PrintOpt_TDC = true;
const char* PrintFileName_TDC = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/TDC_TimeResolution.png";

void TDCResolution(){

	gStyle->SetOptFit(0111);
	
	double gain = 0.;
	double sigma = 0.;
	Fit_Histogram(readFileName_TDC,Parameter,HighLow,channel,gain,sigma);

	cout << sigma << endl;
	if(PrintOpt_TDC) c1->Print(PrintFileName_TDC);

};
	

