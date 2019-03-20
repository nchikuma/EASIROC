//#include <ifstream>
#include <sstream>

const int nData = 1500;
const int nGraph = 3;
const int iGraph = 1;
const int fGrapn = 2;

const char* readFileName = "HoldTiming.txt";

const bool PrintOpt = true;
const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/HoldTiming.png";

double xx[nData],y[3][nData];

void HoldTiming(){

  TCanvas* c1 = new TCanvas("c1");
  c1->SetGrid(1);

  ifstream ifs(readFileName);

  for(int i=0;i<nData;i++){
	  for(int j=0;j<nGraph;j++){
		  ifs >> y[j][i];
		  y[j][i] *= 40.;
	  }
	  xx[i] = 2.*i;
  }
  
    
  TGraph *sshHG[nGraph];
  for(int i=0;i<nGraph;i++) sshHG[i] = new TGraph(nData,xx,y[i]);

  double width = 3.;
  for(int i=iGraph;i<=fGrapn;i++) sshHG[i]->SetLineStyle(i);
  int color_int = 0;
  for(int i=iGraph;i<=fGrapn;i++){
	  color_int++;
	  if(color_int==5||color_int==7||color_int==10) color_int++;
	  sshHG[i]->SetLineColor(color_int);
  }
  for(int i=iGraph;i<=fGrapn;i++) sshHG[i]->SetLineWidth(width);
  sshHG[iGraph]->SetTitle("Slow Shaper (HG)");
  sshHG[iGraph]->GetXaxis()->SetTitle("Time [ns]");
  sshHG[iGraph]->GetXaxis()->SetTitleOffset(1.3);
  sshHG[iGraph]->GetYaxis()->SetTitle("Amplitude [mV]");
  sshHG[iGraph]->Draw("ac");
  for(int i=iGraph+1;i<=fGrapn;i++)  sshHG[i]->Draw("same c");


  TLegend *leg = new TLegend(0.50,0.15,0.7,0.3);
  leg->SetBorderSize(1);
  leg->SetFillColor(kWhite);
  //leg->SetFillStyle(3000);
  stringstream ss_graph = "";
  for(int i=iGraph;i<=fGrapn;i++){
	  if(i==0) ss_graph << "Early hold timinig";
	  if(i==1) ss_graph << "Good hold timinig";
	  if(i==2) ss_graph << "Late hold timinig";
	  leg->AddEntry(sshHG[i],ss_graph.str().c_str(),"l");
	  ss_graph.str("");
  }
  leg->Draw("");


  if(PrintOpt) c1->Print(PrintFileName);

};
