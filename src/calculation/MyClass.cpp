#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ncurses.h>
#include <form.h>
#include "MyClass.hpp"
using namespace std;

int MyClass::setFilenames (std::string fValues, std::string fHeader, std::string fInfo) {
  filenameValues = fValues;
  filenameHeader = fHeader;
  filenameInfo = fInfo;
  return 0;
}

int MyClass::calculateAccelerations() {
  for (int j = 0; j < 4; j++) {
    switch(j) {
      case 0:
        chA = 0;
        chB = 0;
        chC = 0;
        break;
      case 1:
      case 2:
        chA = k[j-1] * dt / 2;
        chB = dt / 2;
        chC = dt * l[j-1] / 2;
        break;
      case 3:
        chA = dt * k[2];
        chB = dt;
        chC = dt * l[2];
        break;
    }
    k[j] = -K * (vxPrev + chA) * sqrt(power(vxPrev + chA, 2) + power(vyPrev + chB, 2)) / m;
    l[j] = -K * (vyPrev + chB) * sqrt(power(vxPrev + chC, 2) + power(vyPrev + chB, 2)) / m - g;
  }
  
  axNext = (k[0] + 2 * k[1] + 2 * k[2] + k[3]) / 6;
  ayNext = (l[0] + 2 * l[1] + 2 * l[2] + l[3]) / 6;

  return 0;
}

int MyClass::calculateChangeValues() {
    xPrev = xNext;
    yPrev = yNext;
    vxPrev = vxNext;
    vyPrev = vyNext;
    axPrev = axNext;
    ayPrev = ayNext;
    return 0;
}

int MyClass::calculateCoordinates() {
  xNext = xPrev + (vxPrev + vxNext) / 2 * dt;
  yNext = yPrev + (vyPrev + vyNext) / 2 * dt;
  return 0;
}

int MyClass::calculateOfftop() {
  ++calculated_points;
  x_max = xPrev;
  fulltime += dt;
  
  if (yNext > yPrev) {
    y_max = yNext;
    y_max_on = xNext;
  }

  if (vFullMax < vFullNext) {
    vFullMax = vFullNext;
  }

  if (vyMax < vyNext) {
    vyMax = vyNext;
  }

  return 0;
}

int MyClass::calculateSpeeds() {
  vxNext = vxPrev + axNext * dt;
  vyNext = vyPrev + ayNext * dt;
  vFullNext = sqrt(power(vxNext, 2) + power(vyNext, 2));
  return 0;
}

int MyClass::clearFiles() {
  ofstream fileValues, fileHeader, fileInfo;
  fileValues.open (filenameValues.c_str(), ios::out);
  fileHeader.open (filenameHeader.c_str(), ios::out);
  fileInfo.open (filenameInfo.c_str(), ios::out);
  /* fileValues << "\r";
  fileHeader << "\r";
  fileInfo << "\r"; */
  fileValues.close();
  fileHeader.close();
  fileInfo.close();
  return 0;
}


int MyClass::calculateWriteToTheFile () {
  std::ofstream file;
  file.open (filenameValues.c_str(), ios::app);
  file << std::fixed << std::setprecision (9) 
       << xPrev << "\t" 
       << yPrev << "\t" 
       << axPrev << "\t" 
       << ayPrev << "\t" 
       << vxPrev << "\t" 
       << vyPrev << "\t" 
       << vFullNext << "\n";
  file.close();
  
  /* For experiment *
  std::cout << std::fixed << std::setprecision (9) 
       << xPrev << "\t" 
       << yPrev << "\t" 
       << axPrev << "\t" 
       << ayPrev << "\t" 
       << vxPrev << "\t" 
       << vyPrev << "\t" 
       << vFullNext << "\n";
   * end */

  return 0;
}

int MyClass::calculateWriteToTheHeader() {
  std::ofstream file;
  file.open(filenameHeader.c_str(), ios::app);
  file << std::fixed << std::setprecision(9)
       << calculated_points << "\n"
       << dt << "\n"
       << x_max << "\n"
       << y_max << "\n"
       << vyMax << "\n"
       << vFullMax << "\n"
       << fulltime;
  file.close();
  return 0;
} 

