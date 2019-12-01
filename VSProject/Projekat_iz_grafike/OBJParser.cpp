#include "OBJParser.h"

Model OBJParser::getModelInfo(string fileName) {
   Model model = { 0 };

   ifstream inOBJ;
   inOBJ.open(fileName);
   if (!inOBJ.good()) {
      cout << "ERROR OPENING OBJ FILE" << endl;
      exit(1);
   }

   while (!inOBJ.eof()) {
      string line;
      getline(inOBJ, line);
      string type = line.substr(0, 2);

      if (type.compare("v ") == 0)
         model.positions++;
      else if (type.compare("vt") == 0)
         model.texels++;
      else if (type.compare("vn") == 0)
         model.normals++;
      else if (type.compare("f ") == 0)
         model.faces++;
   }

   model.vertices = model.faces * 3;

   inOBJ.close();

   return model;
}

void OBJParser::extractOBJdata(string fp, float** positions, float** texels, float** normals, int** faces) {
   // Counters
   int p = 0;
   int t = 0;
   int n = 0;
   int f = 0;

   // Open OBJ file
   ifstream objFile;
   ofstream outputFile;
   outputFile.open("e:\\Blender files\\Spider\\spider.txt");
   objFile.open(fp);
   if (!objFile.good()) {
      cout << "ERROR OPENING OBJ FILE" << endl;
      exit(1);
   }

   // Read OBJ file
   while (!objFile.eof()) {
      string line;
      getline(objFile, line);
      string type = line.substr(0, 2);

      // Positions
      if (type.compare("v ") == 0) {
         char expression[] = "(-?\\d+\\.\\d+)";
         regex pos_regex(expression);
         auto words_begin =
            sregex_iterator(line.begin(), line.end(), pos_regex);
         auto words_end = sregex_iterator();

         int k = 0;
         for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string match_str = match.str();
            outputFile << match_str << ' ';
            positions[p][k] = stof(match_str);
            k++;
         }
         p++;
         outputFile << endl;
      }

      // Texels
      else if (type.compare("vt") == 0) {
         char expression[] = "(-?\\d+\\.\\d+)";
         regex pos_regex(expression);
         auto words_begin =
            sregex_iterator(line.begin(), line.end(), pos_regex);
         auto words_end = sregex_iterator();

         int k = 0;
         for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string match_str = match.str();
            outputFile << match_str << ' ';
            texels[t][k] = stof(match_str);
            k++;
         }
         t++;
         outputFile << endl;
      }

      // Normals
      else if (type.compare("vn") == 0) {
         char expression[] = "(-?\\d+\\.\\d+)";
         regex pos_regex(expression);
         auto words_begin =
            sregex_iterator(line.begin(), line.end(), pos_regex);
         auto words_end = sregex_iterator();

         int k = 0;
         for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string match_str = match.str();
            outputFile << match_str << ' ';
            normals[n][k] = stof(match_str);
            k++;
         }
         n++;
         outputFile << endl;
      }

      // Faces
      else if (type.compare("f ") == 0) {
         char expression[] = "(\\d+)";
         regex pos_regex(expression);
         auto words_begin =
            sregex_iterator(line.begin(), line.end(), pos_regex);
         auto words_end = sregex_iterator();

         int k = 0;
         for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string match_str = match.str();
            outputFile << match_str << ' ';
            faces[f][k] = stof(match_str);
            k++;
         }
         f++;
         outputFile << endl;
      }
   }
   // Close OBJ file
   objFile.close();
   outputFile.close();
}

void OBJParser::writeH(string fp, string name, Model model) {
   ofstream outH;
   outH.open(fp);
   if (!outH.good()) {
      cout << "ERROR CREATING H FILE" << endl;
      exit(1);
   }

   outH << "// Positions: " << model.positions << endl;
   outH << "// Texels: " << model.texels << endl;
   outH << "// Normals: " << model.normals << endl;
   outH << "// Faces: " << model.faces << endl;
   outH << "// Vertices: " << model.vertices << endl;
   outH << endl;
   outH << "#include \"Mesh.h\"" << endl;
   outH << endl;
   outH << "class " + name + " : public Mesh {" << endl;
   outH << "public:" << endl;
   outH << "   const int getVertices();" << endl;
   outH << "   const float* getPositions();" << endl;
   outH << "   const float* getTexels();" << endl;
   outH << "   const float* getNormals();" << endl;
   outH << "};";

   outH.close();
}

void OBJParser::writeCppVertices(string fp, string name, Model model) {
   ofstream outC;
   outC.open(fp);
   if (!outC.good()) {
      cout << "ERROR CREATING Cpp FILE" << endl;
      exit(1);
   }

   outC << "#include " << "\"" << name << ".h" << "\"" << endl;
   outC << endl;

   // Vertices
   outC << "const int vertices = " << model.vertices << ";" << endl;
   outC << endl;

   outC.close();
}

