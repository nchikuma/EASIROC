//const char* readFileName_waveform = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/WaveForm/20150924_MultiHit/ALL0007/doublepulse.txt";
const char* readFileName_waveform = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/WaveForm/20150924_MultiHit/ALL0007/doublepulse_zoom.txt";

const char* PrintFileName_Waveform = "C:/Users/nchikuma/Documents/NaruhiroChikuma/EASIROC/data/figure/DoublePulse_Input.png";

void Draw_Waveform(){

	TGraph* g = new TGraph(readFileName_waveform);
	g->SetLineWidth(2);
	g->SetTitle("Double-pulse input");
	g->GetXaxis()->SetTitle("Time [ns]");
	g->GetXaxis()->SetRangeUser(0,200);
	g->GetYaxis()->SetTitle("Voltage [mV]");
	g->Draw("ac");

	c1->Print(PrintFileName_Waveform);

};
	

