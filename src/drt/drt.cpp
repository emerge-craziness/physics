//-lglut -lGL -lGLU -lgmpxx -lgmp

#include <GL/glut.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <mpfr.h>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;


//USED VARIABLES

int winw, 
    winh,
    size,
    argc,
    drawing_mode = 1;

long double pointX,
            pointX2,
            pointY,
            pointY2,
            vFullPrev,
            vFullNext,
            vyPrev,
            vyNext,
            dt,
            xMax[30],
            yMax[30],
            tMax[30],
            vyMax[30],
            dtMin[30],
            border = 40,
            yx_coefficient,
            xt_coefficient,
            vyt_coefficient,
            fulltime,
            null;

string filename[30], 
       headername[30];

long double maxX = 0,
            maxY = 0,
            maxT = 0,
            maxVy = 0;


//USED FUNCTIONS 

void reshape (int w, int h);
void y_x ();
void x_t ();
void vy_t ();
void keyboard (unsigned char key, int x, int y);
long double max_x ();
long double max_y ();
long double max_v ();
void renderBitmapString(double x, double y, void *font, string str, char);
void renderScene_switch();

int main (int argc, char ** argv) {

    if (argc == 1) {
        filename[0] = "data.txt";
        headername[0] = "data_header.txt";
        argc++;
    }
    else {
        for (int i = 0; i < argc - 1; i++) {
            filename[i] = argv[i+1] + (string)".txt";
            headername[i] = argv[i+1] + (string)"_header.txt";
      
            ifstream header;
            header.open (headername[i].c_str(), ios::in);
            header >> null >> null >> xMax[i] >> yMax[i] >> vyMax[i] >> tMax[i]; // format is: size, dt, xMax, yMax, vyMax, fulltime
            header.close ();
        }
    }

    ::argc = argc;

    max_x ();
    max_y ();
    max_v ();

    glutInit (&argc, argv);
    glutInitWindowSize (1280, 780);
    glutInitWindowPosition (1, 1);
    glutInitDisplayMode (GLUT_RGB);
    glutCreateWindow ("Physics");
    glutDisplayFunc (renderScene_switch);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glClearColor (255, 255, 255, 0);
    //glClearColor (0, 0, 0, 0);
    glutMainLoop ();

    return 0;
}

void renderScene_switch() {
    switch(drawing_mode) {
        case 1:
            y_x();
            break;
        case 2:
            x_t();
            break;
        case 3:
            vy_t();
            break;
    }       
}

