// Positions: 2779
// Texels: 3374
// Normals: 2719
// Faces: 5038
// Vertices: 15114

#include "Mesh.h"

class Spider : public Mesh {
public:
   const int getVertices();
   const float* getPositions();
   const float* getTexels();
   const float* getNormals();
};