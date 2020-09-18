#pragma once
#include <GL/glut.h>
#include <math.h>

struct Model {
   int vertices;
   int positions;
   int texels;
   int normals;
   int faces;
};

struct Position {
   float x;
   float y;
   float z;

   Position(float x = 0, float y = 0, float z = 0)
      : x(x), y(y), z(z) {
   }

   Position& operator=(const Position& a) {
      x = a.x;
      y = a.y;
      z = a.z;
      return *this;
   }

   Position operator+(const Position& a) const {
      return Position(a.x + x, a.y + y, a.z + z);
   }

   Position operator-(const Position& a) const {
      return Position(x - a.x, y - a.y, z - a.z);
   }

   float operator*(const Position& a) const {
      return a.x * x + a.y * y + a.z * z;
   }

   Position operator*(const float& a) const {
      return Position(a * x, a * y, a * z);
   }

   Position operator/(const float& a) const {
      return Position(x / a, y / a, z / a);
   }

   bool operator==(const Position& a) const {
      return (x == a.x && y == a.y && z == a.z);
   }
};

struct MousePosition {
   GLdouble x, y, z;

   MousePosition(GLdouble x = 0, GLdouble y = 0, GLdouble z = 0)
      : x(x), y(y), z(z) {
   }

   MousePosition& operator=(const MousePosition& a) {
      x = a.x;
      y = a.y;
      z = a.z;
      return *this;
   }

   MousePosition operator+(const MousePosition& a) const {
      return MousePosition(x + a.x, y + a.y, z + a.z);
   }

   MousePosition operator-(const MousePosition& a) const {
      return MousePosition(x - a.x, y - a.y, z - a.z);
   }

   MousePosition operator*(const float& a) const {
      return MousePosition(x * a, y * a, z * a);
   }

   MousePosition operator/(const float& a) const {
      return MousePosition(x / a, y / a, z / a);
   }

   float norm() const {
      return fabs(x) + fabs(y) + fabs(z);
   }

   MousePosition normalize() const {
      return MousePosition(x, y, z) / norm();
   }
};

struct WebString {
   Position first, last;
   int health;
};