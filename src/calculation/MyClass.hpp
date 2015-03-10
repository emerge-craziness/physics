#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <NTL/RR.h>
#include <string>

class MyClass {
  public:
  static int calculation();
  static int getdata (int mode);
  static int printdata();
  static int initializeDefaults();
  static int setFilenames (std::string fValues, std::string fHeader, std::string fInfo);
  static int clearFiles();

  private:
  static int calculateWriteToTheFile();
  static int calculateWriteToTheHeader();
  static int calculateAccelerations();
  static int calculateChangeValues();
  static int calculateCoordinates();
  static int calculateOfftop();
  static int calculateSpeeds();


  /* Declaring variables */
  static NTL::RR axNext, 
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
                 v_max,
                 k[4],
                 l[4];
  static int     calculated_points;
  static std::string  filenameValues,
                      filenameHeader,
                      filenameInfo;
};
  
#endif // MYCLASS_HPP
