#ifndef _AMBER_DEFS_H
#define _AMBER_DEFS_H

// A struct to hold mouse data
typedef struct 
 {  
  int Mx,My;
  bool Mleft, Mright;
 }MouseInfo;

// Values that are needed throughout the code.
typedef struct
 {
  int ScrX,ScrY;
  float ScrRatio;
 }AppGlobalVars;

#endif