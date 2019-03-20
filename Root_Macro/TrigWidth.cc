//#include <ifstream>
#include <sstream>

const int nData = 600;
const int nGraph = 4;
const int iGraph = 0;
const int fGrapn = 3;

const char* readFileName = "TrigWidth.txt";
const char* readFileName1 = "TrigWidth1.txt";

const bool PrintOpt = true;
const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/TrigWidth.png";

double xx[2][nData],y[nGraph][nData];

void TrigWidth(){

  TCanvas* c1 = new TCanvas("c1");
  c1->SetGrid(1);

  ifstream ifs(readFileName);
  ifstream ifs1(readFileName1);

  cout << "reading...\n";
  for(int i=0;i<nData;i++){
	  ifs >> xx[0][i] >> y[0][i] >> y[1][i];
	  ifs1 >> xx[1][i] >> y[2][i] >> y[3][i];
	  xx[0][i] = (xx[0][i]-1700)*2;
	  xx[1][i] = (xx[1][i]-1700)*2;
	  y[0][i] /= 6;
	  y[1][i] *=2;
	  y[3][i] *=2;
  }
  cout << "reading finished.\n";
  
    
  TGraph *graph[nGraph];
  for(int i=0;i<2;i++) graph[i] = new TGraph(nData,xx[0],y[i]);
  for(int i=0;i<2;i++) graph[i+2] = new TGraph(nData,xx[1],y[i+2]);

  double width = 3.;
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
  graph[iGraph]->SetTitle("Trigger signal");
  graph[iGraph]->GetXaxis()->SetTitle("Time [ns]");
  graph[iGraph]->GetXaxis()->SetTitleOffset(1.3);
  graph[iGraph]->GetYaxis()->SetTitle("Amplitude [mV]");
  graph[iGraph]->GetYaxis()->SetRangeUser(-120,20);
  graph[iGraph]->Draw("al");
  for(int i=iGraph+1;i<=fGrapn;i++) if(i!=2) graph[i]->Draw("same l");


  TLegend *leg = new TLegend(0.70,0.15,0.95,0.3);
  leg->SetBorderSize(1);
  leg->SetFillColor(kWhite);
  //leg->SetFillStyle(3000);
  stringstream ss_graph = "";
  for(int i=iGraph;i<=fGrapn;i++){
	  if(i==0) ss_graph << "Synch signal (normalized)";
	  if(i==3) ss_graph << "Raw trigger";
	  if(i==1) ss_graph << "Width adjusted (680ns)";
	  if(i!=2)leg->AddEntry(graph[i],ss_graph.str().c_str(),"l");
	  ss_graph.str("");
  }
  leg->Draw("");


  if(PrintOpt) c1->Print(PrintFileName);

};
