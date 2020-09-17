// Positions: 434
// Texels: 566
// Normals: 400
// Faces: 830
// Vertices: 2490

#include "Mesh.h"

class Fly : public Mesh {
public:
   const int getVertices();
   const float* getPositions();
   const float* getTexels();
   const float* getNormals();
};