#include <Fit_Histogram.cc>

const char* readFileName_DoublePulse = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20150924/DoublePulse_60ns.root";
const char* Parameter = "tdc";
const int channel = 4;

const bool PrintOpt_DoublePulse = true;
const char* PrintFileName_DoublePulse = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/DoublePulse_60ns.png";

void DoublePulse(){

	gStyle->SetOptStat(0);

	FileOpen(readFileName_DoublePulse);
	if(!f_open){
		cerr << "No file: '" << filename<< "'.";
		return;
	}
	
	TLegend *leg = new TLegend(0.6,0.75,0.8,0.85);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);

	DrawHist(Parameter,1,channel,"",4,3004);
	leg->AddEntry(h1,"Leading Edge","f");
	PeakSearch(h1,GausParameters[1]);
	DrawHist(Parameter,0,channel,"same",2,3001);
	leg->AddEntry(h1,"Trailing Edge","f");
	PeakSearch(h1,GausParameters[1]);
	leg->Draw();

	

	TText* t1=new TText(467,2000,"-----The first pulse-----");
	t1->SetTextSize(0.035);
	t1->SetTextColor(1);
	t1->Draw("");
	TText* t2=new TText(412,2000,"--The second pulse--");
	t2->SetTextSize(0.035);
	t2->SetTextColor(1);
	t2->Draw("");

	c1->Update();

	//if(PrintOpt_DoublePulse) c1->Print(PrintFileName_DoublePulse);


}
