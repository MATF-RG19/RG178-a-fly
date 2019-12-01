#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "Structs.h"

using namespace std;

class OBJParser {
public:
   Model getModelInfo(string fileName);
   void extractOBJdata(string fp, float** positions, float** texels, float** normals, int** faces);
   void writeH(string fp, string name, Model model);
   void writeCppVertices(string fp, string name, Model model);
   void writeCppPositions(string fp, string name, Model model, int** faces, float** positions);
   void writeCppTexels(string fp, string name, Model model, int** faces, float** texels);
   void writeCppNormals(string fp, string name, Model model, int** faces, float** normals);
   void writeOverrideMethods(string fp, string name);
   void generateFiles(string fp, string objectName);
};

