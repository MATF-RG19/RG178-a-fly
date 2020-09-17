#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "Structs.h"

using namespace std;

class SpiderWeb {
private:
   //number of web rings
   int numOfRings;
   //number of web support threads
   int numOfStrings;
   WebString* strongStrings;
   WebString* weakStrings;
   Position* landingSpots;
   GraphPosition* spiderSpots;
public:
   WebString* getStrongStrings();
   WebString* getWeakStrings();
   int getNumOfRings();
   int getNumOfStrings();
   Position* getLandingSpots();
   GraphPosition* getSpiderSpots();
   int getNumOfLandingSpots();
   int getNumOfSpiderSpots();
   void createSpiderWeb(int numOfRings, int numOfStrings);
   void createLandingSpots();
   void createSpiderSpots();
   void freeMemory();
};