void OBJParser::writeCppPositions(string fp, string name, Model model, int** faces, float** positions) {
   ofstream outC;
   outC.open(fp, ios::app);

   outC << "const float positions[" << model.vertices * 3 << "] = {" << endl;
   for (int i = 0; i < model.faces; i++) {
      int vA = faces[i][0] - 1;
      int vB = faces[i][3] - 1;
      int vC = faces[i][6] - 1;

      outC << "   " << positions[vA][0] << ", " << positions[vA][1] << ", " << positions[vA][2] << ", " << endl;
      outC << "   " << positions[vB][0] << ", " << positions[vB][1] << ", " << positions[vB][2] << ", " << endl;
      outC << "   " << positions[vC][0] << ", " << positions[vC][1] << ", " << positions[vC][2] << ", " << endl;
   }
   outC << "};" << endl;
   outC << endl;

   outC.close();
}

void OBJParser::writeCppTexels(string fp, string name, Model model, int** faces, float** texels) {
   ofstream outC;
   outC.open(fp, ios::app);

   outC << "const float texels[" << model.vertices * 2 << "] = {" << endl;
   for (int i = 0; i < model.faces; i++) {
      int vtA = faces[i][1] - 1;
      int vtB = faces[i][4] - 1;
      int vtC = faces[i][7] - 1;

      outC << "   " << texels[vtA][0] << ", " << texels[vtA][1] << ", " << endl;
      outC << "   " << texels[vtB][0] << ", " << texels[vtB][1] << ", " << endl;
      outC << "   " << texels[vtC][0] << ", " << texels[vtC][1] << ", " << endl;
   }
   outC << "};" << endl;
   outC << endl;

   outC.close();
}

void OBJParser::writeCppNormals(string fp, string name, Model model, int** faces, float** normals) {
   ofstream outC;
   outC.open(fp, ios::app);

   outC << "const float normals[" << model.vertices * 3 << "] = {" << endl;
   for (int i = 0; i < model.faces; i++) {
      int vnA = faces[i][2] - 1;
      int vnB = faces[i][5] - 1;
      int vnC = faces[i][8] - 1;

      outC << "   " << normals[vnA][0] << ", " << normals[vnA][1] << ", " << normals[vnA][2] << ", " << endl;
      outC << "   " << normals[vnB][0] << ", " << normals[vnB][1] << ", " << normals[vnB][2] << ", " << endl;
      outC << "   " << normals[vnC][0] << ", " << normals[vnC][1] << ", " << normals[vnC][2] << ", " << endl;
   }
   outC << "};" << endl;
   outC << endl;

   outC.close();
}

void OBJParser::writeOverrideMethods(string fp, string name) {
   ofstream outC;
   outC.open(fp, ios::app);

   outC << "const int " + name + "::getVertices() {" << endl;
   outC << "   return vertices;" << endl;
   outC << "}" << endl;
   outC << endl;
   outC << "const float* " + name + "::getPositions() {" << endl;
   outC << "   return positions;" << endl;
   outC << "}" << endl;
   outC << endl;
   outC << "const float* " + name + "::getTexels() {" << endl;
   outC << "   return texels;" << endl;
   outC << "}" << endl;
   outC << endl;
   outC << "const float* " + name + "::getNormals() {" << endl;
   outC << "   return normals;" << endl;
   outC << "}" << endl;


   outC.close();
}

void OBJParser::generateFiles(string fp, string objectName) {
   Model model = getModelInfo(fp + ".obj");
   cout << "Model Info" << endl;
   cout << "Positions: " << model.positions << endl;
   cout << "Texels: " << model.texels << endl;
   cout << "Normals: " << model.normals << endl;
   cout << "Faces: " << model.faces << endl;
   cout << "Vertices: " << model.vertices << endl;

   float** positions = new float* [model.positions];
   for (int i = 0; i < model.positions; i++) {
      positions[i] = new float[3];
   }
   float** texels = new float* [model.texels];
   for (int i = 0; i < model.texels; i++) {
      texels[i] = new float[2];
   }
   float** normals = new float* [model.normals];
   for (int i = 0; i < model.normals; i++) {
      normals[i] = new float[3];
   }
   int** faces = new int* [model.faces];
   for (int i = 0; i < model.faces; i++) {
      faces[i] = new int[9];
   }

   cout << "Extracting data..." << endl;
   extractOBJdata(fp + ".obj", positions, texels, normals, faces);
   cout << "Extracted!" << endl;
   cout << "Writing the files" << endl;
   writeH(fp + ".h", objectName, model);
   writeCppVertices(fp + ".cpp", objectName, model);
   writeCppPositions(fp + ".cpp", objectName, model, faces, positions);
   writeCppTexels(fp + ".cpp", objectName, model, faces, texels);
   writeCppNormals(fp + ".cpp", objectName, model, faces, normals);
   writeOverrideMethods(fp + ".cpp", objectName);
   cout << "Done!" << endl;

   for (int i = 0; i < model.positions; i++) {
      delete[] positions[i];
   }
   delete[] positions;
   for (int i = 0; i < model.texels; i++) {
      delete[] texels[i];
   }
   delete[] texels;
   for (int i = 0; i < model.normals; i++) {
      delete[] normals[i];
   }
   delete[] normals;
   for (int i = 0; i < model.faces; i++) {
      delete[] faces[i];
   }
   delete[] faces;
}


