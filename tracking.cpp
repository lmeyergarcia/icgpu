#include "tracking.h"

using namespace std;

/*compare if two hits are common*/
bool Tracking::compareHits(PrPixelHit one, PrPixelHit two){
	if (one.n_id() == two.n_id()) return true;
	//if ((one.x() == two.x()) && (one.y() == two.y()) && (one.z() == two.z())) return true;
	return false;
}

/*calculate angle between two segments */
float Tracking::calculateAngle(float tx_cur, float ty_cur, float tx_next, float ty_next){
	float difference_tx = (tx_next - tx_cur);
	float difference_ty = (ty_next - ty_cur);
	return sqrt(difference_tx*difference_tx + difference_ty*difference_ty);
}

/*test if the angle between two segments are ok*/
bool Tracking::compareBreakingAngle(float angle){
	if(angle <= BREAKING_ANGLE) return true;
	return false;
}

/*compare if two segments can be neighbors seeing the status*/
bool Tracking::compareStatus(int status_one, int status_two){
	if(status_two == (status_one-1)) return true;
	return false;
}

/*escolhe o menor ângulo e retorna o seu índice*/
int Tracking::chooseBestAngle(vector<TrackS> trackAux){
	float angle = trackAux[0].getLastAngle();
	int indice = 0;
	for(int iangle = 1; iangle < (int) trackAux.size(); iangle++){
		if(trackAux[iangle].getLastAngle() < angle){
			angle = trackAux[iangle].getLastAngle();
			indice = iangle;
		}
	}
	return indice;
}

/*test if two segments are neighbors cells and your hits is not used*/
bool Tracking::testSegment(TrackSegment one, TrackSegment two, vector<vector<PrPixelHit> > hits){
	PrPixelHit hit_one =  one.getFirstHit();
	PrPixelHit hit_two = two.getSecondHit();
	bool  testHits = compareHits(hit_one, hit_two);
	//bool testHit_one = compareHitUsed(hit_one);
	//bool testHit_two = compareHitUsed(hit_two);
	bool seg_status = compareStatus(one.getStatus(), two.getStatus());
	//calculate angle
	float angle = calculateAngle(one.getTx(), one.getTy(), two.getTx(), two.getTy());
	bool breakingAngle = compareBreakingAngle(angle);
	return (breakingAngle && testHits && seg_status);//  && testHit_one && testHit_two);
}

//make the combination track with segments
vector<TrackS> Tracking::combinationTrack(vector<vector<TrackSegment> > tSegment, TrackS &track, int sensor_id, vector<vector<PrPixelHit> > hits){
	vector<TrackS> tracks;
	vector<TrackSegment> segments = tSegment[sensor_id];
	//take the last segment
	TrackSegment aux = track.getLastSeg();
	float angle;
	TrackS aux_track;
	int count = 0;
	for(int iseg = 0; iseg < (int) segments.size(); iseg++){
		if(testSegment(aux, segments[iseg], hits)){
			//calcula o ângulo entre os dois últimos segmentos e guarda na track
			angle = calculateAngle(aux.getTx(), aux.getTy(), segments[iseg].getTx(), segments[iseg].getTy());
			aux_track = track;
			aux_track.setLastSeg(segments[iseg], aux_track);
			aux_track.setLastAngle(angle);
			tracks.push_back(aux_track);
			count++;
		}
	}
	if(count == 0) tracks.push_back(track);
	return tracks;
}

/*faz track*/
void Tracking::makeTrack(vector<vector<TrackSegment> > tSegment, TrackS &track, int sensor_id, vector<vector<PrPixelHit> > hits){
	TrackSegment aux;
	int indice = 0;
	int id = sensor_id;

	for(; id >= 0; id = id-2){
		vector<TrackS> trackAux = combinationTrack(tSegment, track, id, hits);
		indice = chooseBestAngle(trackAux);
		track = trackAux[indice];
		//track.setLastSeg(trackAux[indice], track);
	}
}

/*make segments*/
vector<TrackSegment> Tracking::makeSimpleSegment(vector<PrPixelHit> nextHits, vector<PrPixelHit> currentHits){
	vector<TrackSegment> tSegment;
	for(int id_current = 0; id_current < (int) currentHits.size(); id_current++){
		float x_zero = currentHits[id_current].x();
		float y_zero = currentHits[id_current].y();
		float z_zero = currentHits[id_current].z();
		for(int id_next = 0; id_next < (int) nextHits.size(); id_next++){
			float x_one = nextHits[id_next].x();
			float y_one = nextHits[id_next].y();
			float z_one = nextHits[id_next].z();
			float tx = (x_one - x_zero)/(z_one - z_zero); // x = bx + tx*z
			float ty = (y_one - y_zero)/(z_one - z_zero); // y = by + ty*z
			float bx = x_zero - tx*z_zero;
			float by = y_zero - ty*z_zero;
			float x_1000 = bx + tx*1000;
			float y_1000 = by + ty*1000;
			//see the angle between the two hits
			if(true  /*sqrt(tx*tx+ty*ty) <= ACCEPTANCE_ANGLE*/){
                                     	vector<PrPixelHit> tmp;
				//make segment object
				tmp.push_back(currentHits[id_current]);
				tmp.push_back(nextHits[id_next]);
				TrackSegment aux (tmp, INITIAL_STATUS, tx, ty, x_1000, y_1000);
				//print the segment on file
				//printHit(currentHits[id_current]);
				//printHit(nextHits[id_next]);
				//segFile << endl;
				//count the segment
				//no_segments++;
				tSegment.push_back(aux);
				tmp.clear();

			}
		}
	}
	//segFile << no_segments << endl;
  return tSegment;
}

