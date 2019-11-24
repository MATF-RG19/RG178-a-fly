#include<GL/glut.h>
#include <cmath>



/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

int main(int argc, char** argv) {
   /* Ambijentalna boja svetla. */
   GLclampf redA = 138.0 / 255;
   GLclampf greenA = 226.0 / 255;
   GLclampf blueA = 255.0 / 255;
   GLfloat light_ambient[] = { redA, greenA, blueA, 1 };

   /* Difuzna boja svetla. */
   GLfloat light_diffuse[] = { 1, 1, 1, 1 };

   /* Spekularna boja svetla. */
   GLfloat light_specular[] = { 1, 1, 1, 1 };

   /* Ambijentalno osvetljenje scene. */
   GLfloat model_ambient[] = { 0.2, 0.2, 0.2, 1 };

   /* Inicijalizuje se GLUT. */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

   /* Kreira se prozor. */
   glutInitWindowSize(1900, 1000);
   glutInitWindowPosition(10, 10);
   glutCreateWindow(argv[0]);

   /* Registruju se callback funkcije. */
   glutKeyboardFunc(on_keyboard);
   glutReshapeFunc(on_reshape);
   glutDisplayFunc(on_display);

   /* Obavlja se OpenGL inicijalizacija. */
   GLclampf red = 138.0 / 255;
   GLclampf green = 226.0 / 255;
   GLclampf blue = 255.0 / 255;
   glClearColor(0, 0, 0, 0);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   glEnable(GL_LIGHTING);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

   /* Program ulazi u glavnu petlju. */
   glutMainLoop();

   return 0;
}

static void on_keyboard(unsigned char key, int x, int y) {
   switch (key) {
   case 27:
      /* Zavrsava se program. */
      exit(0);
      break;
   }
}

static void on_reshape(int width, int height) {
   /* Podesava se viewport. */
   glViewport(0, 0, width, height);

   /* Podesava se projekcija. */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, (float)width / height, 1, 100);
}

static void on_display(void) {
   /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
   GLfloat light_position[] = { 0, 24, 24, 1 };
   GLfloat light_direction[] = { 0,-1,-1 };
   GLfloat light_cutoff = 45;
   GLfloat light_exponent[] = { 8 };

   /* Nulti koeficijenti refleksije materijala. */
   GLfloat no_material[] = { 0, 0, 0, 1 };

   /* Koeficijenti ambijentalne refleksije materijala. */
   GLfloat material_ambient[] = { 0.2, 0.2, 0.2, 1 };

   /* Koeficijenti ambijentalne refleksije materijala za slucaj
      nehomogene ambijentalne refleksije. */
   GLfloat material_ambient_heterogeneous[] = { 0.8, 0.8, 0.2, 1 };

   /* Koeficijenti difuzne refleksije materijala. */
   GLclampf redMat = 60.0 / 255;
   GLclampf greenMat = 210.0 / 255;
   GLclampf blueMat = 80.0 / 255;
   GLfloat material_diffuse[] = { redMat, greenMat, blueMat, 1 };

   /* Koeficijenti spekularne refleksije materijala. */
   GLfloat material_specular[] = { 1, 1, 1, 1 };

   /* Koeficijent spekularne refleksije za slucaj kada nema spekularne refleksije. */
   GLfloat no_shininess[] = { 0 };

   /* Koeficijent spekularne refleksije za slucaj male vrednosti koeficijenta. */
   GLfloat low_shininess[] = { 5 };

   /* Koeficijent spekularne refleksije za slucaj velike vrednosti koeficijenta. */
   GLfloat high_shininess[] = { 100 };

   /* Emisiona boja materijala. */
   GLfloat material_emission[] = { redMat/20, greenMat/20, blueMat/20, 0 };

   /* Brise se prethodni sadrzaj prozora. */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Podesava se vidna tacka. */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0, 16, 40, 0, 16, 0, 0, 1, 0);

   /* Pozicionira se svetlo. */
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
   glutSolidIcosahedron();
   glPopMatrix();


   /* Nova slika se salje na ekran. */
   glutSwapBuffers();
}