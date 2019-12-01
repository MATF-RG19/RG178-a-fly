#pragma once
class Mesh {
public:
   virtual const int getVertices() = 0;
   virtual const float* getPositions() = 0;
   virtual const float* getTexels() = 0;
   virtual const float* getNormals() = 0;
};

