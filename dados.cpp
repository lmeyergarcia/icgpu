/*class to process data
 *
 *  @author Leticia Freire
 */

#include <fstream>
#include <iostream>
#include "dados.h"
// #include "structs.h"
// #include "PrPixelHit.h"
#include <json/json.h>

//para compilar: g++ -I /usr/include/jsoncpp/ dados.cpp -ljsoncpp

using namespace std;

void DataFile::prepareData() {
	/*opening file*/
	ofstream dataFile("dados.txt");
	ifstream ifs("0.json");
	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj); // reader can also read strings

	/*number of sensors from 0.json */
	no_sensor = obj["event"]["number_of_sensors"].asInt();
	/*number of hits from 0.json */
	no_hits = obj["event"]["number_of_hits"].asInt();
	/*modules z*/
	const Json::Value& mod_z = obj["event"]["sensor_module_z"];
	for(int z = 0; z < mod_z.size(); z++) module_z.push_back(mod_z[z].asFloat());
	/*number of hits per sensor*/
	for(int n_hits = 0; n_hits < no_sensor; n_hits++) no_hits_sensor.push_back(obj["event"]["sensor_number_of_hits"][n_hits].asInt());

	/*building hits*/
	/*hit id*/
	const Json::Value& id = obj["event"]["hit_id"];
	/*hit x*/
	const Json::Value& x = obj["event"]["hit_x"];
	/*hit y*/
	const Json::Value& y = obj["event"]["hit_y"];
	/*hit z*/
	const Json::Value& z = obj["event"]["hit_z"];

	int j = 0;
	int number_hits = 0;
	for(int n_hits = 0; n_hits < no_sensor; n_hits++){
		number_hits += no_hits_sensor[n_hits];
		vector<PrPixelHit> aux;
		for(; j < number_hits; j++){
			PrPixelHit hit;
			hit.setHit(id[j].asUInt(),
		             x[j].asFloat(), y[j].asFloat(), z[j].asFloat(),
		             0.0, 0.0,
		             module_z[n_hits]);
			aux.push_back(hit);
		}
		hits.push_back(aux);
	}

	/*closing file*/
	dataFile.close();
}

void DataFile::prepareResults(){
	/*opening file*/
	ofstream dataFile("dados.txt");
	ifstream ifs("0.json");
	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj); // reader can also read strings

	
	const Json::Value& particles = obj["montecarlo"]["particles"];

	// cout << particles[0][15] << endl;

	for(int particle = 0; particle < particles.size(); particle++){
		vector<unsigned int> aux;
		const Json::Value& ids = particles[particle][15];
		for(int id = 0; id < ids.size(); id++){
			aux.push_back(ids[id].asUInt());
		}
		id_results.push_back(aux);
	}

	for(int i = 0; i < id_results.size(); i++){
		vector<unsigned int> aux = id_results[i];
		for(int j = 0; j < aux.size(); j++)
			dataFile << id_results[i][j] << endl;
	}

	/*closing file*/
	dataFile.close();
}

void DataFile::compareTracks(vector<TrackS> tracks){
	/*opening file*/
	ofstream goodTrack("good.txt");
	ofstream fakeTrack("others.txt");

	cout << "Total de tracks originais: " <<  id_results.size() << endl;
	cout << "Total de tracks formadas: " <<  tracks.size() << endl;

	/*variables*/
	int goodTracks = 0;
	int fakeTracks = 0;

	/*comparing formed tracks with original tracks*/
	for(int track = 0; track < tracks.size(); track++){
		vector<PrPixelHit> hits = tracks[track].getHits();
		/*comecar de tras pra frente*/
		int hit = hits.size()-1;
		unsigned int id = hits[hit].id();
		int trackUsada = 0;
		while(hit >= 0){
			// int good = 0;
			for(int i = 0; i < id_results.size(); i++){
				int good = 0;
				int j = 0;
				while(j < id_results[i].size()){
					while(id == id_results[i][j]){
						good++;
						j++; hit--;
						id = hits[hit].id();
					}
					if(good > 0){
						trackUsada = 1;
						float goodPer = (float) good/id_results[i].size();
						// cout << goodPer << endl;
						if(goodPer >= 0.6){
							goodTracks++;
							// cout << goodPer << endl;
							for(int i = hits.size()-1; i>= 0; i--) 
								goodTrack << hits[i].id() << ", ";
							goodTrack << endl;
						}  
						else{
							fakeTracks++;
							for(int i = hits.size()-1; i>= 0; i--) 
								fakeTrack << hits[i].id() << ", ";
							fakeTrack << endl;
						}
						break;
					}
					j++;
				}
			}
			if(trackUsada) break;
			hit--;
		}
	}


	cout << "Total de tracks good: " <<  goodTracks << endl;
	cout << "Total de tracks fakes e clones: " <<  fakeTracks << endl;

	/*closing file*/
	goodTrack.close();
	fakeTrack.close();

}

int DataFile::getNoSensor() {return no_sensor;}
int DataFile::getNoHit() {return no_hits;}
vector<float> DataFile::getModule() {return module_z;}
vector<int> DataFile::getNoHitsSensor() {return no_hits_sensor;}
vector<vector<PrPixelHit> > DataFile::getHits() {return hits;}
vector<PrPixelHit> DataFile::getHitsSensor(int i) {return hits[i];}
vector<vector<unsigned int> > DataFile::getResult() {return id_results;}