int MyClass::calculation() {
  
  //SETTING VALUES:
  S = power(R, 2.0) * 3.14592653589793;
  K = c * p * S / 2;
  rad = alpha * 3.14592653589793 / 180; //ComputePi_RR() TODO
  vxPrev = cos(rad) * zerospeed;
  vyPrev = sin(rad) * zerospeed;
  vFullNext = sqrt (power (vxPrev, 2.0) + power (vyPrev, 2.0));
  y_max = yPrev;
  y_max_on = xPrev;
  x_max = xPrev;
  
  clearFiles();
  //calculateWriteToTheFile();

  while (yPrev >= 0) {
    calculateAccelerations();  // MAKE THIS METHOD
    calculateSpeeds(); // MAKE THIS METHOD
    calculateCoordinates(); // MAKE THIS METHOD
    calculateOfftop(); // MAKE THIS METHOD
    calculateWriteToTheFile();
    calculateChangeValues(); // MAKE THIS METHOD
  }
  
  calculateWriteToTheHeader(); // MAKE THIS METHOD
  
  return 0;
}

int MyClass::_getdata (int mode) {
  switch (mode) {
  default:
  case 0:
    cout << "mode 0\n";
    cout << "#1/3: angle, degrees: ";
    cin >> alpha;
    if(alpha > 90) {
      alpha = 180 - alpha;
    }
    cout << "#2/3: zero x: ";
    cin >> xPrev;
    cout << "#3/3: zero y: ";
    cin >> yPrev;
    break;

  case 1:
    cout << "mode 1\n";
    cout << "#1/5: mass (kg): ";
    cin >> m;
    cout << "#2/5: angle (degrees; 0 < angle < 180): ";
    cin >> alpha;
    if(alpha > 90) {
      alpha = 180 - alpha;
    }
    cout << "#3/5: muzzle velocity (m/s): ";
    cin >> zerospeed;
    cout << "#4/5: zero x: ";
    cin >> xPrev;
    cout << "#5/5: zero y: ";
    cin >> yPrev;
    break;

  case 2:
    cout << "mode 2\n";
    cout << "#1/6: mass (kg): ";
    cin >> m;
    cout << "#2/6: angle (degrees; 0 < angle < 180): ";
    cin >> alpha;
    if(alpha > 90) {
      alpha = 180 - alpha;
    }
    cout << "#3/6: muzzle velocity (m/s): ";
    cin >> zerospeed;
    cout << "#4/6: zero x: ";
    cin >> xPrev;
    cout << "#5/6: zero y: ";
    cin >> yPrev;
    cout << "#6/6: delta t (s): 1 / ";
    cin >> dt;
    dt = 1 / dt;
    break;

  case 3:
    cout << "mode 3\n";
    cout << "#1/11: mass (kg): ";
    cin >> m;
    cout << "#2/11: angle (degrees; 0 < angle < 180): ";
    cin >> alpha;
    if(alpha > 90) {
      alpha = 180 - alpha;
    }
    cout << "#3/11: muzzle velocity (m/s): ";
    cin >> zerospeed;
    cout << "#4/11: zero x: ";
    cin >> xPrev;
    cout << "#5/11: zero y: ";
    cin >> yPrev;
    cout << "#6/11: delta t (s): 1 / ";
    cin >> dt;
    dt = 1 / dt;
    cout << "#7/11: acceleration of gravity (m/s^2): ";
    cin >> g;
    cout << "#8/11: air density (kg/m^3): ";
    cin >> p;
    cout << "#9/11: drag coefficient: ";
    cin >> c;
    cout << "#10/11: radius of the sphere (m): ";
    cin >> R;
    break;
  }
  return 0;
}

