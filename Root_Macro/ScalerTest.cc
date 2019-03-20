#include <sstream>
#include <math>

const int minbin = 0;
const int maxbin = 5000;
const double peakSearchThreshold = 0.1;
const int peakSearchSigma = 2;
const int maxNumPeaks = 10;
const double FitHeight = 300.;
const double FitRange = 20.;
const double FitSigma = 10.;

const char* Title = "Scaler test";
const char* xAxisTitle = "Scaler counts [/1e+4 s]";
const char* yAxisTitle = "Counts";


TFile* f_open;
std::stringstream hist_name;
std::stringstream f1_name;
TH1D* h1;
TF1* f1[maxNumPeaks];
Double_t GausParameters[3][maxNumPeaks];
Int_t npeaks = 0;


const int nData = 22-4;

const char* readFileName_Scaler = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20151006/scaler/ScalerTest_";

const bool PrintOpt_Scaler = true;
const char* PrintFileName_Scaler = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/ScalerTest.eps";

const char* para_scaler = "scaler";
const int  channel = 66;

double inFreq  = 0.;
double inFerr  = 0.;
double mean_scaler = 0.;
double sigma_scaler = 0.;

double xx[nData],xe[nData],yy[nData],ye[nData];

void ScalerTest(){

  TCanvas* c1 = new TCanvas("c1");
  c1->SetLogy();
  c1->SetLogx();
  c1->SetGrid(1);

  stringstream filename_ss;
  for(int i=0;i<nData;i++){
    if(i==0){
	    filename_ss<<readFileName_Scaler<<"1kHz"<<"_async_scaler.root";
	    inFreq = 1.;
    }
    else if(i<=5){
	    filename_ss<<readFileName_Scaler <<10*i<<"kHz"<<"_async_scaler.root";
	    inFreq = (double) 10*i;
    }
    else if(i<=14){
	    filename_ss<<readFileName_Scaler<<100*(i-5)<<"kHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 100*(i-5);
    }
    else if(i==15){
	    filename_ss<<readFileName_Scaler<< "1MHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 1000;
    }    
    else if(i==16){
	    filename_ss<<readFileName_Scaler<< "1.5MHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 1500;
    }    
    else if(i==17){
	    filename_ss<<readFileName_Scaler<< "2MHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 2000;
    }
    else if(i==18){
	    filename_ss<<readFileName_Scaler<< "4MHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 4000;
    }    
    else if(i==19){
	    filename_ss<<readFileName_Scaler<< "10MHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 10000;
    }    
    else if(i==20){
	    filename_ss<<readFileName_Scaler<< "12MHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 12000;
    }        
    else if(i==21){
	    filename_ss<<readFileName_Scaler<< "15MHz"<<"_async_read1kHz_scaler.root";
	    inFreq = (double) 15000;
    }    

    FileOpen(filename_ss.str().c_str());
    DrawHist(para_scaler,0,channel,mean_scaler);
    mean_scaler -= 0.5;
    mean_scaler *= 10000;
    sigma_scaler = sqrt(mean_scaler);
    cout << "mean_scaler:" << mean_scaler << " sigma_scaler:" << sigma_scaler
	 << endl
	 << "## difference:" << 100 - (mean_scaler/10000./inFreq*100) << endl;

    xx[i] = inFreq;
    xe[i] = inFerr;
    yy[i] = mean_scaler/10000.;
    ye[i] = sigma_scaler/10000.;

    filename_ss.str("");
  }
  
  TGraphErrors *scalerGraph = new TGraphErrors(nData,xx,yy,xe,ye);

  scalerGraph->SetMarkerStyle(21);
  scalerGraph->SetMarkerSize(1);
  //scalerGraph->SetTitle("Scaler test");
  scalerGraph->SetTitle("");
  scalerGraph->GetXaxis()->SetTitle("Input pulse frequency [kHz]");
  scalerGraph->GetXaxis()->SetTitleOffset(1.3);
  scalerGraph->GetYaxis()->SetTitle("Scaler counts [/ms]");
  
  scalerGraph->Draw("ap");

  TF1* f1_scaler = new TF1("f1_scaler","pol1(0)",0.,4000.);
  f1_scaler->SetLineColor(1);
  //scalerGraph->Fit("f1_scaler","+","",1.,4000.);
  f1_scaler->SetParameter(0,0.);
  f1_scaler->SetParameter(1,1.);
  f1_scaler->SetRange(0.,4000.);
  f1_scaler->SetLineStyle(2);
  f1_scaler->SetLineColor(2);
  f1_scaler->Draw("same");
  /*TF1* f2_scaler = new TF1("f2_scaler","pol1(0)",4000.,20000.);
  f2_scaler->SetLineColor(1);
  //scalerGraph->Fit("f2_scaler","+","",10000.,20000.);
  f2_scaler->SetParameter(0,4095.);
  f2_scaler->SetParameter(1,0.);
  f2_scaler->SetRange(4000.,20000.);
  f2_scaler->SetLineStyle(2);
  f2_scaler->SetLineColor(2);
  f2_scaler->Draw("same");*/

  TLegend *leg = new TLegend(0.70,0.25,0.88,0.35);
  leg->SetBorderSize(1);
  leg->SetFillColor(kWhite);
  //leg->SetFillStyle(3000);
  leg->AddEntry(scalerGraph,"Data","p");
  leg->AddEntry(f1_scaler,"Linear line (y=x)","l");
  leg->Draw("");


  if(PrintOpt_Scaler) c1->Print(PrintFileName_Scaler);

};


void FileOpen(char *file){
	f_open = TFile::Open(file);
	cout << ">>>> Open a file '" << file <<"'." << endl;
};

void DrawHist(char* para, int hl, int ch, double &mean, char* op = "",int color = 4,int style = 0){

	if(para == "adc"){
		hist_name << "ADC_";
		if(hl == 1) hist_name << "HIGH_" << ch;
		else        hist_name << "LOW_"  << ch;
	}
	else if(para == "tdc"){
		hist_name << "TDC_";
		if(hl == 1) hist_name << "LEADING_"  << ch;
		else        hist_name << "TRAILING_" << ch;
	}
	else if(para == "scaler"){
		hist_name << "SCALER_";
		if(ch == 64) hist_name << "OR32U";
		else if(ch == 65) hist_name << "OR32L";
		else if(ch == 66) hist_name << "OR64";
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


