//-lglut -lGL -lGLU
//-lgmpxx -lgmp

#include <iostream>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <mpfr.h>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;

//USED VARIABLES

int testn = 1;
//cout << "test " << testn++ << "\n";

mpfr_t axNext, 
       axPrev,
       ayNext,
       ayPrev,
       vxNext,
       vxPrev,
       vyNext,
       vyPrev,
       xNext,
       xPrev,
       yNext,
       yPrev,
       vFull,
       c, p, S, k_neg, zerospeed, m, alpha, dt, R, g, rad;


double in_dt = 0.0001, 
       in_alpha = 45,
       in_m = 0.005, 
       in_g = 9.81,
       in_c = 1.2,
       in_p = 1.3,
       in_R = 0.03,
       in_zerospeed = 120, 
       in_x0 = 0,
       in_y0 = 0,
       fulltime = 0, 
       x_max, 
       y_max, 
       y_max_on;

long double *data;

int winw, 
    winh, 
    size = 1, 
    bits = 64 * 8,
    length,
    drawing_mode = 1;

string filenameValues,
       filenameHeader;

//USED FUNCTIONS 

int calculation();
int getdata(int mode);
int printdata();


/* PROGRAM BEGINNING */

int main(int argc, char ** argv) {

    if(argc == 3) {
        if(getdata(argv[2][0] - '0') == 1) return 1;
        filenameValues = argv[1] + (string)".txt";
        filenameHeader = argv[1] + (string)"_header.txt";
    } 
    else if(argc == 2) {
        if(getdata(argv[1][0] - '0') == 1) return 1;
        filenameValues = "data.txt";
        filenameHeader = "data_header.txt";
    }
    else {
        if(getdata(0) == 1) return 1;
        filenameValues = "data.txt";
        filenameHeader = "data_header.txt";
    }

    calculation();
    //printdata();

    return 0;
}

int printdata() {

   cout.precision(6);
   cout << "\nINPUT"
        << "\nmass:                    " << in_m << " kilograms"
        << "\nangle:                   " << in_alpha << " degrees"
        << "\nmuzzle velocity:         " << in_zerospeed << " m/s"
        << "\ndelta t:                 " << in_dt << " second"
        << "\nacceleration of gravity: " << in_g << " m/s^2"
        << "\nair density:             " << in_p << " kg/m^2"
        << "\ndrag coefficient:        " << in_c
        << "\nradius:                  " << in_R << " metre"
        << setprecision(9)
        << "\n\nRESULTING DATA"
        << "\nmax height (=max y coordinate): " << y_max << " metres (when x is " << y_max_on << ")"
        << "\ncarry (=max x coordinate):      " << x_max << " metres"
        << "\ntime:                           " << fulltime << " seconds"
        << "\nnum of calculated points:       " << size << "\n";
        cout << "\nData's been written to the \"" << filenameValues << "\" and \"" << filenameHeader << "\" files.\n";
}

int getdata(int mode) {

    switch(mode) {

        default:
        case 0:
            cout << "mode 0\n";
            cout << "#1/3: angle (degrees; 0 < angle < 180): ";
            cin >> in_alpha;
                if((in_alpha > 90) && (in_alpha < 180)) in_alpha = in_alpha - 90;
                else if((in_alpha >= 180) || (in_alpha <= 0)) {
                    cerr << ">> impossible angle. its value must be in (0; 180)\n";
                    return 1;
                }
            cout << "#2/3: zero x: ";
            cin >> in_x0;
            cout << "#3/3: zero y: ";
            cin >> in_y0;
            break;

        case 1:
            cout << "mode 1\n";
            cout << "#1/5: mass (kg): ";
            cin >> in_m;
            cout << "#2/5: angle (degrees; 0 < angle < 180): ";
            cin >> in_alpha;
                if((in_alpha > 90) && (in_alpha < 180)) in_alpha = in_alpha - 90;
                else if((in_alpha >= 180) || (in_alpha <= 0)) {
                    cerr << ">> impossible angle. its value must be in (0; 180)\n";
                    return 1;
                }
            cout << "#3/5: muzzle velocity (m/s): ";
            cin >> in_zerospeed;
            cout << "#4/5: zero x: ";
            cin >> in_x0;
            cout << "#5/5: zero y: ";
            cin >> in_y0;
            break;

        case 2:
            cout << "mode 2\n";
            cout << "#1/6: mass (kg): ";
            cin >> in_m;
            cout << "#2/6: angle (degrees; 0 < angle < 180): ";
            cin >> in_alpha;
                if((in_alpha > 90) && (in_alpha < 180)) in_alpha = in_alpha - 90;
                else if((in_alpha >= 180) || (in_alpha <= 0)) {
                    cerr << ">> impossible angle. its value must be in (0; 180)\n";
                    return 1;
                }
            cout << "#3/6: muzzle velocity (m/s): ";
            cin >> in_zerospeed;
            cout << "#4/6: zero x: ";
            cin >> in_x0;
            cout << "#5/6: zero y: ";
            cin >> in_y0;
            cout << "#6/6: delta t (s): 1 / ";
            cin >> in_dt;
                in_dt = 1 / in_dt;
            break;

        case 3:
            cout << "mode 3\n";
            cout << "#1/11: mass (kg): ";
            cin >> in_m;
            cout << "#2/11: angle (degrees; 0 < angle < 180): ";
            cin >> in_alpha;
                if((in_alpha > 90) && (in_alpha < 180)) in_alpha = in_alpha - 90;
                else if((in_alpha >= 180) || (in_alpha <= 0)) {
                    cerr << ">> impossible angle. its value must be in (0; 180)\n";
                    return 1;
                }
            cout << "#3/11: muzzle velocity (m/s): ";
            cin >> in_zerospeed;
            cout << "#4/11: zero x: ";
            cin >> in_x0;
            cout << "#5/11: zero y: ";
            cin >> in_y0;
            cout << "#6/11: delta t (s): 1 / ";
            cin >> in_dt;
                in_dt = 1 / in_dt;
            cout << "#7/11: acceleration of gravity (m/s^2): ";
            cin >> in_g;
            cout << "#8/11: air density (kg/m^3): ";
            cin >> in_p;
            cout << "#9/11: drag coefficient: ";
            cin >> in_c;
            cout << "#10/11: radius of the sphere (m): ";
            cin >> in_R;
            cout << "#11/11: bytes of precision: ";
            cin >> bits;
                bits *= 8;
            break;
    
        cout << "\n";
    }
}


