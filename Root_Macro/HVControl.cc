//#include <ifstream>
#include <sstream>

const int nData = 25;

const char* readFileName = "HVControl.txt";

const bool PrintOpt = true;
//const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/HVControl.png";
const char* PrintFileName = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/HVControl.eps";

double xx[nData],yy[nData];

void HVControl(){

  TCanvas* c1 = new TCanvas("c1");
  c1->SetGrid(1);

  ifstream ifs(readFileName);

  for(int i=0;i<nData;i++){
	  ifs >> xx[i] >> yy[i];
  }
    
  TGraph *graph = new TGraph(nData,xx,yy);

  //graph->SetLineStyle(i);
  //int color_int = 0;
  //graph->SetLineColor(color_int);
  //double width = 3.;
  //graph->SetLineWidth(width);
  graph->SetMarkerStyle(21);
  graph->SetMarkerSize(1);
  //graph->SetTitle("DAC control of MPPC's bias voltage");
  graph->SetTitle("");
  graph->GetXaxis()->SetTitle("16-bit DAC input");
  graph->GetXaxis()->SetTitleOffset(1.3);
  graph->GetYaxis()->SetTitle("Bias voltage [V]");
  graph->Draw("ap");

  TF1* f1 = new TF1("f1","pol1(0)",0.,50000.);
  f1->SetLineColor(1);
  f1->SetParameter(0,-1.94);
  f1->SetParameter(1,0.002378);
  //graph->Fit("f1","+","",4000.,39000.);
  f1->SetRange(0.,50000.);
  f1->SetLineStyle(2);
  f1->SetLineColor(2);
  f1->Draw("same");
  TF1* f2 = new TF1("f2","pol1(0)",0.,80000.);
  f2->SetLineColor(1);
  f2->SetParameter(0,92.);
  f2->SetParameter(1,0.);
  f2->SetRange(0.,80000.);
  f2->SetLineStyle(2);
  f2->SetLineColor(2);
  f2->Draw("same");


  TLegend *leg = new TLegend(0.5,0.15,0.88,0.27);
  leg->SetBorderSize(1);
  leg->SetFillColor(kWhite);
  //leg->SetFillStyle(3000);
  leg->AddEntry(graph,"data","p");
  leg->AddEntry(f1,"Y = 0.0024*X - 1.94 / Y = 92.","l");
  leg->Draw("");

  if(PrintOpt) c1->Print(PrintFileName);

};
