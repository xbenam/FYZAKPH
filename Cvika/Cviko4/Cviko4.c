#include <GL/gl.h>
#include <GL/glut.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>




/*
* sikmy vrh
* v(r) = r (x(t), y(t))
* x(t) = V0x * t                    Vx(t) = V0x
* y(t) = V0y * t - 1/2 * g * t^2    Vy(t) = V0y - g * t
* 
* V0x = V0 * cos(alfa)
* V0y = V0 * sin(alfa)
* 
* y(t1) = 0 t1(V01 - 1/2 g * t1) = 0 
*                     ||
*                      V
*                      x => t1 = (2 * V0 * sin(alfa)) / g
* x(t1) = d = (2 * V0^2 * cos(alfa) * sin(alfa)) / g
*/
const int icaskrok = 1000/60;
float Lmax = 2000.0;

float x = 0.0;
float y = 0.1;

float tx = 0.0;
float ty = 0.0;

float alfa = 0.0;
float velocity = 0.0;
float speed = 0.0;

float gravity = -9.81;

int delay = 1;

float maxY = -9999.0;
float maxX = -9999.0;


void aktualizuj (const int ihod )
{
    if(ihod==1)Sleep(500);
    glutPostRedisplay ();
    
    if(y < -(maxY / 2) + 5) {y = -(maxY / 2) + 5;return;}
    if(y > -(maxY / 2) + 5){
        x += (cos(alfa * 3.14 / 180) * velocity);
        y += (sin(alfa * 3.14 / 180) * velocity + 0.5 * gravity * ihod);
    }
    if(tx >= -(maxX / 2))
            tx -= speed;
    
    glutTimerFunc ( icaskrok , aktualizuj , ihod +1);
}

void obsluhaResize (int sirka , int vyska )
{

    glViewport (0, 0, sirka , vyska );
    glMatrixMode ( GL_PROJECTION );
    //glLoadIdentity();
    if ( sirka == 0) sirka ++;
    // const float pomstr = ((float) vyska )/ sirka ;
    // float mat [16];
    gluOrtho2D(-0.5*maxX, 5*maxX, -0.5*maxY, 0.5*maxY);
    // // Tu je symetricka , takze aj po riadkoch by bolo OK.
    // //---------------------------------------------------------------
    // // Naplnime skalovaciu maticu hodnotami.
    // // Spravne po slovensky by sa asi mala nazyvat MERITKOVA MATICA.
    // //---------------------------------------------------------------
    // for (int ii = 0; ii < 16; ii ++) mat [ii] = 0.0;
    // mat [0] = 2.0 / (maxX*1.1) ; // skalovaci faktor pre x-ove suradnice
    // mat [5] = 2.0 / (maxY*1.1) ; // pre y-ove
    // mat [10] = 1.0; // pre z-ove
    // mat [15] = 1.0; // ta pridavna jednotka , pravy dolny prvok matice
    // //--------------------------------
    // // Posleme maticu OpenGL ,stroju '.
    // //--------------------------------
    // glLoadMatrixf ( mat );

}

void strela ()
{
    glClear ( GL_COLOR_BUFFER_BIT );

    glPointSize(5);
    glMatrixMode ( GL_MODELVIEW );

    glLoadIdentity ();    
    
    glColor3f (1.0 , 0.0 , 0.5);
    glTranslatef ( tx, ty, 0.0);
    glBegin ( GL_POINTS );
    glVertex2f (0.0, 0.0);
    glEnd ();

    glLoadIdentity ();
    glColor3f (0.0 , 1.0 , 0.0);

    glTranslatef ( x, y, 0.0);

    glBegin ( GL_POINTS );
    glVertex2f (0.0, 0.0);
    glEnd ();

    glutSwapBuffers ();
}

int main (int argc , char ** argv )
{
    alfa = atof(argv[1]);
    velocity = atof(argv[2]);
    speed = atof(argv[3]);

    int i = 1;

    maxY = -9999.0;
    maxX = -9999.0;

    while (y > 0){
        if(y < 0.0) y = 0;
        if(y > 0.0){
            x += cos(alfa * 3.14 / 180) * velocity;
            y += sin(alfa * 3.14 / 180) * velocity + 0.5 * gravity * (i);
            maxY = max(y, maxY);
            maxX = max(x, maxX);
        }
        i++;
    }
    float distance = maxX+1;
    float tankX;

    while(1)
    {
        tankX = distance;
        x = 0;
        y = 0.1;
        i = 1;
        while(y>0){
            x += cos(alfa * 3.14 / 180) * velocity;
            y += sin(alfa * 3.14 / 180) * velocity + 0.5 * gravity * (i);
            tankX -= speed;
            i++;
        }
        if((tankX - (x-1)) * (tankX - (x+1)) <= 0) break;
        distance++;
    }
    printf("d1: %f\nd2: %f", distance, maxX);
    
    maxX = distance;
    x = -(maxX / 2) + 5;
    y = -(maxY / 2) + 5.1;
    tx = (maxX / 2) - 5;
    ty = -(maxY / 2) + 5.1;

    glutInit (& argc , argv );
    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize(640,640);
    // if (maxX >= maxY)
    //     glutInitWindowSize (860 , 860*(maxY/maxX));
    // else
    //     glutInitWindowSize (640 * (maxX/maxY) , 640);
    
    glutInitWindowPosition (50 , 50);
    glutCreateWindow (" OpenGL : CV4 ");
    glutDisplayFunc ( strela );
    glClearColor (0.0 , 0.0 , 0.0 , 0.0);
    glutReshapeFunc ( obsluhaResize );
    glutTimerFunc ( icaskrok , aktualizuj , 1);
    glutMainLoop ();

    return 0;
}