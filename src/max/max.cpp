//-lglut -lGL -lGLU -lgmpxx -lgmp

#include <GL/glut.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <mpfr.h>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;

//USED VARIABLES

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
       chA,
       chB,
       chC,
       c, p, S, k_neg, zerospeed, m, alpha, dt, R, g, rad;

double in_dt, 
       in_alpha,
       in_m,
       in_g,
       in_c,
       in_p,
       in_R,
       in_zerospeed,
       in_x0,
       in_y0,
       x_max_prev, 
       x_max_curr, 
       x_max_MAX, 
       y_max,
       y_max_on;

int bits = 64 * 8,
    j;

//USED FUNCTIONS 
int calculation();
int getdata();
int printdata();


int main(int argc, char ** argv) {
    getdata();
    calculation();
    printdata();

    return in_alpha;
}


int printdata() {
    cout << "\nmax carry is " << x_max_MAX << " meters on the " << j - 2 << " degrees angle.\n";
}


int getdata() {
            cout << "#1/10: mass (kg): ";
            cin >> in_m;
            cout << "#2/10: muzzle velocity (m/s): ";
            cin >> in_zerospeed;
            cout << "#3/10: zero x: ";
            cin >> in_x0;
            cout << "#4/10: zero y: ";
            cin >> in_y0;
            cout << "#5/10: delta t (s): 1 / ";
            cin >> in_dt;
                in_dt = 1 / in_dt;
            cout << "#6/10: acceleration of gravity (m/s^2): ";
            cin >> in_g;
            cout << "#7/10: air density (kg/m^3): ";
            cin >> in_p;
            cout << "#8/10: drag coefficient: ";
            cin >> in_c;
            cout << "#9/10: radius of the sphere (m): ";
            cin >> in_R;
            cout << "#10/10: bytes of precision: ";
            cin >> bits;
                bits *= 8;
            cout << "\n";
}

