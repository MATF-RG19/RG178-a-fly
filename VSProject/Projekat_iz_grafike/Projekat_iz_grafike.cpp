#include <GL/glut.h>
#include <cmath>
#include "OBJParser.h"
#include "DrawUtils.h"

constexpr auto PI = 3.14159265359;

static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse_click(int button, int state, int mouse_x, int mouse_y);
static void on_special_keys(int key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);
GLfloat rotationYaw = 0;
GLfloat rotationPitch = 0;
GLfloat step = 2;
SpiderWeb spiderWeb;
Spider spider;
Fly fly;
DrawUtils drawUtils;
MouseUtils mouseUtils;
int windowWidth = 1900;
int windowHeight = 1000;

//Position of the clicked landing spot
int cursorPosition;

//Vector from the current position of the fly to the position marked with the mouse
Position animationOffset;
//Vector perpendicular to the spider web plain
Position animationOffsetUp;
//Step for the movement on the spider web plain
Position dAnimationOffset;
//Step for the movement above the spider web plain
Position dAnimationOffsetUp;


//Current position of the spider
Position spiderPosition;
//Vector from the spider to the fly
Position spiderOffset;
//Step for the spider movement
Position dSpiderOffset;


//If true, objects on the scene should be moving
bool flyAnimationActive, spiderAnimationActive;

//Mouse screen position
double mouseX, mouseY;

//Number of web stings destroyed
int score;

//If true, the game is over
bool gameOver;

//Calculates the vector between two landing spots and sets the flight speed for the fly
void calculateAnimationOffset(int startPosition, int endPosition) {
   Position start;
   if ((startPosition / spiderWeb.getNumOfStrings()) % 2 == 0) {
      int tmpCurPos = ((startPosition / spiderWeb.getNumOfStrings()) / 2) * spiderWeb.getNumOfStrings() + startPosition % spiderWeb.getNumOfStrings();
      start.x = (spiderWeb.getStrongStrings()[tmpCurPos].first.x + spiderWeb.getStrongStrings()[tmpCurPos].last.x) / 2;
      start.y = (spiderWeb.getStrongStrings()[tmpCurPos].first.y + spiderWeb.getStrongStrings()[tmpCurPos].last.y) / 2;
      start.z = (spiderWeb.getStrongStrings()[tmpCurPos].first.z + spiderWeb.getStrongStrings()[tmpCurPos].last.z) / 2;
   }
   else {
      int tmpCurPos = ((startPosition / spiderWeb.getNumOfStrings() - 1) / 2) * spiderWeb.getNumOfStrings() + startPosition % spiderWeb.getNumOfStrings();
      start.x = (spiderWeb.getWeakStrings()[tmpCurPos].first.x + spiderWeb.getWeakStrings()[tmpCurPos].last.x) / 2;
      start.y = (spiderWeb.getWeakStrings()[tmpCurPos].first.y + spiderWeb.getWeakStrings()[tmpCurPos].last.y) / 2;
      start.z = (spiderWeb.getWeakStrings()[tmpCurPos].first.z + spiderWeb.getWeakStrings()[tmpCurPos].last.z) / 2;
   }

   Position end;
   if ((endPosition / spiderWeb.getNumOfStrings()) % 2 == 0) {
      int tmpCurPos = ((endPosition / spiderWeb.getNumOfStrings()) / 2) * spiderWeb.getNumOfStrings() + endPosition % spiderWeb.getNumOfStrings();
      end.x = (spiderWeb.getStrongStrings()[tmpCurPos].first.x + spiderWeb.getStrongStrings()[tmpCurPos].last.x) / 2;
      end.y = (spiderWeb.getStrongStrings()[tmpCurPos].first.y + spiderWeb.getStrongStrings()[tmpCurPos].last.y) / 2;
      end.z = (spiderWeb.getStrongStrings()[tmpCurPos].first.z + spiderWeb.getStrongStrings()[tmpCurPos].last.z) / 2;
   }
   else {
      int tmpCurPos = ((endPosition / spiderWeb.getNumOfStrings() - 1) / 2) * spiderWeb.getNumOfStrings() + endPosition % spiderWeb.getNumOfStrings();
      end.x = (spiderWeb.getWeakStrings()[tmpCurPos].first.x + spiderWeb.getWeakStrings()[tmpCurPos].last.x) / 2;
      end.y = (spiderWeb.getWeakStrings()[tmpCurPos].first.y + spiderWeb.getWeakStrings()[tmpCurPos].last.y) / 2;
      end.z = (spiderWeb.getWeakStrings()[tmpCurPos].first.z + spiderWeb.getWeakStrings()[tmpCurPos].last.z) / 2;
   }
   animationOffset = start - end;
   dAnimationOffset = animationOffset / 150.0f;
   animationOffsetUp = Position();
   dAnimationOffsetUp = Position(0.0f, 0.015f, 0.0f);

   spiderOffset = spiderPosition - end;
   dSpiderOffset = (spiderOffset / sqrt(spiderOffset * spiderOffset)) / 200.0f;
}