int MyClass::getdata()
{
    char values[11][20]; // to store data
    FIELD *field[11];
    FORM *my_form;
    wchar_t ch;

    /* Initialize screen */
    initscr();
    cbreak();
    noecho();
    raw();
    keypad( stdscr, true );

    /* Initialize the fields and set their options */
    for ( int i = 0; i < 10; i++ )
    {
        field[i] = new_field( 1, 20, 2 + i * 2, 25, 0, 0 );
        set_field_back( field[i], A_UNDERLINE ); 
        field_opts_off( field[i], O_AUTOSKIP );
    }
    field[10] = NULL;
    refresh();

    /* Create the form and post it */
    my_form = new_form( field );
    post_form( my_form );
    refresh();


    /* Initialize defaults */
    stringstream tempss;
    tempss << m << "\n";
    tempss << alpha << "\n";
    tempss << zerospeed << "\n";
    tempss << xPrev << "\n";
    tempss << yPrev << "\n";
    tempss << dt << "\n";
    tempss << g << "\n";
    tempss << p << "\n";
    tempss << c << "\n";
    tempss << R << "\n";
    string tempstring;
    for ( int i = 0; i < 10; i++ )
    {
        tempss >> tempstring;
        set_field_buffer( field[i], 0, tempstring.c_str() );
    }
    refresh();

    mvprintw( 2, 2, "mass: ");
    mvprintw( 4, 2, "angle: " );
    mvprintw( 6, 2, "zero speed: " );
    mvprintw( 8, 2, "zero x: " );
    mvprintw( 10, 2, "zero y: " );
    mvprintw( 12, 2, "delta time: " );
    mvprintw( 14, 2, "g: " );
    mvprintw( 16, 2, "air density: " );
    mvprintw( 18, 2, "coefficient: " );
    mvprintw( 20, 2, "radius: " );
    mvprintw( 22, 2, "Press <F5> to run calculation, <F1> to escape" );
    refresh();


    

    
    /* Loop through to get the data */
    //form_driver( my_form, REQ_NEXT_FIELD );
    //form_driver( my_form, REQ_PREV_FIELD );
    form_driver( my_form, REQ_END_LINE );
    while(( ch = getch() ) != KEY_F( 5 ))
    {   
        /* cerr << "ch: " << ch << "\n"; */
        switch(ch)
        {   
            case KEY_F( 1 ):
                unpost_form( my_form );
                free_form( my_form );
                for ( int i = 0; i < 11; i++ )
                {
                    free_field( field[i] );
                }
                endwin();
                return 1;
            case 10: // Enter
            case KEY_DOWN:
                /* Go to next field */
                form_driver( my_form, REQ_NEXT_FIELD );
                /* Go to the end of the present buffer */
                /* Leaves nicely at the last character */
                form_driver( my_form, REQ_END_LINE );
                break;
            case KEY_UP:
                /* Go to previous field */
                form_driver( my_form, REQ_PREV_FIELD );
                form_driver( my_form, REQ_END_LINE );
                break;
            case KEY_LEFT:
                form_driver( my_form, REQ_PREV_CHAR );
                break;
            case KEY_RIGHT:
                form_driver( my_form, REQ_NEXT_CHAR );
                break;
            case 330: // Delete
                form_driver( my_form, REQ_DEL_CHAR );
                break;
            case 127: // Backspace
                /* Delete a char */
                form_driver( my_form, REQ_PREV_CHAR );
                form_driver( my_form, REQ_DEL_CHAR );
                break;
            default:
                /* If this is a normal character, it gets */
                /* Printed                */
                form_driver(my_form, ch);
                break;
        }
    }
    form_driver( my_form, REQ_VALIDATION );

    stringstream ss;
    for ( int i = 0; i < 11; i++ )
    {
        snprintf( values[i], 20, "%s", field_buffer( field[i], 0 ) );
        for ( int j = 0; j < 19; j++ )
        {
            ss << values[i][j];
        }
        ss << " ";
    }

    ss >> m
       >> alpha
       >> zerospeed
       >> xPrev
       >> yPrev
       >> dt
       >> g
       >> p
       >> c
       >> R;
    
    writeDefaults();
    
    unpost_form( my_form );
    free_form( my_form );
    for ( int i = 0; i < 11; i++ )
    {
        free_field( field[i] );
    }

    endwin();
    return 0;
}

