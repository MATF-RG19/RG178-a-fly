#include "DrawUtils.h"

DrawUtils::DrawUtils() {
   this->spiderWeb = NULL;
   this->spider = NULL;
   this->fly = NULL;
   this->landingPosition = 0;
}

DrawUtils::DrawUtils(SpiderWeb* spiderWeb, Spider* spider, Fly* fly) {
   this->spiderWeb = spiderWeb;
   this->spider = spider;
   this->fly = fly;
   this->landingPosition = 0;
}

SpiderWeb DrawUtils::getSpiderWeb() {
   return *(this->spiderWeb);
}

Spider DrawUtils::getSpider() {
   return *(this->spider);
}

Fly DrawUtils::getFly() {
   return *(this->fly);
}

int DrawUtils::getLandingPosition() {
   return this->landingPosition;
}

void DrawUtils::drawSpiderWeb() {
   glDisable(GL_LIGHTING);
   glLineWidth(3);
   glColor3f(1, 1, 1);
   glBegin(GL_LINES);

   for (int i = 0; i < getSpiderWeb().getNumOfStrings() * getSpiderWeb().getNumOfRings(); i++) {
      if (getSpiderWeb().getStrongStrings()[i].health > 0) {
         glVertex3f(getSpiderWeb().getStrongStrings()[i].first.x, getSpiderWeb().getStrongStrings()[i].first.y, getSpiderWeb().getStrongStrings()[i].first.z);
         glVertex3f(getSpiderWeb().getStrongStrings()[i].last.x, getSpiderWeb().getStrongStrings()[i].last.y, getSpiderWeb().getStrongStrings()[i].last.z);
      }
   }

   for (int i = 0; i < getSpiderWeb().getNumOfStrings() * getSpiderWeb().getNumOfRings(); i++) {
      if (getSpiderWeb().getWeakStrings()[i].health > 0) {
         glVertex3f(getSpiderWeb().getWeakStrings()[i].first.x, getSpiderWeb().getWeakStrings()[i].first.y, getSpiderWeb().getWeakStrings()[i].first.z);
         glVertex3f(getSpiderWeb().getWeakStrings()[i].last.x, getSpiderWeb().getWeakStrings()[i].last.y, getSpiderWeb().getWeakStrings()[i].last.z);
      }
   }

   glEnd();
   glEnable(GL_LIGHTING);
}

void DrawUtils::drawModel(Mesh* mesh, GLfloat material[4]) {
   glBegin(GL_TRIANGLES);
   GLfloat no_material[] = { 0, 0, 0, 1 };
   GLfloat no_shininess[] = { 0 };
   glMaterialfv(GL_FRONT, GL_DIFFUSE, material);
   glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
   glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
   glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
   for (int i = 0; i < (mesh->getVertices() * 3); ) {
      GLfloat x = mesh->getPositions()[i];
      GLfloat nx = mesh->getNormals()[i++];
      GLfloat y = mesh->getPositions()[i];
      GLfloat ny = mesh->getNormals()[i++];
      GLfloat z = mesh->getPositions()[i];
      GLfloat nz = mesh->getNormals()[i++];
      glNormal3f(nx, ny, nz);
      glVertex3f(x, y, z);
   }
   glEnd();

   glDrawArrays(GL_TRIANGLES, 0, mesh->getVertices());
}

void DrawUtils::drawSpider() {
   glScaled(0.25, 0.25, 0.25);
   GLclampf spiderMatRed = 20.0 / 255;
   GLclampf spiderMatGreen = 20.0 / 255;
   GLclampf spiderMatBlue = 20.0 / 255;
   GLfloat material_diffuse_spider[] = { spiderMatRed, spiderMatGreen, spiderMatBlue, 1 };
   drawModel(spider, material_diffuse_spider);
   glScaled(4, 4, 4);
}

