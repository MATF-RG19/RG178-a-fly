#pragma once
#include "Spider.h"
#include "Fly.h"
#include "SpiderWeb.h"
#include "MouseUtils.h"

class DrawUtils {
private:
   SpiderWeb* spiderWeb;
   Spider* spider;
   Fly* fly;

   int landingPosition;

public:
   DrawUtils();
   DrawUtils(SpiderWeb* spiderWeb, Spider* spider, Fly* fly);
   SpiderWeb getSpiderWeb();
   Spider getSpider();
   Fly getFly();
   int getLandingPosition();
   void drawSpiderWeb();
   void drawModel(Mesh* model, GLfloat material[4]);
   void drawSpider();
   void drawFly(int cursorPosition, Position animationOffset);
   void drawSphere(float size, int slices, int stacks, GLfloat materialDiffuse[4]);
   void drawLandingSpots(MouseUtils mouseUtils);
   void drawText(const char* text, int length, int x, int y);
};

