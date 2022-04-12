// Code written by Serena Fattori
// for any issue write to: dr.serena.fattori@gmail.com


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath> 

using namespace std;
int test(int, double, double, double*);
int test_s(int, double, double*);

void rebin() {
	char path_in[100], file_in[20], file_in_path[120], path_out[100], file_out[100];
	string line;
	
	int index = 0;
	const int Bdec = 75; // numero di bin per decade 
	const int Ndec = 3; // numero di decadi
	const int N_BINS_LOG = Bdec * Ndec; // Numero totale di bin logaritmici
	double integral_lin = 0, integral_log = 0, integral_log_rebin = 0; // SF
	double y0 = 1; // bin iniziale del binnaggio logaritmico
	//const int NUMEL = 8189; // file Giada
	const int NUMEL = 8192; // file Pablo
	//const int NUMEL = 381; // file MITRA
	const int NUM_ESTREMI = NUMEL+1; // numero di estremi del binnaggio lineare
	double x[NUMEL], c_lin[NUMEL], dx_lin[NUMEL]; // vettori dove scarico il contenuto del file iniziale (x = y e c_lin = ydy)
	double ticks_lin[NUM_ESTREMI]; // vettore che conterrà gli estremi dei bin lineari  
	double x_rebin[N_BINS_LOG], dx_log[N_BINS_LOG]; //vettore che conterrà i bin logaritmici (valore centrale di ciascun bin)
	double ticks_log[N_BINS_LOG+1], c_log[N_BINS_LOG], c_log_normwidth[N_BINS_LOG]; // vettori che conterranno gli estremi dei bin logaritmici,
	// i conteggi dei bin logaritmici, e i conteggi normalizzati
	

	// APERTUTRA FILE IN INPUT
	sprintf(path_in, "/home/cirrone/Dropbox/script_e_software/MPGroupSoftware/Microdosimetry"); 
	sprintf(file_in, "y_ydy_FE_27_69mm.txt");
	sprintf(file_in_path, "%s/%s",path_in,file_in);
	ifstream file(file_in_path);	
	

	
	// APERTURA FILE IN OUTPUT
	sprintf(path_out, "/home/cirrone/Dropbox/script_e_software/MPGroupSoftware/Microdosimetry"); 
	sprintf(file_out,"%s/Log_bin_%d_%s", path_out, Bdec, file_in); 
	ofstream out;	
	out.open(file_out);
	
	cout << file_out << endl;
	
	
	while (getline(file, line)) {
		istringstream iss(line);
		iss >> x[index] >> c_lin[index];
		dx_lin[index] = x[index] - x[index-1]; // SF
		//cout << x[index] << "  " << c[index] << endl;
		integral_lin += (c_lin[index]/dx_lin[index]) *dx_lin[index]; //SF
		++index;
	}
	file.close();
	cout << "Integral lin:  " << integral_lin << endl;

	double lin_step = x[1] - x[0];
	if (x[0] < y0)
		cout << "problem: the smallest linear bin is smaller than the smallest log bin!! " << endl;
	ticks_lin[0] = x[0] - lin_step*0.5;
	for (int a = 0; a < NUMEL; ++a) {
		ticks_lin[a + 1] = x[a] + lin_step*0.5;
	}
	
	/*
	// SPETTRO (con TH1D) BINNATO LIINEARE **********************************************
	TCanvas* c1 = new TCanvas("c1","Microdosimetric Spectra",200,10,1000,700);  // Serena
	c1->Divide(2,2);
	c1->cd(1);
	TH1D* hlin = new TH1D("hlin","hlin",NUMEL,ticks_lin[0],ticks_lin[NUMEL-1]);
	for(int a=0; a<NUMEL; ++a)
	    hlin->Fill(x[a],c_lin[a]); ///dx_lin[a]);
	hlin->SetTitle("Spettro binnato lineare");	
	hlin->SetLineColor(0);
	hlin->SetMarkerStyle(20);
	hlin->SetMarkerSize(0.4);
	hlin->SetMarkerColor(4);
  	hlin->Draw("E2");
  	hlin->GetXaxis()->SetTitle("y [keV/um]");
	hlin->GetYaxis()->SetTitle("y d(y) / (0.013341067286 keV/um)");
	 hlin->GetXaxis()->SetTitleOffset(1.2);
	 hlin->GetXaxis()->SetLabelSize(0.03);
	 hlin->GetYaxis()->SetTitleOffset(1.2);
	 hlin->GetYaxis()->SetLabelSize(0.03);
	 hlin->GetYaxis()->SetNdivisions(120);
	hlin->GetXaxis()->SetRangeUser(0,200);
	hlin->SetMaximum(1.0);
	//  *********************************************************************************
	*/	
		
	/*	
	//  SPETTRO (con TGraph) BINNATO LIINEARE IN SCALA X LOGARITMICA  *******************
	c1->cd(2)->SetLogx();
	TGraph * gr_lin = new TGraph(NUMEL,x,c_lin);
	 gr_lin->SetName("gr_lin");
	 gr_lin->SetMarkerStyle(2);
	 gr_lin->SetMarkerSize(2);
	 gr_lin->SetLineColor(4);
	 gr_lin->SetLineWidth(2);
	 gr_lin->Draw("AL.");
	 gr_lin->SetTitle("Spettro binnato lineare, plottato con scala X logaritmica");	
	 gr_lin->GetXaxis()->SetTitle("y [keV/um]");
	 gr_lin->GetYaxis()->SetTitle("y d(y) / (0,168619489559165 keV/um)");
	 gr_lin->GetXaxis()->SetTitleOffset(1.2);
     gr_lin->GetXaxis()->SetLabelSize(0.03);
	 gr_lin->GetYaxis()->SetTitleOffset(1.2);
     gr_lin->GetYaxis()->SetLabelSize(0.03);
	 gr_lin->GetYaxis()->SetNdivisions(120);
	 gr_lin->GetXaxis()->SetLimits(1.0,1000.);
//	  gr_lin->GetYaxis()->SetRangeUser(1e0,1e8);
//	  gr_lin->GetYaxis()->SetRangeUser(0.,7.);
	//  *********************************************************************************
	*/
	
	
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// REBBINNAGGIO
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ----------------------------------------------------------------------------------
	// trovo i valori (inferiori) dei bin logaritmici
	for (int a = 0; a < N_BINS_LOG + 1; ++a)
	    ticks_log[a] = y0*pow(10.,1.*a/Bdec);
       
	// trovo il valore centrale di ciascun bin logaritmico
	// inizializzo il numero di conteggi di ciascun bin logaritmico a 0
	// trovo la larghezza di ciacun bin logaritmico
	for (int a = 0; a < N_BINS_LOG; ++a) {
	    x_rebin[a] = ticks_log[a] * sqrt(ticks_log[a + 1] / ticks_log[a]);
	    c_log[a] = 0.;
	    dx_log[a] = ticks_log[a + 1] - ticks_log[a];
	}
	// ----------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------
	// per ciascun bin lineare vado a vedere quanti bin logaritmici copre (1,2,>2)
	// per ciascuno di questi casi mi devo comportare in modo diverso (switch case)
	int rtest = -1;
	double s1, s2;
	for (int a = 0; a<NUMEL; ++a) {
		s1 = ticks_lin[a];
		s2 = ticks_lin[a + 1];
		rtest = test(N_BINS_LOG, s1,s2,ticks_log);
		/*if(rtest > 0){
			cout << rtest <<"  s1="<<s1<<"  s2="<<s2<< endl;
			cout << " --------------------- " << endl;
		}*/
		int id = 0;
		int t1 = -1;
		int t2 = -1;
		rtest = (rtest > 1) ? 2 : rtest;
		switch (rtest) {
		case 0: // a linear bin is smaller than a single log bin
			id = test_s(N_BINS_LOG, s1, ticks_log);
			c_log[id] = c_log[id] + c_lin[a];
			break;
		case 1: // a linear bin is in between two different log bins
			id = test_s(N_BINS_LOG, s2, ticks_log);
			c_log[id]= c_log[id] + c_lin[a]/ (s2 - s1)*(s2 - ticks_log[id]);
			c_log[id-1] = c_log[id-1] + c_lin[a] / (s2 - s1)*(ticks_log[id] - s1);
			//cout <<"------- "<< id<<"\t"<< s1<<"\t"<< ticks_log[id] <<"\t"<<s2<< endl;
			break;
		case 2: // a liner bin is larger more than 2 log bins!
			t1 = test_s(N_BINS_LOG, s1, ticks_log);
			t2 = test_s(N_BINS_LOG, s2, ticks_log);
			c_log[t1] = c_log[t1] + c_lin[a] / (s2 - s1) * (ticks_log[t1+1] - s1);
			c_log[t2] = c_log[t2] + c_lin[a] / (s2 - s1) * (s2 - ticks_log[t2]);
			for (int b = t1 + 1; b < t2; ++b) {
				//cout <<"********* "<< s1 << " " << s2 << " " << t1 << " " << t2 << " " << b << " " << dx_log[b] << endl;
				c_log[b] = c_log[b] + c_lin[a] / (s2 - s1)*dx_log[b];
			}
			//cout << "Problema, aggiungere il caso in cui rtest sia maggiore di 1." << endl;
			break;
		default:
			cout << "Problema, Caso non previsto." << endl;
			break;
		}

	}
	// ----------------------------------------------------------------------------------
	
	// ----------------------------------------------------------------------------------
	// Rinormalizzo dividendo il contenuto di ciascun bin per l'ampiezza del bin
	for (int a = 0; a < N_BINS_LOG; ++a) {
		//c_log_normwidth[a] = (c_log[a] / dx_log[a])*0.168619490; /// riscalato;
		c_log_normwidth[a] = (c_log[a] / dx_log[a]); /// 74.869;
		cout <<a<<"\t"<< x_rebin[a] << "\t" << c_log[a] << "  "  << dx_log[a] << "  " << c_log_normwidth[a] <<endl;
		//cout <<a<<"\t"<< x_rebin[a]  << "  "  << dx_log[a] << "  " << c_log_normwidth[a] <<endl;
		integral_log +=  c_log[a]  * dx_log[a]; // SF
		integral_log_rebin += c_log_normwidth[a] * dx_log[a]; // SF
			
		out << x_rebin[a] << "\t" << c_log_normwidth[a] << endl;

		}
	
	cout << "Integral log:  " << integral_log << endl;  // SF
	cout << "Integral log rebin:  " << integral_log_rebin << endl;  // SF
	out.close();  // SF
	
	// ----------------------------------------------------------------------------------
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	
	
	/*
	//  SPETTRO BINNATO LOGARITMICO  ****************************************************
	c1->cd(3)->SetLogx();
	TGraph * gr_log = new TGraph(N_BINS_LOG,x_rebin,c_log);
	 gr_log->SetName(" gr_log");
	 gr_log->SetMarkerStyle(2);
	 gr_log->SetMarkerSize(2);
	 gr_log->SetLineColor(4);
	 gr_log->SetLineWidth(2);
	 gr_log->Draw("AL.");
	 gr_log->SetTitle("Spettro binnato logaritmico");	
	 gr_log->GetXaxis()->SetTitle("y [keV/um]");
	 gr_log->GetYaxis()->SetTitle("y d(y)");
	 gr_log->GetXaxis()->SetTitleOffset(1.2);
     gr_log->GetXaxis()->SetLabelSize(0.03);
	 gr_log->GetYaxis()->SetTitleOffset(1.2);
     gr_log->GetYaxis()->SetLabelSize(0.03);
	 gr_log->GetYaxis()->SetNdivisions(120);
	 gr_log->GetXaxis()->SetLimits(1.0,1000.);
//	  gr_log->GetYaxis()->SetRangeUser(1e0,1e8);
//	  gr_log->GetYaxis()->SetRangeUser(0.,7.);
	//  *********************************************************************************
	*/
	
	
	

	 /*
	//  SPETTRO BINNATO LOGARITMICO RINORMALIZZATO  *************************************
	c1->cd(4)->SetLogx();
	TGraph *gr_lognorm = new TGraph(N_BINS_LOG,x_rebin,c_log_normwidth);
	gr_lognorm->SetName("gr_lognorm");
	gr_lognorm->SetMarkerStyle(2);
	gr_lognorm->SetMarkerSize(2);
	gr_lognorm->SetLineColor(4);
	gr_lognorm->SetLineWidth(2);
	gr_lognorm->Draw("AL.");
	gr_lognorm->SetTitle("Spettro binnato logaritmico rinormalizzato");	
	gr_lognorm->GetXaxis()->SetTitle("y [keV/um]");
	gr_lognorm->GetYaxis()->SetTitle("y d(y) / (1 keV/um)");
	gr_lognorm->GetXaxis()->SetTitleOffset(1.2);
    gr_lognorm->GetXaxis()->SetLabelSize(0.03);
	gr_lognorm->GetYaxis()->SetTitleOffset(1.2);
    gr_lognorm->GetYaxis()->SetLabelSize(0.03);
	gr_lognorm->GetYaxis()->SetNdivisions(120);
	gr_lognorm->GetXaxis()->SetLimits(y0,1000.);
//	 gr_lognorm->GetYaxis()->SetRangeUser(1e0,1e8);
//	 gr_lognorm->GetYaxis()->SetRangeUser(0.,7.);
	//  *********************************************************************************
	*/


}



// FUNZIONI CHIAMATE NEL MAIN:

// TEST ----------------------------------------------------------------------------------
int test(int iter, double s1, double s2, double* tlog) { 
	int result = 0;
	for (int a = 0; a < iter + 1; ++a) {
		if (tlog[a] >= s1 && tlog[a] <= s2)
			++result;
	}
	return result;
}
// ---------------------------------------------------------------------------------------


// TEST_S --------------------------------------------------------------------------------
int test_s(int iter, double s, double* tlog) {
	int result = 0;
	for (int a = 0; a < iter + 1; ++a) {
		if (tlog[a] <= s)
			++result;
	}
	return result-1;
}
// ---------------------------------------------------------------------------------------
int main() { rebin(); return 0;}
