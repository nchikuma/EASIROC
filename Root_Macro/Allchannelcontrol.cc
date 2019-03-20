//#include <Fit_Histogram.cc>

const int minbin = 600;
const int maxbin = 2000;
const double peakSearchThreshold = 0.1;
const int peakSearchSigma = 2;
const int maxNumPeaks = 10;
const double FitHeight = 300.;
const double FitRange = 20.;
const double FitSigma = 10.;

const char* Title = "All channels operation";
const char* xAxisTitle = "MPPC channel";
const char* yAxisTitle = "Counts";

TFile* f_open;
std::stringstream hist_name;
std::stringstream f1_name;
TH1D* h1;
TF1* f1[maxNumPeaks];
Double_t GausParameters[3][maxNumPeaks];
Int_t npeaks = 0;

//const char* readFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20151006/InputDAC_ch40_400.root";
const char* readFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/mppc_all/mppc_0030_0105.root";

const bool PrintOpt = true;
//const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/MPPCoperation.png";
const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/MPPCoperation2.png";

const char* para = "adc";
const int  HighLow = 1;

const int xMin  = 0;
const int xMax  = 32;
const int nBinx = 32;
const int yMax  = 900;
const int yMin  = 700;
const int nBiny = yMax-yMin;

void Allchannelcontrol(){

  TCanvas* c1 = new TCanvas("c1");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);

  stringstream filename_ss;
  filename_ss << readFileName;	  
  FileOpen(filename_ss.str().c_str());
  
  TH2I *Graph = new TH2I("mppc","mppc",nBinx,xMin,xMax,nBiny,yMin,yMax);
  int pedestal = 0;
  for(int ch=0;ch<32;ch++){
	  DrawHist(para,HighLow,ch,0);
	  PeakSearch(h1,GausParameters[1]);
	  //GaussianFit(h1,GausParameters);	
	  SortAscendingOrder(GausParameters,npeaks,1);
	  pedestal = GausParameters[1][0];
	  //Fit_Histogram(filename_ss.str().c_str(), para,HighLow, ch, 0, 0);
	  for(int j=yMin;j<=yMax;j++){
		  Graph->Fill(ch,j-pedestal+750,h1->GetBinContent(j));
	  }
  }
  

  Graph->SetTitle("32-channel arrayed MPPC Operation");
  Graph->GetXaxis()->SetTitle("channel");
  Graph->GetXaxis()->SetTitleOffset(1.3);
  Graph->GetYaxis()->SetTitle("ADC value");
  Graph->GetZaxis()->SetRangeUser(0,500);
  
  Graph->Draw("colz");

  if(PrintOpt) c1->Print(PrintFileName);

};

void FileOpen(char *file){
	f_open = TFile::Open(file);
	cout << ">>>> Open a file '" << file <<"'." << endl;
};

void DrawHist(char* para, int hl, int ch, double &mean, char* op = "",int color = 4,int style = 0){

	if(para == "adc"){
		hist_name << "adc_";
		if(hl == 1) hist_name << "high_" << ch;
		else        hist_name << "low_"  << ch;
	}
	else if(para == "tdc"){
		hist_name << "tdc_";
		if(hl == 1) hist_name << "leading_"  << ch;
		else        hist_name << "trailing_" << ch;
	}
	else if(para == "scaler"){
		hist_name << "scaler_";
		if(ch == 64) hist_name << "rate_or32u";
		else if(ch == 65) hist_name << "rate_or32l";
		else if(ch == 66) hist_name << "rate_or64";
		else hist_name << ch;
	}
	
	cout << ">>>> Draw a histgram '" << hist_name.str() <<"'." << endl;

	h1 = (TH1D*) gROOT->FindObject(hist_name.str().c_str());
	h1->GetXaxis()->SetRangeUser(minbin,maxbin);
	h1->SetTitle(Title);
	h1->GetXaxis()->SetTitle(xAxisTitle);
	h1->GetYaxis()->SetTitle(yAxisTitle);
	((TGaxis*)h1->GetYaxis())->SetMaxDigits(3);
	h1->SetLineColor(color);
	h1->SetFillColor(color);
	h1->SetFillStyle(style);
	h1->Draw(op);

	mean = h1->GetMean();

	hist_name.str("");

};

void PeakSearch(TH1D* hist, double* peakBins){
	TSpectrum *s = new TSpectrum(maxNumPeaks);
	npeaks = s->Search(hist,peakSearchSigma,"q",peakSearchThreshold);
	cout << ">>>> Found '" << npeaks << "' candidate peaks to fit. " << endl;;

	for(int i=0;i<maxNumPeaks;i++){
		if(i<npeaks){
			if(i==0) cout << "      Peak positions: ";
			peakBins[i] = s->GetPositionX()[i];
			cout << peakBins[i] << "  ";
		}
	}
	cout << endl;
};


void SortAscendingOrder(double array[3][maxNumPeaks],int Narray,int iOrder){
	double tmp;
	for (int i=0; i<Narray; ++i) {
		for (int j=i+1; j<Narray; ++j) {
			if (array[iOrder][i] > array[iOrder][j]) {
				for(int k=0;k<3;k++){
					tmp =  array[k][i];
					array[k][i] = array[k][j];
					array[k][j] = tmp;
				}
			}
		}
	}
};
