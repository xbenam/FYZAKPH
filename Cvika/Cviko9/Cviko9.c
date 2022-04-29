#include <math.h>
#include <stdio.h>
#include <stdlib.h>

FILE *fa;

float gravity = -9.81;
float mass = 80;
float S = 2;
float mol = 0.029;
float ro = 101325;
float R = 8.314;
float C = 0.5;
float height = 10000;
float speed = 0.1;
float h = 1;
float k0 = 0; 


float fFunction (float time){
    return gravity * time *  k0;
}

int main (int argc , char ** argv )
{
    k0 = 0.5 * ((C * S * ro) / mass);
    fa = fopen("data.dat", "w");
    

    
    int time = 1;
    while (height >= 0) {
        fprintf(fa,"%d %f\n",time, height);
        height = height + h * fFunction(time);
        time ++;
    }

    fclose(fa);
    return 0;

}
