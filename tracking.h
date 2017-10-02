#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "PrPixelHit.h"
// #include "structs.h"
#include "dados.h"
using namespace std;

const int INITIAL_STATUS = 1;
const int DIFFERENCE_SENSOR = 3;
const float BREAKING_ANGLE = 0.005;
//const float BREAKING_ANGLE = 1.5708;
const float ACCEPTANCE_ANGLE = 900;

class Tracking{
  public:
    void makeTracking(DataFile data);
    vector<TrackSegment> makeSimpleSegment(vector<PrPixelHit> nextHits, vector<PrPixelHit> currentHits);
    void forwardProcess(vector<TrackSegment>& currentSeg, vector<TrackSegment>& nextSeg, vector<vector<PrPixelHit> > hits);
    void backwardProcess(vector<vector<TrackSegment> > &tSegment, vector<TrackS> &tracks, vector<vector<PrPixelHit> > hits);
    bool compareHits(PrPixelHit one, PrPixelHit two);
    float calculateAngle(float tx_cur, float ty_cur, float tx_next, float ty_next);
    bool compareBreakingAngle(float angle);
    void makeTrack(vector<vector<TrackSegment> > tSegment, TrackS &track, int sensor_id, vector<vector<PrPixelHit> > hits);
    vector<TrackS> combinationTrack(vector<vector<TrackSegment> > tSegment, TrackS &track, int sensor_id, vector<vector<PrPixelHit> > hits);
    bool testSegment(TrackSegment one, TrackSegment two, vector<vector<PrPixelHit> > hits);
    bool compareStatus(int status_one, int status_two);
    int chooseBestAngle(vector<TrackS> trackAux);
    vector<TrackS> getTracks();
    vector<vector<TrackSegment> > getSimpleSegments();
  private:
    vector<vector<TrackSegment> > tSegment;
    vector<TrackS> tracks;
};
