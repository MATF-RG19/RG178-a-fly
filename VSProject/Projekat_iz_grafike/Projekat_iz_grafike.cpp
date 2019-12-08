#include <GL/glut.h>
#include <cmath>
#include "OBJParser.h"
#include "Spider.h"

#define PI 3.14159265359

static void on_keyboard(unsigned char key, int x, int y);
static void processSpecialKeys(int key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
GLfloat rotationYaw = 0;
GLfloat rotationPitch = 0;
GLfloat step = 2;
Spider spider;

int main(int argc, char** argv) {
   spider = Spider();

   OBJParser parser;
   //parser.generateFiles("e:\\Blender files\\Spider\\Spider", "Spider");


   /* Ambient light color. */
   GLclampf redA = 138.0 / 255;
   GLclampf greenA = 226.0 / 255;
   GLclampf blueA = 255.0 / 255;
   GLfloat light_ambient[] = { redA, greenA, blueA, 1 };

   /* Diffuse light color. */
   GLfloat light_diffuse[] = { 1, 1, 1, 1 };

   /* Ambiental scene light. */
   GLfloat model_ambient[] = { 0.2, 0.2, 0.2, 1 };

   /* GLUT init */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

   /* Window creation */
   glutInitWindowSize(1900, 1000);
   glutInitWindowPosition(10, 10);
   glutCreateWindow(argv[0]);

   /* Register callback functions. */
   glutKeyboardFunc(on_keyboard);
   glutSpecialFunc(processSpecialKeys);
   glutReshapeFunc(on_reshape);
   glutDisplayFunc(on_display);

   /* OpenGl init */
   GLclampf red = 0.0 / 255;
   GLclampf green = 0.0 / 255;
   GLclampf blue = 0.0 / 255;
   glClearColor(red, green, blue, 0);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   glEnable(GL_LIGHTING);
   glShadeModel(GLU_SMOOTH);
   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

   glutMainLoop();

   return 0;
}

static void on_keyboard(unsigned char key, int x, int y) {
   switch (key) {
   case 27:
      exit(0);
      break;
   }


}

static void processSpecialKeys(int key, int x, int y) {
   switch (key) {
   case GLUT_KEY_LEFT:
      rotationYaw += step;
      glutPostRedisplay();
      break;
   case GLUT_KEY_RIGHT:
      rotationYaw -= step;
      glutPostRedisplay();
      break;
   case GLUT_KEY_UP:
      rotationPitch += step;
      glutPostRedisplay();
      break;
   case GLUT_KEY_DOWN:
      rotationPitch -= step;
      glutPostRedisplay();
      break;
   }
}

static void on_reshape(int width, int height) {
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, (float)width / height, 1, 100);
}

void drawModel(Mesh* mesh) {
   glBegin(GL_TRIANGLES);
   GLclampf spiderMatRed = 36.0 / 255;
   GLclampf spiderMatGreen = 36.0 / 255;
   GLclampf spiderMatBlue = 36.0 / 255;
   GLfloat material_diffuse_spider[] = { spiderMatRed, spiderMatGreen, spiderMatBlue, 1 };
   glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_spider);
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

void drawSpiderWeb() {
   glDisable(GL_LIGHTING);
   glLineWidth(3);
   glColor3f(1, 1, 1);
   glBegin(GL_LINES);

   //number of support threads
   int nsthr = 10;
   //number of rings
   int nrng = 8;

   for (int i = 0; i < nsthr; i++) {
      glVertex3f(0, 0, 0);
      glVertex3f(3 * sin(i * (3.14 / (nsthr / 2))), 0, 3 * cos(i * (3.14 / (nsthr / 2))));

      for (int j = 1; j <= nrng; j++) {
         float node1x = (j * 2.8 / nrng) * sin(i * (PI / (nsthr / 2)));
         float node1z = (j * 2.8 / nrng) * cos(i * (PI / (nsthr / 2)));
         float node2x = (j * 2.8 / nrng) * sin((i + 1) * (PI / (nsthr / 2)));
         float node2z = (j * 2.8 / nrng) * cos((i + 1) * (PI / (nsthr / 2)));
         glVertex3f(node1x, 0, node1z);
         glVertex3f(node2x, 0, node2z);
      }
   }

   glEnd();
   glEnable(GL_LIGHTING);
}

static void on_display(void) {
   GLfloat light_position[] = { 0, 24, 24, 1 };
   GLfloat light_direction[] = { 0,-1,-1 };
   GLfloat light_cutoff = 45;
   GLfloat light_exponent[] = { 8 };

   GLfloat no_material[] = { 0, 0, 0, 1 };

   GLfloat material_ambient[] = { 0.2, 0.2, 0.2, 1 };

   GLfloat material_ambient_heterogeneous[] = { 0.8, 0.8, 0.2, 1 };

   GLclampf redMat = 60.0 / 255;
   GLclampf greenMat = 210.0 / 255;
   GLclampf blueMat = 80.0 / 255;
   GLfloat material_diffuse[] = { redMat, greenMat, blueMat, 1 };

   GLfloat material_specular[] = { 1, 1, 1, 1 };

   GLfloat no_shininess[] = { 0 };

   GLfloat low_shininess[] = { 5 };

   GLfloat high_shininess[] = { 100 };

   GLfloat material_emission[] = { redMat / 20, greenMat / 20, blueMat / 20, 0 };

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0, 16, 40, 0, 16, 0, 0, 1, 0);

   glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light_cutoff);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, light_exponent);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

   glPushMatrix();

   glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
   glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
   glutSolidSphere(22, 256, 256);

   glTranslatef(0, sin(3.14 / 4) * 23, cos(3.14 / 4) * 23);
   glRotatef(rotationYaw, 0, 1, 0);
   glRotatef(rotationPitch + 31, 1, 0, 0);

   glScaled(0.2, 0.2, 0.2);
   drawModel(&spider);
   glScaled(5, 5, 5);

   glScaled(3, 3, 3);
   drawSpiderWeb();

   glPopMatrix();

   glutSwapBuffers();
}