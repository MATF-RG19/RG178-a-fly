#pragma once
typedef struct Model {
   int vertices;
   int positions;
   int texels;
   int normals;
   int faces;
} Model;

typedef struct SpiderWebNode {
   float x, y, z;
   int connections[4];
} SpiderWebNode;