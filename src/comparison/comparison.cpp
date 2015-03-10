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
    argc;

long double pointX,
            pointX2,
            pointY,
            pointY2,
            dt,
            xMax[30],
            yMax[30],
            border = 40,
            coefficient,
            fulltime,
            null;

string filename[30], 
       headername[30];

long double maxX = 0, maxXnum = 0,
            maxY = 0, maxYnum = 0;


//USED FUNCTIONS 

void reshape (int w, int h);
void graphs ();
void keyboard (unsigned char key, int x, int y);
long double max_x (long double a[], int len);
long double max_y (long double a[], int len);

int main (int argc, char ** argv) {

    ::argc = argc;
    
    for (int i = 0; i < argc - 1; i++) {
        filename[i] = argv[i+1] + (string)".txt";
        headername[i] = argv[i+1] + (string)"_header.txt";

        ifstream header;
        header.open (headername[i].c_str(), ios::in);
        header >> size >> dt >> xMax[i] >> yMax[i] >> fulltime; // format is: size, dt, xMax, yMax, fulltime
        header.close ();
    }

    int len = sizeof (xMax) / sizeof (xMax[0]);
    max_x (xMax, len);
    max_y (yMax, len);

    glutInit (&argc, argv);
    glutInitWindowSize (1280, 780);
    glutInitWindowPosition (1, 1);
    glutInitDisplayMode (GLUT_RGB);
    glutCreateWindow ("Physics");
    glutDisplayFunc (graphs);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glClearColor (255, 255, 255, 0);
    glutMainLoop ();

    return 0;
}


void graphs () { 

    glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity ();

    //GRID:

    double grid_coefficient = coefficient;
    double i = 10;
    if (coefficient > 50) {
        for (grid_coefficient = coefficient; grid_coefficient > 50; grid_coefficient /= 10) {
            i /= 10;
        }
    }
    else if (coefficient < 5) {
        for (grid_coefficient = coefficient; grid_coefficient < 5; grid_coefficient *= 10) {
            i /= 0.1;
        }
    }
    const double i_beginning = i;
    int difference = i / i_beginning;
    if (i != i_beginning) {
        grid_coefficient /= (i / i_beginning);
        i = i_beginning;
    }

    //HORIZONTAL
    
    for (int g = 0; g * grid_coefficient * difference + border * 2 < winh; ++g) {
        if (g * difference % 10 == 0) {
            glLineWidth (2);
            glColor3f (0.5f, 0.5f, 0.5f);
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
        header >> size >> dt >> xMax[i] >> yMax[i]; // format is: size, dt, xMax, yMax, fulltime
 
        switch (i % 9) {
            case 0:
                glColor3ub (0, 0, 255); //blue
                break;
            case 1:
                glColor3ub (0, 0, 0); //white
                break;
            case 2:
                glColor3ub (255, 0, 0);
                break;
            case 3:
                glColor3ub (255, 255, 0); //yellow
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
        
        long double coef = maxX / xMax[i];

        file >> pointX >> pointY >> null >> null >> null >> null >> null;
        cout << pointX * coef * coefficient << " " << pointY * coef * coefficient << '\n';
 
        for (int j = 0; j < size; j++) {
            file >> pointX2 >> pointY2 >> null >> null >> null >> null >> null;
            
            glBegin (GL_LINES);
                glVertex2d (pointX * coef * coefficient, pointY * coef * coefficient);
                glVertex2d (pointX2 * coef * coefficient, pointY2 * coef * coefficient);
            glEnd ();
 
            pointX = pointX2;
            pointY = pointY2;
        }
 
        file.close ();
        header.close ();
 
    }

    glFlush ();
}



void keyboard (unsigned char key, int x, int y) {
    switch (key) {
        case 1: // Escape key
        case 16: // Escape key
        case 27: // Escape key
        case 'q':
            exit (0);
            //return; 
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
    if (byy > byx) coefficient = byx;
    else coefficient = byy;

    winw = w;
    winh = h;
}

long double max_x (long double a[], int len) {
    for (int i = 0; i < len; i++) {
        if (maxX < a[i]) {
            maxX = a[i];
            maxXnum = i;
        }
    }
}

long double max_y (long double a[], int len) {
    for (int i = 0; i < len; i++) {
        if (maxY < a[i] * maxX / xMax[i]) {
            maxY = a[i] * maxX / xMax[i];
            maxYnum = i;
        }
    }
}
