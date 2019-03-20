//#include <ifstream>
#include <sstream>

const int nData = 4000;
const int nGraph = 2;
const int iGraph = 1;
const int fGrapn = 1;

const char* readFileName = "clock1.txt";
const char* readFileName1 = "clock2.txt";

const bool PrintOpt = true;
const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/ClockOut2.png";

double xx[nData],y[nGraph][nData];

void ClockOut(){

  TCanvas* c1 = new TCanvas("c1");
  c1->SetGrid(1);

  ifstream ifs(readFileName);
  ifstream ifs1(readFileName1);

  cout << "reading...\n";
  for(int i=0;i<nData;i++){
	  ifs >> y[0][i];
	  ifs1 >> y[1][i];
	  y[0][i] *=2;
	  y[1][i] *=2;
	  xx[i] = i/1000.;
  }
  cout << "reading finished.\n";
  
    
  TGraph *graph[nGraph];
  for(int i=0;i<2;i++) graph[i] = new TGraph(nData,xx,y[i]);

  double width = 2;
  for(int i=iGraph;i<=fGrapn;i++) graph[i]->SetLineStyle(i);
  for(int i=iGraph;i<=fGrapn;i++) graph[i]->SetMarkerStyle(i);
  int color_int = 0;
  for(int i=iGraph;i<=fGrapn;i++){
	  color_int++;
	  if(color_int==5||color_int==7||color_int==10) color_int++;
	  graph[i]->SetLineColor(color_int);
  }
  for(int i=iGraph;i<=fGrapn;i++) graph[i]->SetLineWidth(width);
  for(int i=iGraph;i<=fGrapn;i++) graph[i]->SetMarkerSize(1);
  graph[iGraph]->SetTitle("User Clock Out");
  graph[iGraph]->GetXaxis()->SetTitle("Time [us]");
  graph[iGraph]->GetXaxis()->SetTitleOffset(1.3);
  graph[iGraph]->GetYaxis()->SetTitle("Amplitude [mV]");
  graph[iGraph]->GetYaxis()->SetRangeUser(-120,20);
  graph[iGraph]->Draw("al");
  for(int i=iGraph+1;i<=fGrapn;i++) if(i!=2) graph[i]->Draw("same l");


  TLegend *leg = new TLegend(0.70,0.15,0.85,0.2);
  leg->SetBorderSize(1);
  leg->SetFillColor(kWhite);
  //leg->SetFillStyle(3000);
  stringstream ss_graph = "";
  for(int i=iGraph;i<=fGrapn;i++){
	  if(i==0) ss_graph << "100 kHz";
	  if(i==1) ss_graph << "3 MHz";
	  if(i==1)leg->AddEntry(graph[i],ss_graph.str().c_str(),"l");
	  ss_graph.str("");
  }
  leg->Draw("");
  


  if(PrintOpt) c1->Print(PrintFileName);

};
