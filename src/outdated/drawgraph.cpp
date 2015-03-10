//-lglut -lGL -lGLU
//-lgmpxx -lgmp
//
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
    length, 
    drawing_mode = 1;

double pointX,
       pointX2,
       pointY,
       pointY2,
       dt,
       xMax,
       yMax,
       border,
       coefficient,
       coefficient_x,
       coefficient_y,
       fulltime,
       null;

string filename, 
       headername;


//USED FUNCTIONS 

void reshape(int w, int h);
void renderScene_mode_1();
void renderScene_mode_2();
void renderScene_switch();
void keyboard(unsigned char key, int x, int y);
void renderBitmapString(double x, double y, void *font, string str, char);

int main(int argc, char ** argv) {
    
    if(argc == 2) {
        filename = argv[1] + (string)".txt";
        headername = argv[1] + (string)"_header.txt";
    }
    else {
        headername = "header.txt";
        filename = "data.txt";
    }
    cout << argc << " " << headername << " " << filename << "\n";

    ifstream header;
    header.open(headername.c_str(), ios::in);
    header >> size >> dt >> xMax >> yMax >> fulltime; // format is: size, dt, xMax, yMax, fulltime
    header.close();
    cout << size << " " << dt << " " << xMax << " " << yMax << " " << fulltime << "\n";

    glutInit(&argc, argv);
    glutInitWindowSize(1280, 780);
    glutInitWindowPosition(1, 1);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Physics");
    glutDisplayFunc(renderScene_switch);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glClearColor(0, 0, 0, 0);
    glutMainLoop();

    return 0;
}

void renderScene_switch() {
    switch(drawing_mode) {
        case 1:
            renderScene_mode_1();
            break;
        case 2:
            renderScene_mode_2();
            break;
    }       
}


//y(x) GRAPH:

