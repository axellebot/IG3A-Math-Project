/*********************************************************
 *                    main.cpp                           *
 *********************************************************/

#include <stdlib.h>
#include <fstream>
#include <iostream>

// Inclusion des fichiers d'en-tete Glut
#include <GL/glut.h>

#include "constants.h"
#include "Point.h"

using namespace std;

double Scal = 36;

double trX = 0.0, trY = 0.0, dist = 0.;//,trZ=0.0;

bool leftClicked;
int anglex, angley, x, y, xold, yold;

/* Prototype des fonctions pour OpenGL */
void idle();
/**
 * Display function
 */
void display();//  procedure a modifier en fonction de la scene

/**
 * Keyboard inputs triggered function
 * @param key
 * @param x
 * @param y
 */
void keyboardInput(unsigned char key, int x, int y);
/**
 * Special inputs (extra keyboard, ...) triggered function
 * @param key
 * @param x
 * @param y
 */
void special(int key, int x, int y);

/**
 * Reshape function
 * @param x
 * @param y
 */
void reshape(int x, int y);

/**
 * Mouse input triggered function
 * @param bouton
 * @param etat
 * @param x
 * @param y
 */
void mouseInput(int bouton, int etat, int x, int y);

/**
 * Mouse motion triggered function
 * @param x
 * @param y
 */
void mouseMotion(int x, int y);

/****************************************************************
 **                                                            **
 **  Procedure avec mise en file des sommets des primitives    **
 **                                                            **
 ****************************************************************/
void init();

int main(int argc, char **argv) {
    /* initialisation de glut et creation de la fenetre */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1000, 1000);
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
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardInput);
    glutSpecialFunc(special);
    glutMouseFunc(mouseInput);
    glutMotionFunc(mouseMotion);


    /* Entree dans la boucle principale glut */
    glutMainLoop();
    return 0;
}

/****************************************************************
 **                                                            **
 **                  OpenGL overloading                        **
 **                                                            **
 ****************************************************************/

// fonction permettant d'afficher les objets en utilisant des listes
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
    glCallList(1); // appel de la liste numero 1
    glCallList(2);   // appel de la liste numero 2
    glFlush();
    glutSwapBuffers(); //switch buffers
}

void keyboardInput(unsigned char key, int x, int y) {
    printf("Keyboard input : %d\n", key);
    switch (key) {
        case 'q' : /*la key 'q' permet de quitter le programme */
            exit(0);
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

void reshape(int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //taille de la scene
    double Ortho = -150;
    glOrtho(-Ortho, Ortho, -Ortho, Ortho, -Ortho, Ortho);// fenetre
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);
}

void mouseInput(int button, int state, int x, int y) {
    /* si on appuie sur le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        leftClicked = true; /* le booleen leftClicked passe a 1 (vrai) */
        xold = x; /* on sauvegarde la position de la souris */
        yold = y;
    }
    /* si on relache le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        leftClicked = false; /* le booleen leftClicked passe a 0 (faux) */
}

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
void tracePoint(Point p, double r0, double g0, double b0, double size) {
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
void traceSegment(Point p1, Point p2, double red, double green, double blue, double size) {
    glColor3f(red, green, blue);//initialisation de la couleur
    glLineWidth(size); // initialisation de la taille
    glBegin(GL_LINES); // on trace un segment
    glVertex2f(p1.x, p1.y); // coordonnees du premier point
    glVertex2f(p2.x, p2.y); // coordonnees du dernier point
    glEnd(); // fin de glBegin
}

/****************************************************************
 **                                                            **
 **               Initialisation de  la scene                  **
 **                                                            **
 ****************************************************************/
//fonction ou les objets sont a definir
void init() {
    cout << "\nStart Init" << endl;

    Point o, i, j;
    o.x = 0., o.y = 0., i.x = 1., i.y = 0., j.x = 0., j.y = 1.;

    glNewList(1, GL_COMPILE_AND_EXECUTE); //liste numero 1
    tracePoint(o, 0., 0., 1., 15.);//O
    tracePoint(i, 1., 0., 0., 10.); //I
    tracePoint(j, 0., 0.5, 0., 10.); //J
    glEndList();
    glNewList(2, GL_COMPILE_AND_EXECUTE);  //liste numero 2
    traceSegment(o, i, 1.0, 0.0, 1.0, 2.0); // on trace [OI]
    traceSegment(o, j, 1.0, 0.50, 0.0, 2.0);// on trace [OJ]
    glEndList();

    glNewList(4, GL_COMPILE_AND_EXECUTE);  //liste numero 4

    glEndList();

    glNewList(5, GL_COMPILE_AND_EXECUTE);  //liste numero 5

    glEndList();
    glNewList(6, GL_COMPILE_AND_EXECUTE); //liste numero 6

    glEndList();

    cout << "\nEnd Init" << endl;
}