int MyClass::initializeDefaults() 
{
    ifstream defaultsFile;
    defaultsFile.open( "./.calculate.defaults" );
    if( defaultsFile.good() )
    {
        defaultsFile >> m
                     >> alpha
                     >> zerospeed
                     >> xPrev
                     >> yPrev
                     >> dt
                     >> g
                     >> p
                     >> c
                     >> R;
    }
    else
    {
        c = 1.2;
        p = 1.3;
        zerospeed = 120.0; 
        m = 0.005; 
        alpha = 45.0; 
        dt = 0.0001; 
        R = 0.03; 
        g = 9.81;
        xPrev = 0;
        yPrev = 0;
    }

    calculated_points = 1;

    return 0;
}

int MyClass::writeDefaults()
{
    ofstream defaultsFile;
    defaultsFile.open( "./.calculate.defaults", ios::out );
    defaultsFile << m << "\n"
                 << alpha << "\n"
                 << zerospeed << "\n"
                 << xPrev << "\n"
                 << yPrev << "\n"
                 << dt << "\n"
                 << g << "\n"
                 << p << "\n"
                 << c << "\n"
                 << R;
    defaultsFile.close();
    return 0;
}
    

int MyClass::printdata() {
   std::ofstream file;
   file.precision (6);
   file.open (filenameInfo.c_str(), ios::app);
   file << "INPUT"
   << "\nmass:                    " << m << " kilograms"
   << "\nangle:                   " << alpha << " degrees"
   << "\nmuzzle velocity:         " << zerospeed << " m/s"
   << "\ndelta t:                 " << dt << " second"
   << "\nacceleration of gravity: " << g << " m/s^2"
   << "\nair density:             " << p << " kg/m^2"
   << "\ndrag coefficient:        " << c
   << "\nradius:                  " << R << " metre"
   << std::setprecision(9)
   << "\n\nRESULTING DATA"
   << "\nmax height (=max y coordinate):   " << y_max << " metres (when x is " << y_max_on << ")"
   << "\ncarry (=max x coordinate):        " << x_max << " metres"
   << "\ntime:                             " << fulltime << " seconds"
   << "\nnum of calculated points:         " << calculated_points << "\n";
   std::cout << "done.\nData've been written to \"" << filenameValues << "\" and \"" << filenameHeader << "\".\nMore information about results you can get from \"" << filenameInfo << "\".\n" << flush;
   file.close();
   return 0;
}

std::string MyClass::filenameValues;
std::string MyClass::filenameHeader;
std::string MyClass::filenameInfo;
NTL::RR MyClass::axNext; 
NTL::RR MyClass::axPrev;
NTL::RR MyClass::ayNext;
NTL::RR MyClass::ayPrev;
NTL::RR MyClass::vxNext;
NTL::RR MyClass::vxPrev;
NTL::RR MyClass::vyNext;
NTL::RR MyClass::vyPrev;
NTL::RR MyClass::vyMax;
NTL::RR MyClass::xNext;
NTL::RR MyClass::xPrev;
NTL::RR MyClass::yNext;
NTL::RR MyClass::yPrev;
NTL::RR MyClass::chA;
NTL::RR MyClass::chB;
NTL::RR MyClass::chC;
NTL::RR MyClass::vFullMax;
NTL::RR MyClass::vFullNext;
NTL::RR MyClass::c;
NTL::RR MyClass::p;
NTL::RR MyClass::S; 
NTL::RR MyClass::K;
NTL::RR MyClass::zerospeed; 
NTL::RR MyClass::m; 
NTL::RR MyClass::alpha; 
NTL::RR MyClass::dt; 
NTL::RR MyClass::R; 
NTL::RR MyClass::g; 
NTL::RR MyClass::rad;
NTL::RR MyClass::fulltime; 
NTL::RR MyClass::x_max; 
NTL::RR MyClass::y_max; 
NTL::RR MyClass::y_max_on;
NTL::RR MyClass::v_max;
NTL::RR MyClass::k[4];
NTL::RR MyClass::l[4];
int MyClass::calculated_points;