int calculation() {

    mpfr_set_default_prec(bits);
    
    //INITIALIZING:
    mpfr_t t[9];
    for(int i = 0; i < 9; ++i) mpfr_init(t[i]);
    mpfr_t k[4], l[4];
    for(int i = 0; i < 4; ++i) {
        mpfr_init(k[i]);
        mpfr_init(l[i]);
    }
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
    mpfr_init(chA);
    mpfr_init(chB);
    mpfr_init(chC);
    

    x_max_prev = mpfr_get_ld(xPrev, MPFR_RNDA);
    
    bool done = 0;

    for(j = 1; !done; ++j) {

        in_alpha = j;

        cout << "testing the angle of " << j << " ...\t";

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

        mpfr_set_d(alpha, in_alpha++, MPFR_RNDU);

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
 
        
        //CALCULATION:

        for(int i = 1; mpfr_get_d(yPrev, MPFR_RNDA) >= 0; ++i) {

         for(int j = 0; j < 4; j++) {
            switch(j) {
             case 0:
                //chA = 0;
                mpfr_set_d(chA, 0, MPFR_RNDA);
                //chB = 0;
                mpfr_set_d(chB, 0, MPFR_RNDA);
                //chC = 0;
                mpfr_set_d(chC, 0, MPFR_RNDA);
                break;
             case 1:
             case 2:
                //chA = k[j-1] * dt / 2;
                mpfr_mul(t[0], k[j-1], dt, MPFR_RNDA);
                mpfr_div_si(chA, t[0], 2, MPFR_RNDA);
                //chB = dt / 2;
                mpfr_div_si(chB, dt, 2, MPFR_RNDA);
                //chC = dt * l[j-1] / 2;
                mpfr_mul(t[0], dt, l[j-1], MPFR_RNDA);
                mpfr_div_si(chC, t[0], 2, MPFR_RNDA);
                break;
             case 3:
                //chA = dt * k[2]
                mpfr_mul(chA, dt, k[2], MPFR_RNDA);
                //chB = dt
                mpfr_set(chB, dt, MPFR_RNDA);
                //chC = dt * l[2]
                mpfr_mul(chC, dt, l[2], MPFR_RNDA);
                break;
            }
             //k[j] = -k * (vxPrev + chA) * sqrt((vx + chA)^2 + (vy + chB)^2) / m;
             mpfr_add(t[0], vxPrev, chA, MPFR_RNDA);
             mpfr_add(t[1], vyPrev, chB, MPFR_RNDA);
             mpfr_pow_si(t[2], t[0], 2, MPFR_RNDA);
             mpfr_pow_si(t[3], t[1], 2, MPFR_RNDA);
             mpfr_add(t[4], t[2], t[3], MPFR_RNDA);
             mpfr_sqrt(t[5], t[4], MPFR_RNDA);
             mpfr_mul(t[6], t[0], t[5], MPFR_RNDA);
             mpfr_mul(t[7], k_neg, t[6], MPFR_RNDA);
             mpfr_div(k[j], t[7], m, MPFR_RNDA);
             //l[j] = -k * (vyPrev + chB) * sqrt((vx + chC)^2 + (vy + chB)^2) / m - g;
             mpfr_add(t[0], vxPrev, chC, MPFR_RNDA);
             mpfr_add(t[1], vyPrev, chB, MPFR_RNDA);
             mpfr_pow_si(t[2], t[0], 2, MPFR_RNDA);
             mpfr_pow_si(t[3], t[1], 2, MPFR_RNDA);
             mpfr_add(t[4], t[2], t[3], MPFR_RNDA);
             mpfr_sqrt(t[5], t[4], MPFR_RNDA);
             mpfr_mul(t[6], t[1], t[5], MPFR_RNDA);
             mpfr_mul(t[7], k_neg, t[6], MPFR_RNDA);
             mpfr_div(t[8], t[7], m, MPFR_RNDA);
             mpfr_sub(l[j], t[8], g, MPFR_RNDA);

             //cout << mpfr_get_ld(k[j], MPFR_RNDA) << " ";
             //cout << mpfr_get_ld(l[j], MPFR_RNDA) << " ";
         }
    
    //vxNext = vxPrev + (k[0] + 2k[1] + 2k[2] + k[3]) / 6 * dt
    //vxNext = vxPrev + ayNext * dt
         mpfr_mul_si(k[1], k[1], 2, MPFR_RNDA);
         mpfr_mul_si(k[2], k[2], 2, MPFR_RNDA);
         mpfr_add(t[0], k[0], k[1], MPFR_RNDA);
         mpfr_add(t[1], k[2], k[3], MPFR_RNDA);
         mpfr_add(t[2], t[1], t[0], MPFR_RNDA);
         mpfr_mul(t[3], t[2], dt, MPFR_RNDA);
         mpfr_div_si(t[4], t[3], 6, MPFR_RNDA);
         mpfr_add(vxNext, t[4], vxPrev, MPFR_RNDA);
         
   
    //vyNext = vyPrev + (l[0] + 2l[1] + 2l[2] + l[3]) / 6 * dt
    //vyNext = vyPrev + ayNext * dt
         mpfr_mul_si(l[1], l[1], 2, MPFR_RNDA);
         mpfr_mul_si(l[2], l[2], 2, MPFR_RNDA);
         mpfr_add(t[0], l[0], l[1], MPFR_RNDA);
         mpfr_add(t[1], l[2], l[3], MPFR_RNDA);
         mpfr_add(t[2], t[1], t[0], MPFR_RNDA);
         mpfr_mul(t[3], t[2], dt, MPFR_RNDA);
         mpfr_div_si(t[4], t[3], 6, MPFR_RNDA);
         mpfr_add(vyNext, t[4], vyPrev, MPFR_RNDA);

//!!!!!!!!!!!!

            mpfr_add(t[0], vxPrev, vxNext, MPFR_RNDZ);
            mpfr_mul(t[1], t[0], dt, MPFR_RNDZ);
            mpfr_div_ui(t[2], t[1], 2, MPFR_RNDZ);
            mpfr_add(xNext, xPrev, t[2], MPFR_RNDZ);
            
            mpfr_add(t[0], vyPrev, vyNext, MPFR_RNDZ);
            mpfr_mul(t[1], t[0], dt, MPFR_RNDZ);
            mpfr_div_ui(t[2], t[1], 2, MPFR_RNDZ);
            mpfr_add(yNext, yPrev, t[2], MPFR_RNDZ);
            
            
            //OFFTOP:

            x_max_curr = mpfr_get_ld(xPrev, MPFR_RNDA);

            //CHANGING VALUES
            
            mpfr_set(xPrev, xNext, MPFR_RNDA);
            mpfr_set(yPrev, yNext, MPFR_RNDA);
            mpfr_set(vxPrev, vxNext, MPFR_RNDA);
            mpfr_set(vyPrev, vyNext, MPFR_RNDA);
            mpfr_set(axPrev, axNext, MPFR_RNDA);
            mpfr_set(ayPrev, ayNext, MPFR_RNDA);

        }

        if(x_max_prev > x_max_curr) {
            x_max_MAX = x_max_prev;
            done = true;
        }
        
        cout << x_max_curr << "\n";
        x_max_prev = x_max_curr;
    }
}