int calculation() {
    
    mpfr_set_default_prec(bits);
    
    //FILE FOR WRITING:

    ofstream file;
    file.open(filenameValues.c_str(), ios::out);
    
    //INITIALIZING:
    
    mpfr_t t[8];
    for(int i = 0; i < 8; ++i) mpfr_init(t[i]);
    mpfr_init(c);
    mpfr_init(p);
    mpfr_init(S);
    mpfr_init(k_neg);
    mpfr_init(m);
    mpfr_init(dt);
    mpfr_init(zerospeed);
    mpfr_init(alpha);
    mpfr_init(R);
    mpfr_init(g);
    mpfr_init(rad);
    mpfr_init(vxNext);
    mpfr_init(vxPrev);
    mpfr_init(axNext);
    mpfr_init(axPrev);
    mpfr_init(vyNext);
    mpfr_init(vyPrev);
    mpfr_init(ayNext);
    mpfr_init(ayPrev);
    mpfr_init(xNext);
    mpfr_init(xPrev);
    mpfr_init(yNext);
    mpfr_init(yPrev);
    mpfr_init(vFull);

    
    //SETTING VALUES:

    mpfr_set_d(c, in_c, MPFR_RNDU);

    mpfr_set_d(p, in_p, MPFR_RNDU);

    mpfr_set_d(R, in_R, MPFR_RNDU);

    mpfr_pow_ui(t[0], R, 2, MPFR_RNDU);
    mpfr_mul_d(S, t[0], 3.14159265, MPFR_RNDU);

    mpfr_mul(t[0], c, p, MPFR_RNDU);
    mpfr_mul(t[1], t[0], S, MPFR_RNDU);
    mpfr_mul_si(t[2], t[1], -1, MPFR_RNDU);
    mpfr_div_ui(k_neg, t[2], 2, MPFR_RNDU);

    mpfr_set_d(zerospeed, in_zerospeed, MPFR_RNDU);

    mpfr_set_d(m, in_m, MPFR_RNDU);

    mpfr_set_d(alpha, abs(in_alpha), MPFR_RNDU);

    mpfr_set_d(dt, in_dt, MPFR_RNDU);

    mpfr_set_d(g, in_g, MPFR_RNDU);

    mpfr_mul_d(t[0], alpha, 3.141592653589793, MPFR_RNDU);
    mpfr_div_ui(rad, t[0], 180, MPFR_RNDU);

    mpfr_cos(t[0], rad, MPFR_RNDZ);
    mpfr_mul(vxPrev, zerospeed, t[0], MPFR_RNDZ);

    mpfr_sin(t[0], rad, MPFR_RNDZ);
    mpfr_mul(vyPrev, zerospeed, t[0], MPFR_RNDZ);

    mpfr_set_d(xPrev, in_x0, MPFR_RNDZ);

    mpfr_set_d(yPrev, in_y0, MPFR_RNDZ);   


    y_max = mpfr_get_ld(yPrev, MPFR_RNDA);
    y_max_on = mpfr_get_ld(xPrev, MPFR_RNDA);

    x_max = mpfr_get_ld(xPrev, MPFR_RNDA);

    file << fixed << setprecision(9) << mpfr_get_ld(xPrev, MPFR_RNDA) << "\t" << mpfr_get_ld(yPrev, MPFR_RNDA) << "\t" << 0.0 << "\n" << 0.0 << "\n" << 0.0 << "\n" << 0.0 << "\n" << 0.0 << "\n"; //TODO: add the absolute speed values also.

    //CALCULATION:

    for(int i = 1; mpfr_get_d(yPrev, MPFR_RNDA) >= 0; ++i) {

    //ax next: -K * sqrt(vx^2 + vy^2) * vx / m
         mpfr_pow_ui(t[0], vxPrev, 2, MPFR_RNDZ);
         mpfr_pow_ui(t[1], vyPrev, 2, MPFR_RNDZ);
         mpfr_add(t[2], t[0], t[1], MPFR_RNDZ);
         mpfr_sqrt(t[3], t[2], MPFR_RNDZ);
         mpfr_mul(t[4], t[3], vxPrev, MPFR_RNDZ);
         mpfr_mul(t[5], k_neg, t[4], MPFR_RNDZ);
         mpfr_div(axNext, t[5], m, MPFR_RNDZ);
    
    //ay next: -K * sqrt(vx^2 + vy^2) * vy / m
         mpfr_pow_ui(t[0], vxPrev, 2, MPFR_RNDZ);
         mpfr_pow_ui(t[1], vyPrev, 2, MPFR_RNDZ);
         mpfr_add(t[2], t[0], t[1], MPFR_RNDZ);
         mpfr_sqrt(t[3], t[2], MPFR_RNDZ);
         mpfr_mul(t[4], t[3], vyPrev, MPFR_RNDZ);
         mpfr_mul(t[5], k_neg, t[4], MPFR_RNDZ);
         mpfr_div(t[6], t[5], m, MPFR_RNDZ);
         mpfr_sub(ayNext, t[6], g, MPFR_RNDZ);
    
    //vx next: vx + ax * dt
         mpfr_mul(t[0], axNext, dt, MPFR_RNDZ);
         mpfr_add(vxNext, vxPrev, t[0], MPFR_RNDZ);
    
    //vy next: vy + ay * dt
         mpfr_mul(t[0], ayNext, dt, MPFR_RNDZ);
         mpfr_add(vyNext, vyPrev, t[0], MPFR_RNDZ);
    
    //x  next: x + (vx_previous + vx) / 2 * dt
         mpfr_add(t[0], vxPrev, vxNext, MPFR_RNDZ);
         mpfr_mul(t[1], t[0], dt, MPFR_RNDZ);
         mpfr_div_ui(t[2], t[1], 2, MPFR_RNDZ);
         mpfr_add(xNext, xPrev, t[2], MPFR_RNDZ);
    
    //y  next: y + (vy_previous + vy) / 2 * dt
         mpfr_add(t[0], vyPrev, vyNext, MPFR_RNDZ);
         mpfr_mul(t[1], t[0], dt, MPFR_RNDZ);
         mpfr_div_ui(t[2], t[1], 2, MPFR_RNDZ);
         mpfr_add(yNext, yPrev, t[2], MPFR_RNDZ);
        
   //vFull: sqrt(vx + vy)
        mpfr_add(t[0], vxPrev, vyPrev, MPFR_RNDA);
        mpfr_abs(t[0], t[0], MPFR_RNDA);
        mpfr_sqrt(vFull, t[0], MPFR_RNDA);
        

    //OFFTOP:
        ++size;
        
        if(mpfr_get_d(yNext, MPFR_RNDA) > mpfr_get_d(yPrev, MPFR_RNDA)) {
            y_max = mpfr_get_ld(yPrev, MPFR_RNDA);
            y_max_on = mpfr_get_ld(xPrev, MPFR_RNDA);
        }

        x_max = mpfr_get_ld(xPrev, MPFR_RNDA);
        
        fulltime += in_dt;

    //WRITING TO THE FILE
        file << fixed << setprecision(9) << mpfr_get_ld(xNext, MPFR_RNDA) << "\t" << mpfr_get_ld(yNext, MPFR_RNDA) << "\t" << 0.0 << "\n" << 0.0 << "\n" << 0.0 << "\n" << 0.0 << "\n" << 0.0 << "\n"; //TODO: add the absolute speed values also.


    //CHANGING VALUES
        mpfr_set(xPrev, xNext, MPFR_RNDA);
        mpfr_set(yPrev, yNext, MPFR_RNDA);
        mpfr_set(vxPrev, vxNext, MPFR_RNDA);
        mpfr_set(vyPrev, vyNext, MPFR_RNDA);
        mpfr_set(axPrev, axNext, MPFR_RNDA);
        mpfr_set(ayPrev, ayNext, MPFR_RNDA);
    }
    file.close();

    ofstream headfile;
    headfile.open(filenameHeader.c_str(), ios::out);
    headfile << fixed << setprecision(9)
             << size << "\n"
             << in_dt << "\n"
             << x_max << "\n"
             << y_max << "\n"
             << fulltime;
    headfile.close();
}
