#pragma once
#include "Structs.h"
#include <math.h>

class MouseUtils {
private:
   double* mouseX;
   double* mouseY;
   MousePosition mouseRayStart;
   MousePosition mouseRayEnd;
   float distancePointToLine(Position lineStart, Position lineEnd, Position point);
   Position convertToPosition(MousePosition P);
public:
   MouseUtils();
   MouseUtils(double* mouseX, double* mouseY);
   void calculateRay();
   bool rayTest(const Position center, float radius);
};

