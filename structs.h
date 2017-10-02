#include <vector>
#include <iostream>
#include "PrPixelHit.h"

using namespace std;

class TrackSegment{
	public:
		void setTrackSegment(PrPixelHit one, PrPixelHit two){
			trackSegment.push_back(one); trackSegment.push_back(two);
		}

        //colocar: tx, ty, estado
		vector<PrPixelHit> getTrackSegment() {return trackSegment;}

		PrPixelHit getFirstHit(){return trackSegment[0];}

		PrPixelHit getSecondHit(){return trackSegment[1];}
		//trackSegment.size()-1

		void setTrackSegment(PrPixelHit hit) {trackSegment.push_back(hit);}

		int getStatus(){return status;}

		void setStatus(int _status){status = _status;}

	/*	void printTrackSegment(){
			for(int i = 0; i < trackSegment.size(); i++)
				cout << "x: " << trackSegment[i].x() << " y: " << trackSegment[i].y() << " z: " << trackSegment[i].z() << endl;
		} */

		float getTx(){return tx;}

		void setTx(float _tx){tx = _tx;}

		float getTy(){return ty;}

		void setTy(float _ty){ty = _ty;}

		float getX_1000(){return x_1000;}

		void setX_1000(float _x_1000){x_1000 = _x_1000;}

		float getY_1000(){return y_1000;}

		void setY_1000(float _y_1000){y_1000 = _y_1000;}

		TrackSegment(){}

		TrackSegment(vector<PrPixelHit> _trackSegment, int _status, float _tx, float _ty, float _x_1000, float _y_1000){
			trackSegment = _trackSegment;
			status = _status;
			tx = _tx;
			ty = _ty;
			x_1000 = _x_1000;
			y_1000 = _y_1000;
		}


	private:
		vector<PrPixelHit> trackSegment;
		int status;
		float tx;
		float ty;
		float x_1000;
		float y_1000;
};


class TrackS{
	public:
		TrackSegment getLastSeg() {return m_lastSeg;}
		void setLastSeg(TrackSegment trackSeg) {m_lastSeg = trackSeg;}
		void addHits(PrPixelHit hit) {m_hits.push_back(hit);}
		vector<PrPixelHit> getHits() {return m_hits;}
		float getSum() {return sum;}
		void setSum(float _sum) {sum = _sum;}

		float getLastAngle() {return lastAngle;}
		void setLastAngle(float angle) {lastAngle = angle;}

		void setLastSeg(TrackSegment trackSeg, TrackS &track){
			TrackSegment segment = m_lastSeg;
			vector<PrPixelHit> aux = segment.getTrackSegment();
			m_hits.push_back(aux[0]); //m_hits.push_back(aux[1]);
			m_lastSeg = trackSeg;
		}

		//constructor
		TrackS(){}

		TrackS(vector<PrPixelHit> _track, float _sum){
			m_hits = _track;
			sum = _sum;
		}

		TrackS(vector<PrPixelHit> _track, TrackSegment lastSeg, float _sum){
			m_hits = _track;
			m_lastSeg = lastSeg;
			sum = _sum;
		}

		TrackS(TrackSegment lastSeg){
			m_lastSeg = lastSeg;
		}

	private:
		TrackSegment m_lastSeg;
		vector<PrPixelHit> m_hits;
		float sum;
		float lastAngle;

};
