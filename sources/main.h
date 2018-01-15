//
// Created by axel on 21/11/17.
//

#ifndef MINIPROJETMATH_MAIN_H
#define MINIPROJETMATH_MAIN_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <GL/glut.h>// Inclusion des fichiers d'en-tete Glut
#include <spdlog/spdlog.h> // Logger

#include "Point.h"
#include "Algorithm.h"

/****************************************************************
 **                                                            **
 **                          Constant                          **
 **                                                            **
 ****************************************************************/
#define Pi 3.141592654

/****************************************************************
 **                                                            **
 **                       Configuration                        **
 **                                                            **
 ****************************************************************/
#define MMP_WINDOW_LABEL "Maths - Mini Project"
#define MMP_AUTHORS "Axel LE BOT | Andrew LENC"

//Level of logger
const spdlog::level::level_enum LOGGER_LEVEL = spdlog::level::info;

//LAYERS
#define MMP_LAYER_INDEX_LANDMARK_SEGMENT 1
#define MMP_LAYER_INDEX_LANDMARK_POINTS 2
#define MMP_LAYER_INDEX_DRAW_SEGMENT 3
#define MMP_LAYER_INDEX_DRAW_POINT 4
#define MMP_LAYER_INDEX_DRAW_TEXT 5

//Screen config
const int DEFAULT_SCREEN_WIDTH = 1000;
const int DEFAULT_SCREEN_HEIGHT = 1000;

//Size config
const float SIZE_LANDMARK_POINT = 10;
const float SIZE_POINT = 15;
const float SIZE_LANDMARK_SEGMENT = 2.0;
const float SIZE_SEGMENT = 4.0;
const float DEFAULT_SCALE = 30;
const float DEFAULT_DELETE_THRESHOLD = 0.5; //Thxreshold to delete point

//Moving Behaviors
const float GAP_MOVING = 0.25;
const float GAP_ZOOMING = 0.5;
const bool FREE_Z_ROTATION = false;

/****************************************************************
 **                                                            **
 **                            Datas                           **
 **                                                            **
 ****************************************************************/
//scale of UI
double scale = DEFAULT_SCALE;
//rotation and position on windows axes
int anglex, angley;
//mouse behaviors
bool leftClicked = false, middleClicked = false;
int mouseLeftXOld, mouseLeftYOld;
int mouseMiddleXOld, mouseMiddleYOld;
//tranlation on windows axes
double trX = 0.0, trY = 0.0, trZ = 0.0;
//current algorithm
Algorithm currentAlgo = MonotoneChain;
//Toggle point list display
bool togglePointDisplay = false;
//list of current points displayed
vector<Point> pointList;
vector<Point> hullPointList;

//Console logger(with color support)
auto logger = spdlog::stdout_logger_mt("logger", true);

/****************************************************************
 **                                                            **
 **                     Prototypes Drawing                     **
 **                                                            **
 ****************************************************************/
void drawPoint(Point p, double r0, double g0, double b0, double size);

void drawSegment(Point p1, Point p2, double red, double green, double blue, double size);

void drawTextOnPoint(Point p, string txt);

/****************************************************************
 **                                                            **
 **                  OpenGL overloading                        **
 **                                                            **
 ****************************************************************/
void idle();

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
void resetDisplay();

void resetLandmarkDisplay();

void resetPointsDisplay();

void resetHullSegmentsDisplay();

/****************************************************************
 **                                                            **
 **                     Prototypes Action                      **
 **                                                            **
 ****************************************************************/
void addPoint(Point p);

void addPoint(coord_t x, coord_t y);

void removePoint(coord_t x, coord_t y);

void reset();

void zoomIn();

void zoomOut();

void rotate(int x, int y);

Point convertPointLocation(double x, double y);

vector<Point> getHullPoints();

void deleteLastPoint();

void applyPointEdition();

void applyAlgorithmEdition();

void consoleInput();


/****************************************************************
 **                                                            **
 **                            Menu                            **
 **                                                            **
 ****************************************************************/
void menuMainTrigger(int value);

void menuActionTrigger(int value);

void menuDisplayTrigger(int value);

void menuAlgoTrigger(int value);

void initMenu();

/****************************************************************
 **                                                            **
 **                     Prototypes Main                        **
 **                                                            **
 ****************************************************************/
void initLogger();

void initWindow();

#endif //MINIPROJETMATH_MAIN_H
