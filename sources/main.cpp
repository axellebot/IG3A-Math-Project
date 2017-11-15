/*********************************************************
 *                    main.cpp                           *
 *********************************************************/

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <GL/glut.h>// Inclusion des fichiers d'en-tete Glut
#include "constants.h"
#include "Point.h"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

vector<Point> pointList;

double Scal = 36;
double trX = 0.0, trY = 0.0, dist = 0.;//,trZ=0.0;

bool leftClicked=false;
int anglex, angley, x, y, xold, yold;

void idle();

void addPoint(coord_t x, coord_t y);
void reset();

/****************************************************************
 **                                                            **
 **                           Tracage                          **
 **                                                            **
 ****************************************************************/

/**
 * Tracer un point
 * @param p     Point
 * @param r0    Color Red
 * @param g0    Color Green
 * @param b0    Color Blue
 * @param size  Size
 */
void drawPoint(Point p, double r0, double g0, double b0, double size) {
    glColor3f(r0, g0, b0);    //initialisation de la couleur
    glPointSize(size);    // initialisation de la taille
    glBegin(GL_POINTS);    // on trace un point
    glVertex2f(p.x, p.y);    // coordonnees du point
    glEnd();    // fin de glBegin
}

/**
 * Tracer un Segment
 * @param p1
 * @param p2
 * @param red
 * @param green
 * @param blue
 * @param size
 */
void drawSegment(Point p1, Point p2, double red, double green, double blue, double size) {
    glColor3f(red, green, blue);//initialisation de la couleur
    glLineWidth(size); // initialisation de la taille
    glBegin(GL_LINES); // on trace un segment
    glVertex2f(p1.x, p1.y); // coordonnees du premier point
    glVertex2f(p2.x, p2.y); // coordonnees du dernier point
    glEnd(); // fin de glBegin
}

/****************************************************************
 **                                                            **
 **                  OpenGL overloading                        **
 **                                                            **
 ****************************************************************/

/**
 * Display function
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear display with background color
    glLoadIdentity();

    glTranslatef(0.0, 0.0, dist);
    //Pour la 3D
    /*
    glRotatef(-angley,1.0,0.0,0.0);
    glRotatef(-anglex,0.0,1.0,0.0);
     */

    // Pour la 2D
    glRotatef(-anglex + angley, 0.0, 0.0, 1.0);
    glScalef(Scal, Scal, Scal); // diminution de la vue de la scene
    glRotatef(180, 0.0, 1.0, 0.0);
    glRotatef(180, 1.0, 0.0, 0.0);
    glTranslatef(-trX, trY, 0.);
    //Oredered by layers
    //TOP
    glCallList(MMP_LAYER_INDEX_DRAW_POINT);
    glCallList(MMP_LAYER_INDEX_DRAW_SEGMENT);
    glCallList(MMP_LAYER_INDEX_LANDMARK_POINTS);
    glCallList(MMP_LAYER_INDEX_LANDMARK_SEGMENT);
    //BOTTOM
    glFlush();
    glutSwapBuffers(); //switch buffers
}

/**
 * Keyboard inputs triggered function
 * @param key
 * @param x
 * @param y
 */
void keyboardInput(unsigned char key, int x, int y) {
    printf("Keyboard input : %d\n", key);
    switch (key) {
        case 'q' : /*la key 'q' permet de quitter le programme */
            exit(0);
        case 'r': //Reset
            reset();
            glutPostRedisplay();
            break;
        case '+' :
            dist += 0.5;
            Scal = Scal + 0.5;
            glutPostRedisplay();
            break;
        case '-' :
            dist -= 0.5;
            Scal = Scal - 0.5;
            glutPostRedisplay();
            break;
    }
}

/**
 * Special inputs (extra keyboard, ...) triggered function
 * @param key
 * @param x
 * @param y
 */
void special(int key, int x, int y) {
    printf("Special Keyboard input : %d\n", key);

    switch (key) {
        case GLUT_KEY_UP:
            trY -= 0.25;
            break;
        case GLUT_KEY_DOWN:
            trY += 0.25;
            break;
        case GLUT_KEY_LEFT:
            trX -= 0.25;
            break;
        case GLUT_KEY_RIGHT:
            trX += 0.25;
            break;
    }

    glutPostRedisplay();
}

/**
 * Reshape function
 * @param x
 * @param y
 */
void reshape(int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //taille de la scene
    double Ortho = -150;
    glOrtho(-Ortho, Ortho, -Ortho, Ortho, -Ortho, Ortho);// fenetre
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);
}

