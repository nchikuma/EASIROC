#include <Fit_Histogram.cc>

const char* ReadFileName_ADCtoCharge = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/Readout/data/20150924/Linearity_";

const char* Parameter = "adc";
const int channel = 4;
const int NumReadFile_ADCtoCharge = 14;

const bool PrintOpt_ADCtoCharge = true;
const char* PrintFileName_ADCtoCharge = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/ADCtoCharge.png";

void ADCtoCharge(){
	
	TCanvas *c1 = new TCanvas("c1");
	gStyle->SetOptStat(0);

	stringstream filename_ss;
	double gain = 0.;
	double sigma = 0.;

	double xxh[NumReadFile_ADCtoCharge],yyh[NumReadFile_ADCtoCharge],xeh[NumReadFile_ADCtoCharge],yeh[NumReadFile_ADCtoCharge];
	double xxl[NumReadFile_ADCtoCharge],yyl[NumReadFile_ADCtoCharge],xel[NumReadFile_ADCtoCharge],yel[NumReadFile_ADCtoCharge];
	for(int i=0;i<NumReadFile_ADCtoCharge;i++){
		filename_ss << ReadFileName_ADCtoCharge;
		int inputV;
		if(i<=1)       inputV = 10*(i+1);
		else if(i<=3)  inputV = 50+30*(i-2);
		else if(i<=6)  inputV = 50*(i-2);
		else if(i<=11) inputV = 100*(i-5);
		else if(i<=13) inputV = 750+250*(i-12);
		else if(i<=14) inputV = 1500;
		filename_ss << inputV << "mV.root";
			
		cout << filename_ss.str().c_str() << endl;
		Fit_Histogram(filename_ss.str().c_str(),Parameter,1,channel,gain,sigma);
		xxh[i] = inputV*0.047; //pC
		yyh[i] = gain;
		xeh[i] = 0.;
		yeh[i] = sigma;
		Fit_Histogram(filename_ss.str().c_str(),Parameter,0,channel,gain,sigma);
		xxl[i] = inputV*0.047; //pC
		yyl[i] = gain;
		xel[i] = 0.;
		yel[i] = sigma;

		filename_ss.str("");
	};

	TGraphErrors *ADCtoChargePlotH = new TGraphErrors(NumReadFile_ADCtoCharge,xxh,yyh,xeh,yeh);
	TGraphErrors *ADCtoChargePlotL = new TGraphErrors(NumReadFile_ADCtoCharge,xxl,yyl,xel,yel);
	ADCtoChargePlotH->SetMarkerStyle(2);
	ADCtoChargePlotL->SetMarkerStyle(5);
	ADCtoChargePlotH->SetMarkerSize(2);
	ADCtoChargePlotL->SetMarkerSize(2);
	ADCtoChargePlotH->SetMarkerColor(kRed);
	ADCtoChargePlotL->SetMarkerColor(kBlue);
	ADCtoChargePlotH->SetTitle("ADC dependency on input charge");
	ADCtoChargePlotH->GetXaxis()->SetTitle("Input charge [pC]");
	((TGaxis*)ADCtoChargePlotH->GetYaxis())->SetMaxDigits(3);
	ADCtoChargePlotH->GetYaxis()->SetTitle("ADC value");
	ADCtoChargePlotH->Draw("ap");
	ADCtoChargePlotL->Draw("same p");

	TLegend *leg = new TLegend(0.7,0.3,0.9,0.4);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(ADCtoChargePlotH,"High Gain","lep");
	leg->AddEntry(ADCtoChargePlotL,"Low Gain","lep");
	leg->Draw();

	c1->Update();
	if(PrintOpt_ADCtoCharge) c1->Print(PrintFileName_ADCtoCharge);
}
