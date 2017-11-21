//
// Created by axel on 21/11/17.
//

#ifndef MINIPROJETMATH_MAIN_H
#define MINIPROJETMATH_MAIN_H

#include "constants.h"
#include "Point.h"
#include "Algorithm.h"

/****************************************************************
 **                                                            **
 **                          Constant                          **
 **                                                            **
 ****************************************************************/
#define Pi 3.141592654

#define MMP_WINDOW_LABEL "Maths - Mini Project"
#define MMP_AUTHORS "Axel LE BOT | Andrew LENC"

#define MMP_LAYER_INDEX_LANDMARK_SEGMENT 1
#define MMP_LAYER_INDEX_LANDMARK_POINTS 2
#define MMP_LAYER_INDEX_DRAW_SEGMENT 3
#define MMP_LAYER_INDEX_DRAW_POINT 4

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

double scale = 30;
int anglex, angley, x, y, xold, yold;
double trX = 0.0, trY = 0.0, trZ = 0.0;

Algorithm currentAlgo;

vector<Point> pointList;

bool leftClicked = false;

/****************************************************************
 **                                                            **
 **                     Prototypes Drawing                     **
 **                                                            **
 ****************************************************************/
void drawPoint(Point p, double r0, double g0, double b0, double size);
void drawSegment(Point p1, Point p2, double red, double green, double blue, double size);

/****************************************************************
 **                                                            **
 **                  OpenGL overloading                        **
 **                                                            **
 ****************************************************************/
void display();
void keyboardInput(unsigned char key, int x, int y);
void special(int key, int x, int y);
void reshape(int width, int height);
void mouseInput(int button, int state, int x, int y);
void mouseMotion(int x, int y);

/****************************************************************
 **                                                            **
 **                    Prototypes Display                      **
 **                                                            **
 ****************************************************************/
void refreshLandmarkDisplay();
void refreshPointsDisplay();
void refreshSegmentsDisplay();

/****************************************************************
 **                                                            **
 **                     Prototypes Action                      **
 **                                                            **
 ****************************************************************/
void addPointsTest();
void addPoint(coord_t x, coord_t y);
int chooseAlgorithm();
void reset();
void zoomIn();
void zoomOut();


/****************************************************************
 **                                                            **
 **                     Prototypes Main                        **
 **                                                            **
 ****************************************************************/
void initDisplay();
void init();

#endif //MINIPROJETMATH_MAIN_H
