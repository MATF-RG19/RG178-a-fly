#include "MouseUtils.h"

MouseUtils::MouseUtils() {}

MouseUtils::MouseUtils(double* mouseX, double* mouseY) {
   this->mouseX = mouseX;
   this->mouseY = mouseY;
}

void MouseUtils::calculateRay() {
   double matModelView[16], matProjection[16];
   int viewport[4];
   glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
   glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
   glGetIntegerv(GL_VIEWPORT, viewport);
   double winX = *mouseX;
   double winY = viewport[3] - *mouseY;

   gluUnProject(winX, winY, 0.0, matModelView, matProjection,
      viewport, &mouseRayStart.x, &mouseRayStart.y, &mouseRayStart.z);
   gluUnProject(winX, winY, 1.0, matModelView, matProjection,
      viewport, &mouseRayEnd.x, &mouseRayEnd.y, &mouseRayEnd.z);
}

float MouseUtils::distancePointToLine(Position lineStart, Position lineEnd, Position point) {
   float a = sqrt((lineEnd - lineStart) * (lineEnd - lineStart));
   float b = sqrt((point - lineEnd) * (point - lineEnd));
   float c = sqrt((lineStart - point) * (lineStart - point));

   float s = (a + b + c) / 2;
   return sqrt(s * (s - a) * (s - b) * (s - c)) / a;
}

Position MouseUtils::convertToPosition(MousePosition P) {
   return Position((float)(P.x), (float)(P.y), (float)(P.z));
}

bool MouseUtils::rayTest(const Position center, float radius) {
   float len = distancePointToLine(convertToPosition(mouseRayStart), convertToPosition(mouseRayEnd), center);
   return len <= radius;
}