/**
 * Mouse input triggered function
 * @param bouton
 * @param etat
 * @param x
 * @param y
 */
void mouseInput(int button, int state, int x, int y) {
    cout<<"Mouse Click at "<<x<<';'<<y<<endl;
    /* si on appuie sur le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        leftClicked = true; /* le booleen leftClicked passe a 1 (vrai) */
        xold = x; /* on sauvegarde la position de la souris */
        yold = y;
        //TODO : Added real coordonates with window coordonate
//        addPoint(1, 1);
//        addPoint(2, 1);
    }
    /* si on relache le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        leftClicked = false; /* le booleen leftClicked passe a 0 (faux) */
}

/**
 * Mouse motion triggered function
 * @param x
 * @param y
 */
void mouseMotion(int x, int y) {
    if (leftClicked) /* si le bouton gauche est leftClicked */
    {
        /* on modifie les angles de rotation de l'objet
       en fonction de la position actuelle de la souris et de la derniere
       position sauvegardee */
        anglex = anglex + (x - xold);
        angley = angley + (y - yold);
        glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold = x; /* sauvegarde des valeurs courante de le position de la souris */
    yold = y;
}

/****************************************************************
 **                                                            **
 **                         Affichage                          **
 **                                                            **
 ****************************************************************/
void refreshLandmarkDisplay(){
    Point o, i, j;
    o.x = 0., o.y = 0., i.x = 1., i.y = 0., j.x = 0., j.y = 1.;

    glNewList(MMP_LAYER_INDEX_LANDMARK_POINTS, GL_COMPILE_AND_EXECUTE); //list number X
    drawPoint(o, 0., 0., 1., 15.);//O
    drawPoint(i, 1., 0., 0., 10.); //I
    drawPoint(j, 0., 0.5, 0., 10.); //J
    glEndList();

    glNewList(MMP_LAYER_INDEX_LANDMARK_SEGMENT, GL_COMPILE_AND_EXECUTE); //list number X
    drawSegment(o, i, 1.0, 0.0, 1.0, 2.0); // on trace [OI]
    drawSegment(o, j, 1.0, 0.50, 0.0, 2.0);// on trace [OJ]
    glEndList();
}

void refreshPointsDisplay(){
    glNewList(MMP_LAYER_INDEX_DRAW_POINT, GL_COMPILE_AND_EXECUTE); //List for points
    for(Point p : pointList){
        drawPoint(p,0,0,0,20);
    }
    glEndList();
    glutPostRedisplay();
}

void refreshSegmentsDisplay(){
    glNewList(MMP_LAYER_INDEX_DRAW_SEGMENT, GL_COMPILE_AND_EXECUTE); //List for segments
    glEndList();
}

//fonction ou les objets sont a definir
void initDisplay() {
    cout << "\nStart Init" << endl;

    refreshLandmarkDisplay();
    refreshPointsDisplay();
    refreshSegmentsDisplay();

    cout << "\nEnd Init" << endl;
}

/****************************************************************
 **                                                            **
 **                           Actions                          **
 **                                                            **
 ****************************************************************/
void addPoint(coord_t x,coord_t y){
    Point p; p.x=x;p.y=y;
    pointList.push_back(p);
    cout<<"Added new point at "<<p.x<<';'<<p.y<<endl;
    refreshPointsDisplay();
}

void reset(){
    cout << "\nStart Reset" << endl;

    pointList.clear(); //clear point list
    initDisplay();

    cout << "\nEnd Reset" << endl;
}

/****************************************************************
 **                                                            **
 **                            Main                            **
 **                                                            **
 ****************************************************************/

void init(){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow(MMP_WINDOW_LABEL);
    /* Initialisation d'OpenGL */
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(2.0);
    glEnable(GL_DEPTH_TEST);

    glColor3f(0.0, 0.0, 0.0);
    //glEdgeFlag(GL_FALSE);
    glEdgeFlag(GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    //glEnable(GL_LIGHTING);
    glDisable(GL_LIGHTING);

    /* Register OpenGL Function */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardInput);
    glutSpecialFunc(special);
    glutMouseFunc(mouseInput);
    glutMotionFunc(mouseMotion);
}

int main(int argc, char **argv) {
    /* initialisation de glut et creation de la fenetre */
    glutInit(&argc, argv);

    init();
    initDisplay();

    /* Entree dans la boucle principale glut */
    glutMainLoop();
    return 0;
}