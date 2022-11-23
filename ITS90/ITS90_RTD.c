#include <ITS90/ITS90.h>
#include <math.h>

// W(T90) = R(T90) / R(273.16K)
// basically current reading / reading at 0C



static float A[] = {-2.13534729,3.18324720,-1.80143597,0.71727204,0.50344027,-0.61899395,-0.05332322,0.28021362,0.10715224,-0.29302865,0.04459872,0.11868632,-0.05248134};
static float B[] = {0.183324722,0.240975303,0.209108771,0.190439972,0.142648498,0.077993465,0.012475611,-0.032267127,-0.075291522,-0.056470670,0.076201285,0.123893204,-0.029201193,-0.091173542,0.001317696,0.026025526};
static float C[] = {2.78157254,1.64650916,-0.13714390,-0.00649767,-0.00234444,0.00511868,0.00187982,-0.00204472,-0.00046122,0.00045724};
static float D[] = {439.93285400,472.41802000,37.68449400,7.47201800,2.92082800,0.00518400,-0.96386400,-0.18873200,0.19120300,0.04902500};

// T_90 is temperature in kelvin
static float W_r(float T_90)
{
    float res=0;
    if (T_90>273.16)
    {
        res = C[0];
        for (int i=1;i<10;i++)
        {
            res += C[i]*pow((T_90-754.15)/481.0,i); 
        }
    }
    else
    {
        res=A[0];
        for (int i=1;i<13;i++)
        {
            res += A[i]*pow((log(T_90/273.16)+1.5)/1.5,i);
        }
    }
    return res;
}

static float T_90(float W_r)
{
    float Temp=0;
    if (W_r>=1)
    {
        Temp = 273.15+D[0];
        for (int i=1;i<10;i++)
        {
            Temp += D[i]*pow((W_r - 2.64)/1.64,i);
        }
    }
    else
    {
        Temp = 273.16+B[0];
        for (int i=1;i<16;i++)
        {
            Temp += B[i] * pow(((pow(W_r,1.0/0.66)   - 0.65)/0.35),i);
        }
    }
}

static float delta_W(ITS90_Cal_t *cal, float W)
{
    // >1 means above 0C, 
    // >
    float delta=0;
    float a=0,b=0,c=0,d=0;
    if (W>=1)
    {
        delta = cal->a*(W-1);
        if (cal->b!=0) 
        delta += cal->b*pow(W-1,2);
        if (cal->c!=0)
        delta  += cal->c*pow(W-1,3);
        if (cal->d!=0)
        delta += cal->d*(W - W_r(933.473));
    }
    else
    {
        delta = cal->a*(W-1) + cal->b*(W-1)*log(W);
    }
}

float ITS90_RTD_GetRes(ITS90_Cal_t *cal, float temperature)
{
    float Wr = W_r(temperature+273.15);
    float delta_w = delta_W(cal,Wr);
    float W = Wr + delta_w;
    return W*cal->Water_Res;
}


float ITS90_RTD_GetTemp(ITS90_Cal_t *cal, float resistance)
{
    float p = resistance/cal->Water_Res;
    float W_r_T_90 = p - delta_W(cal,p);
    return T_90(W_r_T_90);
}
