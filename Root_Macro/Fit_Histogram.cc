#include <sstream>
#include <math>

const int minbin = 600;
const int maxbin = 2000;
const double peakSearchThreshold = 0.1;
const int peakSearchSigma = 2;
const int maxNumPeaks = 10;
const double FitHeight = 300.;
const double FitRange = 20.;
const double FitSigma = 10.;

const bool PrintOpt_Fit = false;
const char* PrintName_Fit = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/GainFit_example.png";
//const char* Title = "ADC High Gain ch40";
//const char* Title = "TDC Leading Edge ch1";
//const char* Title = "TDC. Double pulses input.";
//const char* Title = "Scaler test";
const char* Title = "Incalib test (InputDAC: ON)";
//const char* Title = "Incalib test (InputDAC: OFF)";
//const char* xAxisTitle = "ADC value";
//const char* xAxisTitle = "TDC value";
//const char* xAxisTitle = "Scaler counts [/1e+4 s]";
const char* xAxisTitle = "ADC value";
const char* yAxisTitle = "Counts";


TFile* f_open;
std::stringstream hist_name;
std::stringstream f1_name;
TH1D* h1;
TF1* f1[maxNumPeaks];
Double_t GausParameters[3][maxNumPeaks];
Int_t npeaks = 0;

//void FileOpen(char *filename);
//void DrawHist(char* para, int hl, int ch, char* op = "",int color = 4,int style = 0){
//void PeakSearch(TH1D* hist,double* peakBins);
//void GaussianFit(TH1D* hist,double* peakPos);
//void GaussianFit(TH1D* hist,double Parameters[3][maxNumPeaks]);
//void SortAscendingOrder(double* array, int Narray);
//void SortAscendingOrder(double array[3][maxNumPeaks],int Narray,int iOrder)

//void Fit_Histogram(char* filename, char* Parameter, int HighLow, int channel){
//void Fit_Histogram(char* filename, char* Parameter, int HighLow, int channel, double &gain){
void Fit_Histogram(char* filename, char* Parameter, int HighLow, int channel, double &gain, double &sigma){

	bool arguments = false;
	if(Parameter=="adc"||Parameter=="tdc"){
		if(HighLow==0||HighLow==1)
			if(channel>=0&&channel<=63) arguments = true;
	}
	else if(Parameter=="scaler"){
		if(channel>=0&&channel<=66) arguments = true;
	}
	
	if(!arguments){
		cerr << ".x Fit_Histogram(char* filename, char* Parameter, int HighLow, int channel) \n";
		cerr << "Parameter = 'adc'/'tdc'/'scaler', HighLow = 0/1, channel = 0-63(66)) \n";
		return;
	}
	
	FileOpen(filename);
	if(!f_open){
		cerr << "No file: '" << filename<< "'.";
		return;
	}

	DrawHist(Parameter,HighLow,channel,0);
	PeakSearch(h1,GausParameters[1]);

	if(npeaks==0) return;
	GaussianFit(h1,GausParameters);	
	if(PrintOpt_Fit) c1->Print(PrintName_Fit);

	//SortAscendingOrder(GausParameters[1],npeaks);
	SortAscendingOrder(GausParameters,npeaks,1);
	
	gain = 0.;
	//for(int i=0;i<npeaks-1;i++){
	//	gain += GausParameters[1][i+1]-GausParameters[1][i];
	//}
	//gain /= (npeaks - 1);
	//gain = GausParameters[1][2] - GausParameters[1][1];  //2pe - 1pe.
	//gain = GausParameters[1][0];  //the first peak postion.
	if(npeaks==1) gain = 0;
	else gain = h1->Integral(GausParameters[1][0]-150,GausParameters[1][0]+150); //the counts of the first peak.
	cout << "gain:" << gain << endl;

	sigma = 0.;
	//for(int i=0;i<npeaks-1;i++) sigma += GausParameters[2][i];
	//sigma /= (npeaks - 1);
	//sigma = sqrt(GausParameters[2][2]*GausParameters[2][2]+GausParameters[2][1]*GausParameters[2][1]); //2pe - 1pe
	//sigma = GausParameters[2][0]; //the first peak's width.
	sigma = sqrt(gain); //the first peak's width.
	cout << "sigma" << sigma << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

void FileOpen(char *file){
	f_open = TFile::Open(file);
	cout << ">>>> Open a file '" << file <<"'." << endl;
};

//void DrawHist(char* para, int hl, int ch, char* op = "",int color = 4,int style = 0){
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

//void GaussianFit(TH1D* hist,double* peakHeight, double* peakPos, double* peakWidth){
void GaussianFit(TH1D* hist,double Parameters[3][maxNumPeaks]){

	cout << ">>>> Fit with gaussiun around candidate peaks." << endl;

	double ini_para[3];
	double range_min, range_max;

	for(int i=0;i<npeaks;i++){
		ini_para[0] = FitHeight;
		ini_para[1] = Parameters[1][i];
		ini_para[2] = FitSigma/2;
		range_min   = ini_para[1] - FitRange/2;
		range_max   = ini_para[1] + FitRange/2;

		f1_name << "f1_";
		f1_name << i;
		f1[i] = new TF1(f1_name.str().c_str(),"gaus(0)",range_min,range_max);
		f1[i]->SetParameter(0,ini_para[0]);
		f1[i]->SetParameter(1,ini_para[1]);
		f1[i]->SetParameter(2,ini_para[2]);

		hist->Fit(f1_name.str().c_str(),"q+","",range_min,range_max);
		f1[i]->Draw("same");
		f1_name.str("");

		if(i==0) cout << "      Parameters after fit:|| ";
		for(int j=0;j<3;j++){
			Parameters[j][i] = f1[i]->GetParameter(j);
			cout << "para(" << j << ")=" << Parameters[j][i] << " ";
			if(j==2){
				cout << endl;
				if(i!=npeaks-1) cout <<"                           || ";
			}
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

//void SortAscendingOrder(double* array,int Narray){
//	double tmp;
//	for (int i=0; i<Narray; ++i) {
//		for (int j=i+1; j<Narray; ++j) {
//			if (array[i] > array[j]) {
//				tmp =  array[i];
//				array[i] = array[j];
//				array[j] = tmp;
//			}
//		}
//	}
//};