void renderScene_mode_1() { 

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    string graph = "y(x):";
    glColor3f(0.5f, 0.5f, 0.5f);
    renderBitmapString(34, winh - 23, GLUT_BITMAP_HELVETICA_18, graph, '1');

    
    //GRID:

    double grid_coefficient = coefficient;
    double i = 10;
    if(coefficient > 50) {
        for(grid_coefficient = coefficient; grid_coefficient > 50; grid_coefficient /= 10) {
            i /= 10;
        }
    }
    else if(coefficient < 5) {
        for(grid_coefficient = coefficient; grid_coefficient < 5; grid_coefficient *= 10) {
            i /= 0.1;
        }
    }
    const double i_beginning = i;
    int difference = i / i_beginning;
    if(i != i_beginning) {
        grid_coefficient /= (i / i_beginning);
        i = i_beginning;
    }
    ostringstream out;
    out << i / 10;
    

    //HORIZONTAL
    
    for(int g = 0; g * grid_coefficient * difference + border * 2 < winh; ++g) {
        if(g * difference % 10 == 0) {
            glLineWidth(2);
            glColor3f(0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"m";
            renderBitmapString(3, g * grid_coefficient + border, GLUT_BITMAP_HELVETICA_10, a, 'v');
        }
        else {
            glLineWidth(1);
            glColor3f(0.3f, 0.3f, 0.3f);
        }
        glBegin(GL_LINES);
            glVertex2f(border, g * grid_coefficient * difference + border);
            glVertex2f(winw - border, g * grid_coefficient * difference + border);
        glEnd();
    }
    
    
    //VERTICAL

    for(int g = 0; g * grid_coefficient * difference + border * 2 < winw; ++g) {
        if(g * difference % 10 == 0) {
            glLineWidth(2);
            glColor3f(0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"m";
            renderBitmapString(g * grid_coefficient + border, 3, GLUT_BITMAP_HELVETICA_10, a, 'h');
        }
        else {
            glLineWidth(1);
            glColor3f(0.3f, 0.2f, 0.2f);
        }
        glBegin(GL_LINES);
            glVertex2f(grid_coefficient * g * difference + border, border);
            glVertex2f(grid_coefficient * g * difference + border, winh - border);
        glEnd();
    }
    
    
    //GRAPH:

    ifstream file, header;
    file.open(filename.c_str(), ios::in);
    header.open(headername.c_str(), ios::in);
    header >> size >> dt >> xMax >> yMax; // format is: size, dt, xMax, yMax, fulltime

    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(border, border, 0);
    glLineWidth(3);
    
    file >> pointX >> pointY >> null;

    for(int i = 0; i < size; i++) {
        file >> pointX2 >> pointY2 >> null;
        
        glBegin(GL_LINES);
            glVertex2d(pointX * coefficient, pointY * coefficient);
            glVertex2d(pointX2 * coefficient, pointY2 * coefficient);
        glEnd();

        pointX = pointX2;
        pointY = pointY2;
    }

    glFlush();

    file.close();
    header.close();
}


//x(t) GRAPH:

void renderScene_mode_2() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    string graph = "x(t):";
    glColor3f(0.5f, 0.5f, 0.5f);
    renderBitmapString(34, winh - 23, GLUT_BITMAP_HELVETICA_18, graph, '1');


    //GRID:
    
    double grid_coefficient = coefficient_x;
    double i = 10;
    if(coefficient_x > 50) for(grid_coefficient = coefficient_x; grid_coefficient > 50; grid_coefficient /= 10) {
        i /= 10;
    }
    else if(coefficient_x < 5) for(grid_coefficient = coefficient_x; grid_coefficient < 5; grid_coefficient *= 10) {
        i /= 0.1;
    }
    const double i_beginning = i;
    int difference = i / i_beginning;
    if(i != i_beginning) {
        grid_coefficient /= (i / i_beginning);
        i = i_beginning;
    }
    ostringstream out;
    out << i / 10;
    
    
    //HORIZONTAL
    
    for(int g = 0; g * grid_coefficient * difference + border * 2 < winh; ++g) {
        if(g * difference % 10 == 0) {
            glLineWidth(2);
            glColor3f(0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"m";
            renderBitmapString(3, g * grid_coefficient + border, GLUT_BITMAP_HELVETICA_10, a, 'v');
        }
        else {
            glLineWidth(1);
            glColor3f(0.3f, 0.3f, 0.3f);
        }
        glBegin(GL_LINES);
            glVertex2f(border, g * grid_coefficient * difference + border);
            glVertex2f(winw - border, g * grid_coefficient * difference + border);
        glEnd();
    }
    
    
    //VERTICAL
    
    for(int g = 0; g * grid_coefficient * difference + border * 2 < winw; ++g) {
        if(g * difference % 10 == 0) {
            glLineWidth(2);
            glColor3f(0.5f, 0.5f, 0.5f);
            ostringstream out;
            out << i * g / 10;
            string a = out.str().c_str() + (string)"s";
            renderBitmapString(g * grid_coefficient + border, 3, GLUT_BITMAP_HELVETICA_10, a, 'h');
        }
        else {
            glLineWidth(1);
            glColor3f(0.3f, 0.2f, 0.2f);
        }
        glBegin(GL_LINES);
            glVertex2f(grid_coefficient * g * difference + border, border);
            glVertex2f(grid_coefficient * g * difference + border, winh - border);
        glEnd();
    }    


    //GRAPH
    
    ifstream file, header;
    file.open(filename.c_str(), ios::in);
    header.open(headername.c_str(), ios::in);
    header >> size >> dt >> xMax;

    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(border, border, 0);
    glLineWidth(3);
    
    file >> pointX >> null;

    for(int i = 0; i < size - 2; i++) {
        file >> null >> pointX2 >> null;

        glBegin(GL_LINE_STRIP);
            glVertex2d((double)i * dt * coefficient_x, pointX * coefficient_x);
            glVertex2d(((double)i+1) * dt * coefficient_x, pointX2 * coefficient_x);
        glEnd();

        pointX = pointX2;

    }

    glFlush();

    file.close();
    header.close();
}

void renderBitmapString(double x, double y, void *font, string str, char) {
    glRasterPos2d(x, y);
    for(int i = 0; i < str.length(); ++i) {
        glutBitmapCharacter(font, str[i]);
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 1: // Escape key
        case 16: // Escape key
        case 27: // Escape key
        case 'q':
            exit(0);
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
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);      
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    border = 40;
    long double byy = (h - border * 2.7) / yMax;
    long double byx = (w - border * 2.7) / xMax;
    if(byy > byx) coefficient = byx;
    else coefficient = byy;
    if((h - border * 2.7) / xMax > (w - border * 2.7) / fulltime) coefficient_x = (w - border * 2.7) / fulltime;
    else coefficient_x = (h - border * 2.7) / xMax;
    if((h - border * 2.7) / yMax > (w - border * 2.7) / fulltime) coefficient_y = (w - border * 2.7) / fulltime;
    else coefficient_y = (h - border * 2.7) / yMax;
    winw = w;
    winh = h;
}
