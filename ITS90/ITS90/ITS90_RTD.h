#pragma once


typedef struct ITS90_Cal_s
{
    float Water_Res;
    float a,b,c,d;// 0 to 961.78
   
} ITS90_Cal_t;

/* float Argon;    // 83.8058C
    float Mercury;  // 234.3156C
    float Water;    // 0.01C
    float Gallium;  // 29.7646C
    float Indium;   // 156.5985C
    float Tin;      // 231.928C
    float Zinc;     // 419.527C
    float Aluminium;// 660.323C
    float Silver;   // 961.78C
    */
   
float ITS90_GetRes(ITS90_Cal_t *cal, float temperature);
float ITS90_GetTemp(ITS90_Cal_t *cal, float resistance);
