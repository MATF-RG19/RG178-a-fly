#include "SpiderWeb.h"

constexpr auto PI = 3.14159265359;

void SpiderWeb::createSpiderWeb(int numOfRings, int numOfStrings) {
   this->numOfRings = numOfRings;
   this->numOfStrings = numOfStrings;
   Position centerPosition;
   centerPosition.x = 0;
   centerPosition.y = 0;
   centerPosition.z = 0;

   Position** positions = new Position * [numOfRings];
   for (int i = 0; i < numOfRings; i++) {
      positions[i] = new Position[numOfStrings];
   }

   for (int i = 1; i <= numOfRings; i++) {
      for (int j = 1; j <= numOfStrings; j++) {
         positions[i - 1][j - 1].x = (i * 2.8 / numOfRings) * sin(j * (PI / (numOfStrings / 2)));
         positions[i - 1][j - 1].y = 0;
         positions[i - 1][j - 1].z = (i * 2.8 / numOfRings) * cos(j * (PI / (numOfStrings / 2)));;
      }
   }

   this->strongStrings = new WebString[numOfStrings * numOfRings];
   this->weakStrings = new WebString[numOfStrings * numOfRings];

   for (int i = 0; i < numOfRings; i++) {
      for (int j = 0; j < numOfStrings; j++) {
         if (i == 0) {
            this->strongStrings[i * numOfStrings + j].first = centerPosition;
            this->strongStrings[i * numOfStrings + j].last = positions[i][j];
         }
         else {
            this->strongStrings[i * numOfStrings + j].first = positions[i - 1][j];
            this->strongStrings[i * numOfStrings + j].last = positions[i][j];
         }
         this->strongStrings[i * numOfStrings + j].health = 2;
      }
   }

   for (int i = 0; i < numOfRings; i++) {
      for (int j = 0; j < numOfStrings; j++) {
         this->weakStrings[i * numOfStrings + j].first = positions[i][j];
         if (j + 1 == numOfStrings)
            this->weakStrings[i * numOfStrings + j].last = positions[i][0];
         else
            this->weakStrings[i * numOfStrings + j].last = positions[i][j + 1];
         this->weakStrings[i * numOfStrings + j].health = 1;
      }
   }

   for (int i = 0; i < numOfRings; i++) {
      delete[] positions[i];
   }
   delete[] positions;

   createLandingSpots();
}

void SpiderWeb::createLandingSpots() {
   this->landingSpots = new Position[getNumOfLandingSpots()];
   for (int i = 0; i < getNumOfLandingSpots(); i++) {
      float startX;
      float startY;
      float startZ;
      if ((i / getNumOfStrings()) % 2 == 0) {
         int tmpCurPos = ((i / getNumOfStrings()) / 2) * getNumOfStrings() + i % getNumOfStrings();
         startX = (getStrongStrings()[tmpCurPos].first.x + getStrongStrings()[tmpCurPos].last.x) / 2;
         startY = (getStrongStrings()[tmpCurPos].first.y + getStrongStrings()[tmpCurPos].last.y) / 2;
         startZ = (getStrongStrings()[tmpCurPos].first.z + getStrongStrings()[tmpCurPos].last.z) / 2;
      }
      else {
         int tmpCurPos = ((i / getNumOfStrings() - 1) / 2) * getNumOfStrings() + i % getNumOfStrings();
         startX = (getWeakStrings()[tmpCurPos].first.x + getWeakStrings()[tmpCurPos].last.x) / 2;
         startY = (getWeakStrings()[tmpCurPos].first.y + getWeakStrings()[tmpCurPos].last.y) / 2;
         startZ = (getWeakStrings()[tmpCurPos].first.z + getWeakStrings()[tmpCurPos].last.z) / 2;
      }

      landingSpots[i].x = startX;
      landingSpots[i].y = startY;
      landingSpots[i].z = startZ;
   }
}

WebString* SpiderWeb::getStrongStrings() {
   return this->strongStrings;
}

WebString* SpiderWeb::getWeakStrings() {
   return this->weakStrings;
}

int SpiderWeb::getNumOfRings() {
   return this->numOfRings;
}

int SpiderWeb::getNumOfStrings() {
   return this->numOfStrings;
}

Position* SpiderWeb::getLandingSpots() {
   return this->landingSpots;
}

int SpiderWeb::getNumOfLandingSpots() {
   return 2 * getNumOfStrings() * getNumOfRings();
}

void SpiderWeb::freeMemory() {
   delete[] strongStrings;
   delete[] weakStrings;
   delete[] landingSpots;
}