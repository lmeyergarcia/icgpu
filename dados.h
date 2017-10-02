#include <fstream>
#include <iostream>
#include <json/json.h>
#include "PrPixelHit.h"
#include "structs.h"
using namespace std;

class DataFile{
  public:
    void prepareData();
    int getNoSensor();
    int getNoHit();
    vector<float> getModule();
    vector<int> getNoHitsSensor();
    vector<unsigned int> getIsLong();
    vector<vector<PrPixelHit> > getHits();
    vector<PrPixelHit> getHitsSensor(int i);
    void prepareResults();
    vector<vector<unsigned int> > getResult();
    void compareTracks(vector<TrackS> tracks);
    

  private:
    int no_sensor;
    int no_hits;
    vector<float> module_z;
    vector<int> no_hits_sensor;
    vector<vector<PrPixelHit> > hits;
    vector<vector<unsigned int> > id_results;
    vector<unsigned int> isLong;
};