int main(int argc, char** argv) {
   spider = Spider();
   fly = Fly();
   spiderWeb = SpiderWeb();
   spiderPosition = Position();

   flyAnimationActive = false;
   spiderAnimationActive = false;

   /*OBJParser parser;
   parser.generateFiles("d:\\Blender files\\Fly\\fly", "Fly");
   exit(0);*/

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
   glutInitWindowSize(windowWidth, windowHeight);
   glutInitWindowPosition(10, 10);
   glutCreateWindow(argv[0]);

   /* Register callback functions. */
   glutKeyboardFunc(on_keyboard);
   glutSpecialFunc(on_special_keys);
   glutReshapeFunc(on_reshape);
   glutDisplayFunc(on_display);
   glutMouseFunc(on_mouse_click);

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

   spiderWeb.createSpiderWeb(10, 8);

   cursorPosition = rand() % spiderWeb.getNumOfLandingSpots();

   drawUtils = DrawUtils(&spiderWeb, &spider, &fly);

   mouseUtils = MouseUtils(&mouseX, &mouseY);

   score = 0;
   gameOver = false;

   glutMainLoop();

   return 0;
}

static void on_keyboard(unsigned char key, int x, int y) {
   int previousPosition;
   switch (key) {
   case 27:
      spiderWeb.freeMemory();
      exit(0);
      break;
   case 32:
      if (!flyAnimationActive && !gameOver) {
         previousPosition = cursorPosition;
         cursorPosition = drawUtils.getLandingPosition();
         if (cursorPosition != previousPosition) {
            calculateAnimationOffset(previousPosition, cursorPosition);
            if ((previousPosition / spiderWeb.getNumOfStrings()) % 2 == 0) {
               int tmpPos = ((previousPosition / spiderWeb.getNumOfStrings()) / 2) * spiderWeb.getNumOfStrings() + previousPosition % spiderWeb.getNumOfStrings();
               spiderWeb.getStrongStrings()[tmpPos].health--;
               if (spiderWeb.getStrongStrings()[tmpPos].health == 0)
                  score += 2;
            }
            else {
               int tmpPos = ((previousPosition / spiderWeb.getNumOfStrings()) / 2) * spiderWeb.getNumOfStrings() + previousPosition % spiderWeb.getNumOfStrings();
               spiderWeb.getWeakStrings()[tmpPos].health--;
               score++;
            }
            flyAnimationActive = true;
            if (!spiderAnimationActive)
               glutTimerFunc(10, on_timer, 0);
         }
      }
      break;
   }
}

