//
// Created by axel on 25/10/17.
//


#include "main.h"
#include "tools.cpp"

using namespace std;

/**
 * Get Width
 * @return width (in px)
 */
int getWidth() {
    return glutGet(GLUT_WINDOW_WIDTH);
}

/**
 * Get Height
 * @return height (in px)
 */
int getHeight() {
    return glutGet(GLUT_WINDOW_HEIGHT);
}

/****************************************************************
 **                                                            **
 **                           Drawing                          **
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
    const string TAG = "DRAW_POINT";
    logger->debug("{0}", TAG);

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
    const string TAG = "DRAW_SEGMENT";
    logger->debug("{0}", TAG);

    glColor3f(red, green, blue);//initialisation de la couleur
    glLineWidth(size); // initialisation de la taille
    glBegin(GL_LINES); // on trace un segment
    glVertex2f(p1.x, p1.y); // coordonnees du premier point
    glVertex2f(p2.x, p2.y); // coordonnees du dernier point
    glEnd(); // fin de glBegin
}

/**
 *
 * @param point
 * @param txt
 */
void drawTextOnPoint(Point p, string txt) {
    const string TAG = "DRAW_TXT_ON_POINT";
    logger->info("{0} on point : {1}", TAG, txt);
    glColor3f(250.0, 0.0, 0.0);
    glRasterPos2i(p.x, p.y);
    for (int i = 0; i < txt.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, txt[i]);
    }
}

/****************************************************************
 **                                                            **
 **                  OpenGL overloading                        **
 **                                                            **
 ****************************************************************/
void idle() {
    glutPostRedisplay();
}


/**
 * Display function
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear display with background color
    glLoadIdentity();

    glTranslatef(trX, trY, trZ);//translate from window origin (top up corner)
    glRotatef(-anglex + angley, 0.0, 0.0, 1.0); // rotate Z axe
    glScalef(scale, scale, scale); // change scale

    //LAYERS
    //top
    glCallList(MMP_LAYER_INDEX_DRAW_TEXT);
    glCallList(MMP_LAYER_INDEX_DRAW_POINT);
    glCallList(MMP_LAYER_INDEX_DRAW_SEGMENT);
    glCallList(MMP_LAYER_INDEX_LANDMARK_POINTS);
    glCallList(MMP_LAYER_INDEX_LANDMARK_SEGMENT);
    //bottom

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
    const string TAG = "KEYBOARD_INPUT";
    logger->debug("{0}: key->{1}, x->{2}, y->{3}", TAG, key, x, y);

    switch (key) {
        case 'q' : /*la key 'q' permet de quitter le programme */
            exit(0);
            break;
        case 'r': //Reset
            reset();
            break;
        case '+' :
            zoomIn();
            break;
        case '-':
            zoomOut();
            break;
        case 'd':
            deleteLastPoint();
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
    const string TAG = "SPECIAL_INPUT";
    logger->debug("{0}: key->{1}, x->{2}, y->{3}", TAG, key, x, y);

    switch (key) {
        case GLUT_KEY_UP:
            trY -= scale * GAP_MOVING;
            break;
        case GLUT_KEY_DOWN:
            trY += scale * GAP_MOVING;
            break;
        case GLUT_KEY_RIGHT:
            trX -= scale * GAP_MOVING;
            break;
        case GLUT_KEY_LEFT:
            trX += scale * GAP_MOVING;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

/**
 * Reshape function on window resized
 * @param width width
 * @param height height
 */
void reshape(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //taille de la scene
    // help here -> http://www3.ntu.edu.sg/home/ehchua/programming/opengl/cg_introduction.html
    double orthoWidth = getWidth() / 2;
    double orthoHeight = getHeight() / 2;
    glOrtho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, 1, -1);// fenetre
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, getWidth(), getHeight()); //set the viewport
}

/**
 * Mouse input triggered function
 * @param bouton
 * @param etat
 * @param x
 * @param y
 */
void mouseInput(int button, int state, int x, int y) {
    const string TAG = "MOUSE_INPUT";
    logger->debug("{0}: button->{1}, state->{2}, x->{3},y->{4}", TAG, button, state, x, y);
    /* si on appuie sur le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        leftClicked = true;
        mouseLeftXOld = x; /* saved right click position */
        mouseLeftYOld = y;

        Point p = convertPointLocation(x, y);
        addPoint(p);
    }
    /* si on appuie sur le bouton droit */
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        middleClicked = true;
        mouseMiddleXOld = x; /* saved right click position */
        mouseMiddleYOld = y;

        Point pToDelete = convertPointLocation(x, y);
        removePoint(pToDelete.x, pToDelete.y);
    }

    if (button == 3 && state == GLUT_DOWN) {
        zoomOut();
    }
    if (button == 4 && state == GLUT_DOWN) {
        zoomIn();
    }

    /* si on relache le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        leftClicked = false;
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
        middleClicked = false;
}

/**
 * Mouse motion triggered function
 * @param x
 * @param y
 */