void DrawUtils::drawFly(int cursorPosition, Position animationOffset) {
   Position position = Position();
   if ((cursorPosition / getSpiderWeb().getNumOfStrings()) % 2 == 0) {
      int tmpCurPos = ((cursorPosition / getSpiderWeb().getNumOfStrings()) / 2) * getSpiderWeb().getNumOfStrings() + cursorPosition % getSpiderWeb().getNumOfStrings();
      position.x = (getSpiderWeb().getStrongStrings()[tmpCurPos].first.x + getSpiderWeb().getStrongStrings()[tmpCurPos].last.x) / 2;
      position.y = (getSpiderWeb().getStrongStrings()[tmpCurPos].first.y + getSpiderWeb().getStrongStrings()[tmpCurPos].last.y) / 2;
      position.z = (getSpiderWeb().getStrongStrings()[tmpCurPos].first.z + getSpiderWeb().getStrongStrings()[tmpCurPos].last.z) / 2;
   }
   else {
      int tmpCurPos = ((cursorPosition / getSpiderWeb().getNumOfStrings() - 1) / 2) * getSpiderWeb().getNumOfStrings() + cursorPosition % getSpiderWeb().getNumOfStrings();
      position.x = (getSpiderWeb().getWeakStrings()[tmpCurPos].first.x + getSpiderWeb().getWeakStrings()[tmpCurPos].last.x) / 2;
      position.y = (getSpiderWeb().getWeakStrings()[tmpCurPos].first.y + getSpiderWeb().getWeakStrings()[tmpCurPos].last.y) / 2;
      position.z = (getSpiderWeb().getWeakStrings()[tmpCurPos].first.z + getSpiderWeb().getWeakStrings()[tmpCurPos].last.z) / 2;
   }
   position = position + animationOffset + Position(0.0f, 0.025f, 0.0f);

   glTranslatef(position.x, position.y, position.z);
   glScaled(0.02, 0.02, 0.02);
   GLclampf flyMatRed = 5.0 / 255;
   GLclampf flyMatGreen = 5.0 / 255;
   GLclampf flyMatBlue = 5.0 / 255;
   GLfloat material_diffuse_fly[] = { flyMatRed, flyMatGreen, flyMatBlue, 1 };
   drawModel(fly, material_diffuse_fly);
   glScaled(50, 50, 50);
   glTranslatef(-position.x, -position.y, -position.z);
}

void DrawUtils::drawSphere(float size, int slices, int stacks, GLfloat materialDiffuse[4]) {
   GLfloat no_material[] = { 0, 0, 0, 1 };

   GLclampf bRed = 252.0 / 255;
   GLclampf bGreen = 239.0 / 255;
   GLclampf bBlue = 154.0 / 255;
   GLfloat material_ambient[] = { bRed / 5, bGreen / 5, bBlue / 5, 1 };

   GLfloat material_ambient_heterogeneous[] = { 0.8, 0.8, 0.2, 1 };

   GLfloat material_specular[] = { 1, 1, 1, 1 };

   GLfloat no_shininess[] = { 0 };

   GLfloat low_shininess[] = { 5 };

   GLfloat high_shininess[] = { 100 };


   GLfloat material_emission[] = { materialDiffuse[0] / 5, materialDiffuse[1] / 5, materialDiffuse[2] / 5, 1 };

   glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
   glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);

   glutSolidSphere(size, slices, stacks);
}

void DrawUtils::drawLandingSpots(MouseUtils mouseUtils) {
   for (int i = 0; i < getSpiderWeb().getNumOfLandingSpots(); i++) {
      int health = 0;
      if ((i / getSpiderWeb().getNumOfStrings()) % 2 == 0) {
         int tmpCurPos = ((i / getSpiderWeb().getNumOfStrings()) / 2) * getSpiderWeb().getNumOfStrings() + i % getSpiderWeb().getNumOfStrings();
         health = getSpiderWeb().getStrongStrings()[tmpCurPos].health;
      }
      else {
         int tmpCurPos = ((i / getSpiderWeb().getNumOfStrings() - 1) / 2) * getSpiderWeb().getNumOfStrings() + i % getSpiderWeb().getNumOfStrings();
         health = getSpiderWeb().getWeakStrings()[tmpCurPos].health;
      }
      if (health > 0) {
         GLclampf redMat = 110.0 / 255;
         GLclampf greenMat = 190.0 / 255;
         GLclampf blueMat = 255.0 / 255;
         if (health == 2) {
            redMat = 0.0 / 255;
            greenMat = 115.0 / 255;
            blueMat = 210.0 / 255;
         }
         if (mouseUtils.rayTest(getSpiderWeb().getLandingSpots()[i], 0.025)) {
            landingPosition = i;
            redMat = 255.0 / 255;
            greenMat = 224.0 / 255;
            blueMat = 66.0 / 255;
         }
         glTranslatef(getSpiderWeb().getLandingSpots()[i].x, getSpiderWeb().getLandingSpots()[i].y, getSpiderWeb().getLandingSpots()[i].z);
         GLfloat materialDiffuse[] = { redMat, greenMat, blueMat, 1 };
         drawSphere(0.025, 8, 8, materialDiffuse);
         glTranslatef(-getSpiderWeb().getLandingSpots()[i].x, -getSpiderWeb().getLandingSpots()[i].y, -getSpiderWeb().getLandingSpots()[i].z);
      }
   }
}

void DrawUtils::drawText(const char* text, int length, int x, int y) {
   glMatrixMode(GL_PROJECTION);
   double* matrix = new double[16];
   glGetDoublev(GL_PROJECTION_MATRIX, matrix);
   glLoadIdentity();
   glOrtho(0, 1900, 0, 1000, -5, 5);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glPushMatrix();
   glLoadIdentity();
   glRasterPos2d(x, y);
   for (int i = 0; i < length; i++) {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
   }
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixd(matrix);
   glMatrixMode(GL_MODELVIEW);
}