static void on_mouse_click(int button, int state, int x, int y) {
   if (state != GLUT_DOWN or gameOver)
      return;

   windowWidth = glutGet(GLUT_WINDOW_WIDTH);
   windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
   mouseX = x;
   mouseY = y;

   GLbyte color[4];
   GLfloat depth;
   GLuint index;

   glReadPixels(mouseX, windowHeight - mouseY - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
   glReadPixels(mouseX, windowHeight - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
   glReadPixels(mouseX, windowHeight - mouseY - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

   glutPostRedisplay();
}

static void on_special_keys(int key, int x, int y) {
   switch (key) {
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


static void on_timer(int value) {
   if (value != 0)
      return;

   if (flyAnimationActive) {
      animationOffset = animationOffset - dAnimationOffset + dAnimationOffsetUp;
      dAnimationOffsetUp.y -= 0.0002f;
   }

   if (fabs(animationOffset.x) <= 0.0001 && fabs(animationOffset.z) <= 0.0001) {
      flyAnimationActive = false;
      animationOffset = Position();
   }

   if (spiderOffset * spiderOffset > 0.0001) {
      spiderOffset = spiderOffset - dSpiderOffset;
      spiderPosition = spiderPosition - dSpiderOffset;
      spiderAnimationActive = true;
   }
   else {
      spiderAnimationActive = false;
      gameOver = true;
   }

   if (flyAnimationActive || spiderAnimationActive)
      glutTimerFunc(10, on_timer, 0);

   glutPostRedisplay();
}

static void on_reshape(int width, int height) {
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, (float)width / height, 1, 100);
}

void setUpLight() {
   GLfloat light_position[] = { 0, 24, 24, 1 };
   GLfloat light_direction[] = { 0,-1,-1 };
   GLfloat light_cutoff = 45;
   GLfloat light_exponent[] = { 8 };

   glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light_cutoff);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, light_exponent);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
}

float calculateAngle(const Position A, const Position B) {
   if (A * A == 0 || B * B == 0) {
      return 0;
   }
   Position normA = (A / sqrt(A * A));
   Position normB = (B / sqrt(B * B));
   if (normB.x < 0.0f)
      return acos(normA * normB) * 180.0f / PI;
   else
      return 360.0f - (acos(normA * normB) * 180.0f / PI);
}

static void on_display(void) {
   GLclampf bRed = 47.0 / 255;
   GLclampf bGreen = 216.0 / 255;
   GLclampf bBlue = 250.0 / 255;
   glClearColor(bRed, bGreen, bBlue, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0, 16, 40, 0, 16, 0, 0, 1, 0);

   glPushMatrix();

   GLclampf redMat = 60.0 / 255;
   GLclampf greenMat = 210.0 / 255;
   GLclampf blueMat = 80.0 / 255;
   GLfloat materialDiffuse[] = { redMat, greenMat, blueMat, 1 };
   drawUtils.drawSphere(22, 256, 256, materialDiffuse);

   glTranslatef(0, sin(3.14 / 4) * 23, cos(3.14 / 4) * 23);
   glRotatef(31, 1, 0, 0);

   glTranslatef(spiderPosition.x * 3, spiderPosition.y * 3, spiderPosition.z * 3);
   float angle = calculateAngle(Position(0, 0, 1), dSpiderOffset);
   glRotatef(180.0f - angle, 0, 1, 0);
   drawUtils.drawSpider();
   glRotatef(angle - 180.0f, 0, 1, 0);
   glTranslatef(-spiderPosition.x * 3, -spiderPosition.y * 3, -spiderPosition.z * 3);

   glScaled(3, 3, 3);
   drawUtils.drawFly(cursorPosition, animationOffset);
   drawUtils.drawSpiderWeb();
   mouseUtils.calculateRay();
   drawUtils.drawLandingSpots(mouseUtils);

   string text = "SCORE:";
   drawUtils.drawText(text.data(), text.size(), 50, 950);

   string points = to_string(score);
   drawUtils.drawText(points.data(), points.size(), 110, 950);

   if (gameOver) {
      glColor3f(1, 0, 0);
      string text = "GAME OVER";
      drawUtils.drawText(text.data(), text.size(), 910, 950);
   }


   glPopMatrix();

   glutSwapBuffers();
}