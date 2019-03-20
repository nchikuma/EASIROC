#include <Fit_Histogram.cc>

const char* ReadFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20151006/InputDAC_ch40_";

const char* Parameter = "adc";
const int HighLow = 1;
const int channel = 40;
	
const double xmax = 4.;
const double xmin = 0.;
const double ymax = 30.;
const double ymin = 0.;
const int nbin = 10000;
double binWidth = (xmax - xmin)/nbin;

const double textPosX = 2.3;
const double textPosY = 16.0;
const double textSize = 0.05;

const bool PrintOpt_GainHist = true;
const char* PrintName_GainHist = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/InputDAC_Control.png";

void GainHist(){

	TCanvas *c1 = new TCanvas("c1");
	gStyle->SetOptStat(0);

	stringstream filename_ss;
	double gain = 0.;
	double sigma = 0.;

	double xx[4],yy[4],xe[4],ye[4];
	for(int i=0;i<4;i++){
		filename_ss << ReadFileName << 400+i*25 << ".root";
	
		cout << filename_ss.str().c_str() << endl;
		//Fit_Histogram(filename,Parameter,HighLow,channel);
		//Fit_Histogram(filename_ss.str().c_str(),Parameter,HighLow,channel,gain);
		Fit_Histogram(filename_ss.str().c_str(),Parameter,HighLow,channel,gain,sigma);

		//cout << "gain:" << gain << endl;
		//cout << "sigma:" << sigma << endl;
		xx[i] = (double) i*25*0.0184;
		yy[i] = gain;
		xe[i] = 0.;
		ye[i] = sigma;
		filename_ss.str("");
	};
	TGraphErrors *GainPlot = new TGraphErrors(4,xx,yy,xe,ye);
	
	*GainPlot->Fit("pol1","qs");

	double par0 = pol1->GetParameter(0);
	double par1 = pol1->GetParameter(1);
	double offset = -par0/par1;

	c1->Clear();
	TH1D* hist = new TH1D("","",nbin,xmin,xmax);
	hist->SetMinimum(ymin);
	hist->SetMaximum(ymax);
	for(int i=0;i<4;i++){
		xx[i] -= offset;
		hist->SetBinContent((xx[i]-xmin)/binWidth + 1,yy[i]);
		//hist->SetBinError((xx[i]-xmin)/binWidth + 1,ye[i]);
	}

	hist->SetMarkerStyle(3);
	hist->SetMarkerSize(3);
	hist->SetTitle("InputDAC Control");
	hist->GetXaxis()->SetTitle("Over voltage [V]");
	hist->GetYaxis()->SetTitle("Gain (ADC value)");

	hist->Draw("p");

	pol1->SetParameter(0,0.);
	pol1->SetParameter(1,par1);
	pol1->SetRange(xmin,xmax);
	pol1->Draw("same");
	
	stringstream FuncName;
	FuncName.precision(3);
	FuncName << "Y = " << par1 << "* X + 0.0";

	TText* t=new TText(textPosX,textPosY,FuncName.str().c_str());
	t->SetTextSize(textSize);
	t->SetTextColor(kRed);
	t->Draw("");
	
	c1->Update();
	if(PrintOpt_GainHist) c1->Print(PrintName_GainHist);

};
