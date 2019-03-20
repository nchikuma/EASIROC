#include <Fit_Histogram.cc>

const char* ReadFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20150924/AcceptDelay_";

const char* Parameter = "adc";
const int HighLow = 1;
const int channel = 4;
	
const double xmax = 3100.;
const double xmin = 900.;
const double ymax = 110.;
const double ymin = 0.;
const int nbin = 10000;
double binWidth = (xmax - xmin)/nbin;

const double textPosX = 2.3;
const double textPosY = 16.0;
const double textSize = 0.05;

const bool PrintOpt_AcceptDelay = true;
const char* PrintName_AcceptDelay = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/AcceptDelay.png";

const nData = 7;

void AcceptDelay(){

	TCanvas *c1 = new TCanvas("c1");
	gStyle->SetOptStat(0);

	stringstream filename_ss;
	double gain = 0.;
	double sigma = 0.;
	int delay = 0.;

	double xx[nData],yy[nData],xe[nData],ye[nData];
	for(int i=0;i<nData;i++){
		if(i==0) delay = 1000;
		if(i==1) delay = 2000;
		if(i==2) delay = 2050;
		if(i==3) delay = 2080;
		if(i==4) delay = 2100;
		if(i==5) delay = 2500;
		if(i==6) delay = 3000;
		
		//if(i==2) filename_ss << ReadFileName << delay << "_again.root";
		     filename_ss << ReadFileName << delay << ".root";
		//filename_ss << "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20150923/AllOn_3.root";
	
		cout << filename_ss.str().c_str() << endl;
		Fit_Histogram(filename_ss.str().c_str(),Parameter,HighLow,channel,gain,sigma);

		//cout << "gain:" << gain << endl;
		//cout << "sigma:" << sigma << endl;
		xx[i] = (double) delay;
		yy[i] = 1. - gain/10000.;
		xe[i] = 0.;
		ye[i] = sigma/10000.;
		filename_ss.str("");
	};
	
	TGraphErrors *Graph = new TGraphErrors(nData,xx,yy,xe,ye);
	
	Graph->SetMarkerStyle(21);
	Graph->SetMarkerSize(1);
	Graph->SetTitle("Scaler test");
	Graph->GetXaxis()->SetTitle("Input pulse [kHz]");
	Graph->GetXaxis()->SetTitleOffset(1.3);
	Graph->GetYaxis()->SetTitle("Scaler counts [/kHz]");
	
	Graph->Draw("ap");
	
	//TF1* f1_scaler = new TF1("f1_scaler","pol1(0)",0.,4000.);
	//f1_scaler->SetLineColor(1);
	////Graph->Fit("f1_scaler","+","",1.,4000.);
	//f1_scaler->SetParameter(0,0.);
	//f1_scaler->SetParameter(1,1.);
	//f1_scaler->SetRange(0.,4000.);
	//f1_scaler->SetLineStyle(2);
	//f1_scaler->SetLineColor(2);
	//f1_scaler->Draw("same");
	//TF1* f2_scaler = new TF1("f2_scaler","pol1(0)",4000.,20000.);
	//f2_scaler->SetLineColor(1);
	////Graph->Fit("f2_scaler","+","",10000.,20000.);
	//f2_scaler->SetParameter(0,4095.);
	//f2_scaler->SetParameter(1,0.);
	//f2_scaler->SetRange(4000.,20000.);
	//f2_scaler->SetLineStyle(2);
	//f2_scaler->SetLineColor(2);
	//f2_scaler->Draw("same");
	//
	//TLegend *leg = new TLegend(0.70,0.25,0.88,0.35);
	//leg->SetBorderSize(1);
	//leg->SetFillColor(kWhite);
	////leg->SetFillStyle(3000);
	//leg->AddEntry(Graph,"Data","p");
	//leg->AddEntry(f1_scaler,"Linear line (y=x,4095)","l");
	//leg->Draw("");
	//
	//
	//if(PrintOpt_Scaler) c1->Print(PrintFileName_Scaler);


};
