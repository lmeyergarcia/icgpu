#include<iostream>
#include<fstream>
#include "TFile.h"
#include<TGraph>

using namespace std;

int z_1000(){

	TFile *outfile = new TFile("graphs.root","recreate");

	fstream all_tracks("tracks.txt", ios_base::in);
	fstream real_tracks("reais.txt", ios_base::in);

	vector<double> all_x;
	vector<double> all_y;

	vector<double> real_x;
	vector<double> real_y;

	double value;
	bool cntrl_var = true;

	while (all_tracks >> value){
		if (cntrl_var){
			all_x.push_back(value);
			cntrl_var = false;
		}
		else{		
			all_y.push_back(value);		
			cntrl_var = true;
		}
	}

	cntrl_var = true;

	while (real_tracks >> value){
		if (cntrl_var){
			real_x.push_back(value);
			cntrl_var = false;
		}
		else{		
			real_y.push_back(value);		
			cntrl_var = true;
		}
	}

	TGraph *gr_all_tracks = new TGraph(all_x.size(),&(all_x[0]),&(all_y[0]));
	gr_all_tracks->SetMarkerStyle(2);
	gr_all_tracks->SetMarkerSize(0.1);
	gr_all_tracks->Draw("AP");
	gr_all_tracks->Write("All Tracks (z = 1000) ");

	TGraph *gr_real_tracks = new TGraph(real_x.size(),&(real_x[0]),&(real_y[0]));
	gr_real_tracks->SetMarkerStyle(2);
	gr_real_tracks->SetMarkerSize(1);
	gr_real_tracks->Draw("AP");
	gr_real_tracks->Write("All Tracks (z = 1000) ");

	outfile->Close();

	return 0;
}
