#include <stdio.h>
#include <iostream>
#include <fstream>
//#include "dados.h"
#include "tracking.h"
#include <chrono>

using namespace std;

int main(){	
	DataFile data;
	cout << "Rodando codigo: prepareData()" << endl;
	data.prepareData();
	cout << data.getNoHit() << endl;

	cout << "Testando os resultados" << endl;
 	data.prepareResults();
 	// exit(0);
	
	auto start = chrono::high_resolution_clock::now();

	Tracking teste;
	cout << "Rodando codigo: makeTracking()" << endl;
	teste.makeTracking(data);
	
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> dif = end - start;
	cout << dif.count() << " segundos" << endl;

	vector<vector<TrackSegment> > trackSegments;
	ofstream trackFile("tracks.txt");
	cout << "Gravando hits" << endl;
	trackSegments = teste.getSimpleSegments();
	for(int i = 0; i < trackSegments.size(); i++){
		for(int j = 0; j < trackSegments[i].size(); j++){
			float x_1000 = trackSegments[i][j].getX_1000();
			float y_1000 = trackSegments[i][j].getY_1000();
			float tx = trackSegments[i][j].getTx();
			float ty = trackSegments[i][j].getTy();
			float angle = sqrt(tx*tx + ty*ty);
			trackFile << x_1000 << " " << y_1000 << " " << angle << endl; // x_1000 | y_1000 | angle
		}
	}
	trackFile.close();
	
	vector<vector<unsigned int> > id_results = data.getResult();
	ofstream real("reais.txt");
	cout << "Gravando informações da simulação" << endl;
	for(int i = 0; i < trackSegments.size(); i++){
		for(int j = 0; j < trackSegments[i].size(); j++){
			for (int k = 0; k < id_results.size(); k++){
				vector<PrPixelHit> simpleSegment = trackSegments[i][j].getTrackSegment();
				unsigned int id1 = simpleSegment[0].id(); 
				unsigned int id2 = simpleSegment[1].id(); //id dos hits do simpleSegment
				vector<unsigned int>::iterator status_id_1 = find(id_results[k].begin(), id_results[k].end(), id1);
				vector<unsigned int>::iterator status_id_2 = find(id_results[k].begin(), id_results[k].end(), id2);
				float tx = trackSegments[i][j].getTx();
				float ty = trackSegments[i][j].getTy();
				float angle = sqrt(tx*tx + ty*ty);
				if (status_id_1 != id_results[k].end() && status_id_2 != id_results[k].end()){
					float x_1000 = trackSegments[i][j].getX_1000();
					float y_1000 = trackSegments[i][j].getY_1000();
					vector<unsigned int> isLong = data.getIsLong();
					real << x_1000 << " " << y_1000 << " " << angle << " " << k << " " << isLong[k] << endl; // x_1000 | y_1000 | angle | indice | isLong
				}
			}
		}
	}
	real.close();
	
/*	vector<TrackS> tracks = teste.getTracks();
	cout << "Rodando codigo: compareTracks(tracks)" << endl;
	data.compareTracks(tracks);

	ofstream trackFile("tracks.txt");
	for(int i = 0; i < tracks.size(); i++){
		vector<PrPixelHit> hits = tracks[i].getHits();
		trackFile << "(";
		for(int j = 0; j < hits.size(); j++){
			trackFile  << hits[j].id() << ", ";
		}
		trackFile << ")" << endl;
	}
	return 0;
*/
		
}