void mouseMotion(int x, int y) {
    if (leftClicked) /* si le bouton gauche est leftClicked */
    {
        rotate(x, y);
    }
    mouseLeftXOld = x; /* sauvegarde des valeurs courante de le position de la souris */
    mouseLeftYOld = y;
}

/****************************************************************
 **                                                            **
 **                         Display                            **
 **                                                            **
 ****************************************************************/
void resetDisplay() {
    const string TAG = "RESET_DISPLAY";
    logger->info("{0}: START", TAG);

    resetLandmarkDisplay();
    resetPointsDisplay();
    resetHullSegmentsDisplay();

    logger->info("{0}: END", TAG);
}

void resetLandmarkDisplay() {
    Point o, i, j;
    o.x = 0., o.y = 0., i.x = 1., i.y = 0., j.x = 0., j.y = 1.;

    glNewList(MMP_LAYER_INDEX_LANDMARK_POINTS, GL_COMPILE_AND_EXECUTE); //list number X
    drawPoint(j, 0., 0.5, 0., SIZE_LANDMARK_POINT); //J
    drawPoint(i, 1., 0., 0., SIZE_LANDMARK_POINT); //I
    drawPoint(o, 0., 0., 1., SIZE_LANDMARK_POINT);//O
    glEndList();

    glNewList(MMP_LAYER_INDEX_LANDMARK_SEGMENT, GL_COMPILE_AND_EXECUTE); //list number X
    drawSegment(o, i, 1.0, 0.0, 1.0, SIZE_LANDMARK_SEGMENT); // on trace [OI]
    drawSegment(o, j, 1.0, 0.50, 0.0, SIZE_LANDMARK_SEGMENT);// on trace [OJ]
    glEndList();
}

void resetPointsDisplay() {
    vector<Point> displayedPoint = (togglePointDisplay) ? pointList : hullPointList;

    glNewList(MMP_LAYER_INDEX_DRAW_TEXT, GL_COMPILE_AND_EXECUTE); //List for points text
    for (Point p : displayedPoint) {
        drawTextOnPoint(p, "(" + to_string_with_precision(p.x, 2) + ";" + to_string_with_precision(p.y, 2) + ")");
    }
    glEndList();

    glNewList(MMP_LAYER_INDEX_DRAW_POINT, GL_COMPILE_AND_EXECUTE); //List for points
    for (Point p : displayedPoint) {
        drawPoint(p, 0, 0, 0, SIZE_POINT);
    }
    glEndList();
    glutPostRedisplay();
}

void resetHullSegmentsDisplay() {
    glNewList(MMP_LAYER_INDEX_DRAW_SEGMENT, GL_COMPILE_AND_EXECUTE); //List for segments

    //check if list is filled
    if (hullPointList.size() > 0) {
        for (int i = 0; i < hullPointList.size() - 1; i++) {
            drawSegment(hullPointList.at(i), hullPointList.at(i + 1), 0, 0, 0, SIZE_SEGMENT);
        }
        drawSegment(hullPointList.front(), hullPointList.back(), 0, 0, 0, SIZE_SEGMENT);
    }
    glEndList();
}

/****************************************************************n
 **                                                            **
 **                           Actions                          **
 **                                                            **
 ****************************************************************/
void addPoint(Point p) {
    const string TAG = "ADD_POINT";

    pointList.push_back(p);
    logger->info("{0}: Point added : x->{1}, y->{2}", TAG, p.x, p.y);
    applyPointEdition();
}

void addPoint(coord_t x, coord_t y) {
    const string TAG = "ADD_POINT";

    Point p;
    p.x = x;
    p.y = y;

    pointList.push_back(p);
    logger->info("{0}: Point added : x->{1}, y->{2}", TAG, p.x, p.y);
    applyPointEdition();
}

void removePoint(coord_t x, coord_t y) {
    const string TAG = "REMOVE_POINT";
    logger->info("{0}: Remove point at {1};{2} ", TAG, x, y);

    cout << "Removed point at : " << x << " and " << y << endl;
    float threshold = DEFAULT_DELETE_THRESHOLD;
    for (int i = 0; i < pointList.size(); i++) {
        if ((x - threshold < pointList[i].x && pointList[i].x < x + threshold)
            && (y - threshold < pointList[i].y && pointList[i].y < y + threshold)) {
            pointList.erase(pointList.begin() + i);
        }
    }
    applyPointEdition();
}

void reset() {
    string TAG = "RESET";
    logger->info("{0}: Start", TAG);

    pointList.clear(); //clear point list
    hullPointList.clear(); //clear hull point list
    trX = trY = 0;//reset position
    scale = DEFAULT_SCALE;//reset scale
    glutReshapeWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

    resetDisplay();

    glutPostRedisplay();

    logger->info("{0}: End", TAG);
}

void zoomIn() {
    string TAG = "ZOOM_IN";
    scale += GAP_ZOOMING;
    logger->info("{0}: Zoom Int", TAG, scale);
    glutPostRedisplay();
}