void y_x () { 

    glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity ();

    string graph = "y(x):";
    glColor3f(0.5f, 0.5f, 0.5f);
    renderBitmapString(34, winh - 23, GLUT_BITMAP_HELVETICA_18, graph, '1');

    //GRID:

    double grid_coefficient = yx_coefficient;
    double i = 10;
    if (yx_coefficient > 50) {
        for (grid_coefficient = yx_coefficient; grid_coefficient > 50; grid_coefficient /= 10) {
            i /= 10;
        }
    }
    else if (yx_coefficient < 5) {
        for (grid_coefficient = yx_coefficient; grid_coefficient < 5; grid_coefficient *= 10) {
            i /= 0.1;
        }
    }
    const double i_beginning = i;
    int difference = i / i_beginning;
    if (i != i_beginning) {
        grid_coefficient /= (i / i_beginning);
        i = i_beginning;
    }
    ostringstream out;
    out << i / 10;

    //HORIZONTAL
    
    for (int g = 0; g * grid_coefficient * difference + border * 2 < winh; ++g) {
        if (g * difference % 10 == 0) {
            glLineWidth (2);
            glColor3f (0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"m";
            renderBitmapString(3, g * grid_coefficient + border, GLUT_BITMAP_HELVETICA_10, a, 'v');
        }
        else {
            glLineWidth (1);
            glColor3f (0.3f, 0.3f, 0.3f);
        }
        glBegin (GL_LINES);
            glVertex2f (border, g * grid_coefficient * difference + border);
            glVertex2f (winw - border, g * grid_coefficient * difference + border);
        glEnd ();
    }
    
    
    //VERTICAL

    for (int g = 0; g * grid_coefficient * difference + border * 2 < winw; ++g) {
        if (g * difference % 10 == 0) {
            glLineWidth (2);
            glColor3f (0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"m";
            renderBitmapString(g * grid_coefficient + border, 3, GLUT_BITMAP_HELVETICA_10, a, 'h');
        }
        else {
            glLineWidth (1);
            glColor3f (0.3f, 0.2f, 0.2f);
        }
        glBegin (GL_LINES);
            glVertex2f (grid_coefficient * g * difference + border, border);
            glVertex2f (grid_coefficient * g * difference + border, winh - border);
        glEnd ();
    }


    
    //GRAPHS:

    glTranslatef (border, border, 0);

    for(int i = 0; i < argc - 1; i++) {

        ifstream file, header;
        file.open (filename[i].c_str(), ios::in);
        header.open (headername[i].c_str(), ios::in);
        header >> size >> dt >> xMax[i] >> yMax[i];                 // format is: size, dt, xMax, yMax, fulltime
        
        glLineWidth (3);
        switch (i % 9) {
            case 0:
                glColor3ub (128, 128, 128); //yellow
                break;
            case 1:
                glColor3ub (255, 0, 0);
                break;
            case 2:
                glColor3ub (173, 255, 47); //greem yellow
                break;
            case 3:
                glLineWidth (3);
                glColor3ub (0, 0, 0); //black
                break;
            case 4:
                glColor3ub (91, 146, 229); //
                break;
            case 5:
                glColor3ub (0, 0, 255);
                break;
            case 6:
                glColor3ub (255, 215, 0); //orange
                break;
            case 7:
                glColor3ub (150, 170, 0);
                break;
            case 8:
                glColor3ub (170, 150, 40);
                break;
        }
        
        file >> pointX >> pointY >> null >> null >> null >> null >> null;
 
        for (int j = 0; j < size - 1; j++) {
            file >> pointX2 >> pointY2 >> null >> null >> null >> null >> null;
            
            glBegin (GL_LINES);
                glVertex2d (pointX * yx_coefficient, pointY * yx_coefficient);
                glVertex2d (pointX2 * yx_coefficient, pointY2 * yx_coefficient);
            glEnd ();
 
            pointX = pointX2;
            pointY = pointY2;
        }
 
        file.close ();
        header.close ();
 
    }

    glFlush ();
}

void x_t () {
    glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity ();
    
    string graph = "x(t):";
    glColor3f(0.5f, 0.5f, 0.5f);
    renderBitmapString(34, winh - 23, GLUT_BITMAP_HELVETICA_18, graph, '1');

    //GRID:

    double grid_coefficient = xt_coefficient;
    double i = 10;
    if (xt_coefficient > 50) {
        for (grid_coefficient = xt_coefficient; grid_coefficient > 50; grid_coefficient /= 10) {
            i /= 10;
        }
    }
    else if (xt_coefficient < 5) {
        for (grid_coefficient = xt_coefficient; grid_coefficient < 5; grid_coefficient *= 10) {
            i /= 0.1;
        }
    }
    const double i_beginning = i;
    int difference = i / i_beginning;
    if (i != i_beginning) {
        grid_coefficient /= (i / i_beginning);
        i = i_beginning;
    }
    ostringstream out;
    out << i / 10;

    //HORIZONTAL
    
    for (int g = 0; g * grid_coefficient * difference + border * 2 < winh; ++g) {
        if (g * difference % 10 == 0) {
            glLineWidth (2);
            glColor3f (0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"m";
            renderBitmapString(3, g * grid_coefficient + border, GLUT_BITMAP_HELVETICA_10, a, 'v');
        }
        else {
            glLineWidth (1);
            glColor3f (0.3f, 0.3f, 0.3f);
        }
        glBegin (GL_LINES);
            glVertex2f (border, g * grid_coefficient * difference + border);
            glVertex2f (winw - border, g * grid_coefficient * difference + border);
        glEnd ();
    }
    
    
    //VERTICAL

    for (int g = 0; g * grid_coefficient * difference + border * 2 < winw; ++g) {
        if (g * difference % 10 == 0) {
            glLineWidth (2);
            glColor3f (0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"s";
            renderBitmapString(g * grid_coefficient + border, 3, GLUT_BITMAP_HELVETICA_10, a, 'h');
        }
        else {
            glLineWidth (1);
            glColor3f (0.3f, 0.2f, 0.2f);
        }
        glBegin (GL_LINES);
            glVertex2f (grid_coefficient * g * difference + border, border);
            glVertex2f (grid_coefficient * g * difference + border, winh - border);
        glEnd ();
    }


    
    //GRAPHS:

    glTranslatef (border, border, 0);

    for(int i = 0; i < argc - 1; i++) {

        ifstream file, header;
        file.open (filename[i].c_str(), ios::in);
        header.open (headername[i].c_str(), ios::in);
        header >> size >> dt >> xMax[i] >> yMax[i];                 // format is: size, dt, xMax, yMax, fulltime
        
        glLineWidth (3);
        switch (i % 9) {
            case 0:
                glColor3ub (173, 255, 47); //greem yellow
                break;
            case 1:
                glColor3ub (255, 0, 0);
                break;
            case 2:
                glColor3ub (0, 0, 0); //black
                break;
            case 3:
                glColor3ub (128, 128, 128); //grey
                break;
            case 4:
                glColor3ub (91, 146, 229); //
                break;
            case 5:
                glColor3ub (0, 0, 255);
                break;
            case 6:
                glColor3ub (255, 215, 0); //orange
                break;
            case 7:
                glColor3ub (150, 170, 0);
                break;
            case 8:
                glColor3ub (170, 150, 40);
                break;
        }
        
        file >> pointX >> null >> null >> null >> null >> null >> null;
 
        for (int j = 0; j < size - 1; j++) {
            file >> pointX2 >> null >> null >> null >> null >> null >> null;
            
            glBegin (GL_LINES);
                glVertex2d (dt * (double)j * xt_coefficient, pointX * xt_coefficient);
                glVertex2d (dt * (double)(j + 1) * xt_coefficient, pointX2 * xt_coefficient);
            glEnd ();
 
            pointX = pointX2;
        }
 
        file.close ();
        header.close ();
 
    }

    glFlush ();
    
};

void vy_t () {
    glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity ();
    
    string graph = "vy(t):";
    glColor3f(0.5f, 0.5f, 0.5f);
    renderBitmapString(34, winh - 23, GLUT_BITMAP_HELVETICA_18, graph, '1');

    //GRID:

    double grid_coefficient = vyt_coefficient;
    double i = 10;
    if (vyt_coefficient > 50) {
        for (grid_coefficient = vyt_coefficient; grid_coefficient > 50; grid_coefficient /= 10) {
            i /= 10;
        }
    }
    else if (vyt_coefficient < 5) {
        for (grid_coefficient = vyt_coefficient; grid_coefficient < 5; grid_coefficient *= 10) {
            i /= 0.1;
        }
    }
    const double i_beginning = i;
    int difference = i / i_beginning;
    if (i != i_beginning) {
        grid_coefficient /= (i / i_beginning);
        i = i_beginning;
    }
    ostringstream out;
    out << i / 10;

    //HORIZONTAL
    
    for (int g = 0; g * grid_coefficient * difference + border * 2 < winh; ++g) {
        if (g * difference % 10 == 0) {
            glLineWidth (2);
            glColor3f (0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"m/s";
            renderBitmapString(3, g * grid_coefficient + border, GLUT_BITMAP_HELVETICA_10, a, 'v');
        }
        else {
            glLineWidth (1);
            glColor3f (0.3f, 0.3f, 0.3f);
        }
        glBegin (GL_LINES);
            glVertex2f (border, g * grid_coefficient * difference + border);
            glVertex2f (winw - border, g * grid_coefficient * difference + border);
        glEnd ();
    }
    
    
    //VERTICAL

    for (int g = 0; g * grid_coefficient * difference + border * 2 < winw; ++g) {
        if (g * difference % 10 == 0) {
            glLineWidth (2);
            glColor3f (0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"s";
            renderBitmapString(g * grid_coefficient + border, 3, GLUT_BITMAP_HELVETICA_10, a, 'h');
        }
        else {
            glLineWidth (1);
            glColor3f (0.3f, 0.2f, 0.2f);
        }
        glBegin (GL_LINES);
            glVertex2f (grid_coefficient * g * difference + border, border);
            glVertex2f (grid_coefficient * g * difference + border, winh - border);
        glEnd ();
    }


    
    //GRAPHS:

    glTranslatef (border, border, 0);

    for(int i = 0; i < argc - 1; i++) {

        ifstream file, header;
        file.open (filename[i].c_str(), ios::in);
        header.open (headername[i].c_str(), ios::in);
        header >> size >> dt >> xMax[i] >> yMax[i] >> vyMax[i];                 // format is: size, dt, xMax, yMax, fulltime
        
        switch (i % 9) {
            case 0:
                glColor3ub (0, 0, 0); //black
                break;
            case 1:
                glColor3ub (255, 0, 0);
                break;
            case 2:
                glColor3ub (128, 128, 128); //yellow
                break;
            case 3:
                glColor3ub (173, 255, 47); //greem yellow
                break;
            case 4:
                glColor3ub (91, 146, 229); //
                break;
            case 5:
                glColor3ub (0, 0, 255);
                break;
            case 6:
                glColor3ub (255, 215, 0); //orange
                break;
            case 7:
                glColor3ub (150, 170, 0);
                break;
            case 8:
                glColor3ub (170, 150, 40);
                break;
        }
        glLineWidth (3);
        
        file >> null >> null >> null >> null >> null >> vyPrev >> null;
 
        for (int j = 0; j < size - 1; j++) {
            file >> null >> null >> null >> null >> null >> vyNext >> null;
            
            glBegin (GL_LINES);
                glVertex2d (dt * (double)j * vyt_coefficient, vyPrev * vyt_coefficient);
                glVertex2d (dt * (double)(j + 1) * vyt_coefficient, vyNext * vyt_coefficient);
            glEnd ();
 
            vyPrev = vyNext;
        }
 
        file.close ();
        header.close ();
 
    }

    glFlush ();
    
};


void keyboard (unsigned char key, int x, int y) {
    switch (key) {
        case 1: // Escape key
        case 16: // Escape key
        case 27: // Escape key
        case 'q':
            exit (0);
            //return; 
            break;
        case '1':
            drawing_mode = 1; //y(x)
            glutPostRedisplay();
            break;
        case '2':
            drawing_mode = 2; //x(t)
            glutPostRedisplay();
            break;
        case '3':
            //drawing_mode = 3; //v(x)
            glutPostRedisplay();
            break;
    }
}

void reshape (int w, int h) {
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (0, w, 0, h);      
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    long double byy = (h - border * 2.7) / maxY;
    long double byx = (w - border * 2.7) / maxX;
    if (byy > byx) yx_coefficient = byx;
    else yx_coefficient = byy;

    byy = (h - border * 2.7) / maxX;
    byx = (w - border * 2.7) / maxT;
    if (byy > byx) xt_coefficient = byx;
    else xt_coefficient = byy;

    byy = (h - border * 2.7) / maxVy;
    byx = (w - border * 2.7) / maxT;
    if (byy > byx) vyt_coefficient = byx;
    else vyt_coefficient = byy;

    winw = w;
    winh = h;
}

long double max_x () {
    for (int i = 0; i < argc - 1; i++) {
        if (maxX < xMax[i]) {
            maxX = xMax[i];
        }
    }
}

long double max_y () {
    for (int i = 0; i < argc - 1; i++) {
        if (maxY < yMax[i]) {
            maxY = yMax[i];
        }
    }
}

long double max_t () {
    for (int i = 0; i < argc - 1; i++) {
        if (maxT < tMax[i]) {
            maxT = tMax[i];
        }
    }
}

long double max_v () {
    for (int i = 0; i < argc - 1; i++) {
        if (maxVy < vyMax[i]) {
            maxVy = vyMax[i];
        }
    }
}

void renderBitmapString(double x, double y, void *font, string str, char) {
    glRasterPos2d(x, y);
    for(int i = 0; i < str.length(); ++i) {
        glutBitmapCharacter(font, str[i]);
    }
}
