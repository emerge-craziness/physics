#include <iostream>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <NTL/RR.h>
using namespace std;
using namespace NTL;

RR axNext, 
   axPrev,
   ayNext,
   ayPrev,
   vxNext,
   vxPrev,
   vyNext,
   vyPrev,
   vyMax,
   xNext,
   xPrev,
   yNext,
   yPrev,
   chA,
   chB,
   chC,
   vFullMax,
   vFullNext,
   c,
   p,
   S, 
   K,
   zerospeed, 
   m, 
   alpha, 
   dt, 
   R, 
   g, 
   rad,
   fulltime, 
   x_max, 
   y_max, 
   y_max_on,
   v_max;

int calculated_points = 1;

string filenameValues,
       filenameHeader,
       filenameInfo;


//DECLARING FUNCTIONS 

int calculation();
int getdata(int mode);
int printdata();
int initialize_defaults();

/* PROGRAM BEGINNING */

int main(int argc, char ** argv) {

  initialize_defaults();

  if(argc == 3) {
    if(getdata(argv[2][0] - '0') == 1) return 1;
    filenameValues = argv[1] + (string)".txt";
    filenameHeader = argv[1] + (string)"_header.txt";
    filenameInfo = argv[1] + (string)"_info.txt";
  } 
  else if(argc == 2) {
    if(getdata(argv[1][0] - '0') == 1) return 1;
    filenameValues = "data.txt";
    filenameHeader = "data_header.txt";
    filenameInfo = "data_info.txt";
  }
  else {
    if(getdata(0) == 1) return 1;
    filenameValues = "data.txt";
    filenameHeader = "data_header.txt";
    filenameInfo = "data_info.txt";
  }

  calculation();
  printdata();

  return 0;
}

int printdata() {
   ofstream f;
   f.precision(6);
   f.open(filenameInfo.c_str(), ios::out);
   f << "INPUT"
   << "\nmass:                    " << m << " kilograms"
   << "\nangle:                   " << alpha << " degrees"
   << "\nmuzzle velocity:         " << zerospeed << " m/s"
   << "\ndelta t:                 " << dt << " second"
   << "\nacceleration of gravity: " << g << " m/s^2"
   << "\nair density:             " << p << " kg/m^2"
   << "\ndrag coefficient:        " << c
   << "\nradius:                  " << R << " metre"
   << setprecision(9)
   << "\n\nRESULTING DATA"
   << "\nmax height (=max y coordinate):   " << y_max << " metres (when x is " << y_max_on << ")"
   << "\ncarry (=max x coordinate):        " << x_max << " metres"
   << "\ntime:                             " << fulltime << " seconds"
   << "\nnum of calculated points:         " << calculated_points << "\n";
   cout << "\nData's been written to the \"" << filenameValues << "\" and \"" << filenameHeader << "\" files.\nMore information about results you can get from the " << filenameInfo << " file\n";
   f.close();
   return 0;
}

int getdata(int mode) {
  switch(mode) {
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

int initialize_defaults() {
  c = 1.2;
  p = 1.3;
  zerospeed = 120.0; 
  m = 0.005; 
  alpha = 45.0; 
  dt = 0.0001; 
  R = 0.03; 
  g = 9.81;
  return 0;
}


int calculation() {
  
  RR k[4], l[4];

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

  //OPEN FILE FOR WRITING:
  ofstream file;
  file.open(filenameValues.c_str(), ios::out);
  
  file << fixed << setprecision(9) 
       << xPrev << "\t" 
       << yPrev << "\t" 
       << axPrev << "\t" 
       << ayPrev << "\t" 
       << vxPrev << "\t" 
       << vyPrev << "\t" 
       << vFullNext << "\n";

  //CALCULATING:
  while (yPrev >= 0) {
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
    vxNext = vxPrev + axNext * dt;
     
    ayNext = (l[0] + 2 * l[1] + 2 * l[2] + l[3]) / 6;
    vyNext = vyPrev + ayNext * dt;
 
    xNext = xPrev + (vxPrev + vxNext) / 2 * dt;
    yNext = yPrev + (vyPrev + vyNext) / 2 * dt;
    
    vFullNext = sqrt(power(vxNext, 2) + power(vyNext, 2));
 
    //OFFTOP:
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

    file << fixed << setprecision(9) << xNext << "\t" << yNext << "\t" << xNext << "\t" << ayNext << "\t" << vxNext << "\t" << yNext  << "\t" << vFullNext << "\n";

    xPrev = xNext;
    yPrev = yNext;
    vxPrev = vxNext;
    vyPrev = vyNext;
    axPrev = axNext;
    ayPrev = ayNext;
  }
  
  file.close();

  ofstream headfile;
  headfile.open(filenameHeader.c_str(), ios::out);
  headfile << fixed << setprecision(9)
           << calculated_points << "\n"
           << dt << "\n"
           << x_max << "\n"
           << y_max << "\n"
           << vyMax << "\n"
           << vFullMax << "\n"
           << fulltime;
  headfile.close();
}