void zoomOut() {
    if (scale - GAP_ZOOMING > 1) {
        string TAG = "ZOOM_OUT";
        scale -= GAP_ZOOMING;
        logger->info("{0}: Zoom Int", TAG, scale);
        glutPostRedisplay();
    }
}

void rotate(int x, int y) {
    if (FREE_Z_ROTATION) {
        /* on modifie les angles de rotation de l'objet
       en fonction de la position actuelle de la souris et de la derniere
       position sauvegardee */
        anglex += (x - mouseLeftXOld);
        angley += (y - mouseLeftYOld);
        glutPostRedisplay();
    }
}

Point convertPointLocation(double x, double y) {
    const string TAG = "CONVERT_POINT_LOCATION";
    double oldX = x, oldY = y;

    x = x - getWidth() / 2 - trX;
    y = y - getHeight() / 2 + trY;

    x /= (scale);
    y /= (scale);

    y = -y; //the y axe is displayed upside-down

    Point p;
    p.x = x;
    p.y = y;

    logger->debug("{0}: Convert location x:{1}->{2}, y:{3}->{4}", TAG, oldX, p.x, oldY, p.y);

    return p;
}

vector<Point> getHullPoints() {
    const string TAG = "GET_HULL_POINTS";
    vector<Point> hullPointList;

    if (pointList.size() > 0) {
        auto start = std::chrono::system_clock::now();

        switch (currentAlgo) {
            case MonotoneChain:
                hullPointList = Algorithms::MonotoneChain::convexHull(pointList);
                break;
            default :
                hullPointList = {};
        }

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        logger->info("{0}:counter {1} secondes", TAG, elapsed_seconds.count());
    } else {
        logger->warn("{0} No point to use", TAG);
    }


    return hullPointList;
}

void deleteLastPoint() {
    const string TAG = "GET_HULL_POINTS";
    logger->info("{0}:Delete last point", TAG);

    if (pointList.size() > 0) pointList.pop_back();

    applyPointEdition();
}

void applyPointEdition() {
    hullPointList = getHullPoints();
    resetPointsDisplay();
    resetHullSegmentsDisplay();
}

void applyAlgorithmEdition() {
    hullPointList = getHullPoints();
    resetHullSegmentsDisplay();
    glutPostRedisplay();
}

void consoleInput() {
    int count = 0;
    cout << "Console Input : \n";
    cout << "How many points you wanna add ?\n";
    cin >> count;
    for (int i = 0; i < count; i++) {
        printf("Point number %d\n", i + 1);
        double x, y;
        printf("%d -> X : ", i + 1);
        cin >> x;
        printf("%d -> Y : ", i + 1);
        cin >> y;
        addPoint(x, y);
    }
}

/****************************************************************
 **                                                            **
 **                            Menu                            **
 **                                                            **
 ****************************************************************/


void menuMainTrigger(int value) {
    logger->info("Main Menu Triggered :", value);
}

void menuActionTrigger(int value) {
    logger->info("Action Menu Triggered : {0}", value);
    switch (value) {
        case 0:
            consoleInput();
            break;
    }
}

void menuDisplayTrigger(int value) {
    logger->info("Display Menu Triggered : {0}", value);
    switch (value) {
        case 0:
            togglePointDisplay = !togglePointDisplay;
            resetPointsDisplay();
            break;
    }
}

void menuAlgoTrigger(int value) {
    logger->info("Algo Menu Triggered : {0}", value);
    currentAlgo = (Algorithm) value;
    applyAlgorithmEdition();
}


void initMenu() {
    // Action Sub Menu
    int actionMenu = glutCreateMenu(menuActionTrigger);
    glutAddMenuEntry("Add points through console", 0);

    // Display Sub Menu
    int displayMenu = glutCreateMenu(menuDisplayTrigger);
    glutAddMenuEntry("Toggle display", 0);

    // Algo Sub Menu
    int algoMenu = glutCreateMenu(menuAlgoTrigger);
    glutAddMenuEntry("Monotone Chain", MonotoneChain);

    // Create an entry
    glutCreateMenu(menuMainTrigger);
    glutAddSubMenu("Action", actionMenu);
    glutAddSubMenu("Display", displayMenu);
    glutAddSubMenu("Algorithms", algoMenu);

    // Let the menu respond on the right mouse button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/****************************************************************
 **                                                            **
 **                            Main                            **
 **                                                            **
 ****************************************************************/
void initLogger() {
    logger->set_level(LOGGER_LEVEL); // Set specific logger's log level
}

void initWindow() {
    //chooseAlgorithm();

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    glutCreateWindow(MMP_WINDOW_LABEL);

    initMenu();

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
    glutIdleFunc(idle);
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

    initLogger();// init logger
    initWindow();//init window

    resetDisplay();

    /* Entree dans la boucle principale glut */
    glutMainLoop();

    return 0;
}