/*forward process*/
void Tracking::forwardProcess(vector<TrackSegment>& currentSeg, vector<TrackSegment>& nextSeg, vector<vector<PrPixelHit> > hits){
	for(int id_current = 0; id_current < (int) currentSeg.size(); id_current++){
		//take the tx and ty of the segments
		float tx_cur = currentSeg[id_current].getTx();
		float ty_cur = currentSeg[id_current].getTy();
		//take the first hit of the segment on currentSeg
		PrPixelHit hit_cur = currentSeg[id_current].getSecondHit();
		//PrPixelHit hit_cur = takeHit(hit_cur_id, hits);
		for(int id_next = 0; id_next < (int) nextSeg.size(); id_next++){
			//take the tx and ty of the segments
			TrackSegment tmp_seg = nextSeg[id_next];
			float tx_next = tmp_seg.getTx();
			float ty_next = tmp_seg.getTy();
			//take the first hit of the segment on nextSeg
			//vector<PrPixelHit> tmpNext = nextSeg[id_next].getTrackSegment();
			PrPixelHit hit_next = nextSeg[id_next].getFirstHit();
			//PrPixelHit hit_next = takeHit(hit_next_id, hits);
      if (!compareHits(hit_cur, hit_next)) continue;
			//calculate the breaking angle and verify if the segments has one common hit
			float angle = calculateAngle(tx_cur, ty_cur, tx_next, ty_next);
			//verify if the angle and the hit it's ok
			if(compareBreakingAngle(angle)){
				//increase the status
				//if two segments has the same continuation, the preference is for the segment with greater status
				if(currentSeg[id_current].getStatus()+1 > nextSeg[id_next].getStatus()){
					nextSeg[id_next].setStatus(currentSeg[id_current].getStatus()+1);
					/*printSegStatus(currentSeg[id_current].getFirstHit());
					printSegStatus(currentSeg[id_current].getSecondHit());
					printSegStatus(nextSeg[id_next].getFirstHit());
					printSegStatus(nextSeg[id_next].getSecondHit());
					segStatus << "status 1: " << currentSeg[id_current].getStatus() << endl;
					segStatus << "status 2: " << nextSeg[id_next].getStatus() << endl;*/
				}
			/*	printTrackSegment(currentSeg[id_current]);
				printTrackSegment(nextSeg[id_next]);
				segStatus << angle << endl;
				segStatus << endl; */
			}
		}
	}
}

/*backward process*/
void Tracking::backwardProcess(vector<vector<TrackSegment> > &tSegment, vector<TrackS> &tracks, vector<vector<PrPixelHit> > hits){
	int i = 1;
	for(int isen =  tSegment.size()-1; isen >= 0; isen--){
		vector<TrackSegment> currentSensor = tSegment[isen];
		for(int iseg = 0; iseg < (int) currentSensor.size(); iseg++){
			TrackS track(currentSensor[iseg]);
			//make track
			makeTrack(tSegment, track, isen-2, hits);


			//setUsedTrack(tSegment, auxTrack[id], id_sensor);
			vector<PrPixelHit> aux = track.getHits();
			if(aux.size() <= 2) continue;
			//printTrack(track, i); i++;
			tracks.push_back(track);
			//return;
		}
	}
}

void Tracking::makeTracking(DataFile data){
  int no_sensors = data.getNoSensor();
  vector<vector<PrPixelHit> > hits = data.getHits();
  /*make segments*/
	for(unsigned isen = 0; isen < no_sensors-2; isen++){
		vector<TrackSegment> tmpSeg = makeSimpleSegment(hits[isen+2], hits[isen]);
		tSegment.push_back(tmpSeg);
	}
    return; //Murilo TODO
  //count the total of segments
	int contSeg = 0;
	for(int i = 0; i <  (int) tSegment.size(); i++){
		contSeg = contSeg+tSegment[i].size();
	}

  cout << contSeg << endl;

  int i = 0;
	/*increase the status*/
	for(int isen = 0; isen < (int) tSegment.size()-2; isen++){
		//std::cout << tSegment[isen].size() << std::endl;
		forwardProcess(tSegment[isen], tSegment[isen+2], hits);
	}

  /*backward process*/
	backwardProcess(tSegment, tracks, hits);

}

vector<TrackS> Tracking::getTracks() {return tracks;}

vector<vector<TrackSegment> > Tracking::getSimpleSegments() {return tSegment;}
