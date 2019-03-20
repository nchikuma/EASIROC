#include <Fit_Histogram.cc>

const char* readFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20151010/InCalib_DAC400_ADConly.root";
//const char* readFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20151010/InCalib_DAC100_ADConly.root";

const bool PrintOpt = true;
const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/Incalib_test_ok.png";
//const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/Incalib_test_ng.png";

const char* para = "adc";
const int HighLow = 1;
const int ch = 1;

const int xMin  = 0;
const int xMax  = 64;
const int nBinx = 64;
const int yMax  = 2000;
const int yMin  = 0;
const int nBiny = yMax-yMin;

void Incalib_test(){

  TCanvas* c1 = new TCanvas("c1");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  c1->SetLogy();

  stringstream filename_ss;
  filename_ss << readFileName;	  
  FileOpen(filename_ss.str().c_str());
  
  DrawHist(para,HighLow,ch,0);

  if(PrintOpt) c1->Print(PrintFileName);

};
