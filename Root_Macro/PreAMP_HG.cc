//#include <ifstream>
#include <sstream>

const int nData = 41;
const int iGraph = 0;
const int fGrapn = 8;

const char* readFileName = "PreAMP_HG.txt";

const bool PrintOpt = true;
const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/PreAMP_HG.png";

double xx[nData],y[15][nData];

void PreAMP_HG(){

  TCanvas* c1 = new TCanvas("c1");
  c1->SetGrid(1);

  ifstream ifs(readFileName);

  for(int i=0;i<nData;i++){
	  for(int j=0;j<15;j++){
		  ifs >> y[j][i];
		  y[j][i] *= 20.;
	  }
	  xx[i] = 50.*i;
  }
  
    
  TGraph *paHG[15];
  for(int i=0;i<15;i++) paHG[i] = new TGraph(nData,xx,y[i]);

  double width = 3.;
  //paHG[0]->SetLineStyle(0);
  //paHG[1]->SetLineStyle(1);
  for(int i=iGraph;i<=fGrapn;i++) paHG[i]->SetLineStyle(i);
  int color_int = 0;
  for(int i=iGraph;i<=fGrapn;i++){
	  color_int++;
	  if(color_int==5||color_int==7||color_int==10) color_int++;
	  paHG[i]->SetLineColor(color_int);
  }
  for(int i=iGraph;i<=fGrapn;i++) paHG[i]->SetLineWidth(width);
  paHG[iGraph]->SetTitle("Preamplifier (HG)");
  paHG[iGraph]->GetXaxis()->SetTitle("Time [ns]");
  paHG[iGraph]->GetXaxis()->SetTitleOffset(1.3);
  paHG[iGraph]->GetYaxis()->SetTitle("Amplitude [mV]");
  paHG[iGraph]->Draw("ac");
  for(int i=iGraph+1;i<=fGrapn;i++)  paHG[i]->Draw("same c");


  TLegend *leg = new TLegend(0.70,0.15,0.88,0.7);
  leg->SetBorderSize(1);
  leg->SetFillColor(kWhite);
  //leg->SetFillStyle(3000);
  stringstream ss_graph = "";
  for(int i=iGraph;i<=fGrapn;i++){
	  ss_graph << 100*(i+1) << "fF";
	  leg->AddEntry(paHG[i],ss_graph.str().c_str(),"l");
	  ss_graph.str("");
  }
  leg->Draw("");


  if(PrintOpt) c1->Print(PrintFileName);

};
