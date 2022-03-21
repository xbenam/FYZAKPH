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
FILE *fa;
const int icaskrok = 25;
const float Lmax = 2000.0;

float x = -980.0;
float y = -980.0;

float tx = 980.0;
float ty = -980.0;

float alfa = 0.0;
float velocity = 0.0;
float speed = 0.0;

float gravity = -9.81;


void aktualizuj (const int ihod )
{
    glutPostRedisplay ();
    // printf("%f\n",y);
    if(y < -990.0) y = -990;
    if(y >= -989.0){
        printf("%f \n", y);
        x += cos(alfa * 3.14 / 180) * velocity;
        y +=  sin(alfa * 3.14 / 180) * velocity + 0.5 * gravity * ihod;
    }
    if(tx >= -980)
        tx -= speed;
    fprintf(fa,"%f %f\n", x, y);
    glutTimerFunc ( icaskrok , aktualizuj , ihod +1);
}

void obsluhaResize (int sirka , int vyska )
{

    glViewport (0, 0, sirka , vyska );
    glMatrixMode ( GL_PROJECTION );
    //glLoadIdentity();
    if ( sirka == 0) sirka ++;
    const float pomstr = ((float) vyska )/ sirka ;
    float mat [16];

    // Tu je symetricka , takze aj po riadkoch by bolo OK.
    //---------------------------------------------------------------
    // Naplnime skalovaciu maticu hodnotami.
    // Spravne po slovensky by sa asi mala nazyvat MERITKOVA MATICA.
    //---------------------------------------------------------------
    for (int ii = 0; ii < 16; ii ++) mat [ii] = 0.0;
    mat [0] = 2.0/ Lmax ; // skalovaci faktor pre x-ove suradnice
    mat [5] = 2.0/( Lmax * pomstr ); // pre y-ove
    mat [10] = 1.0; // pre z-ove
    mat [15] = 1.0; // ta pridavna jednotka , pravy dolny prvok matice
    //--------------------------------
    // Posleme maticu OpenGL ,stroju '.
    //--------------------------------
    glLoadMatrixf ( mat );

}

void strela ()
{
    glClear ( GL_COLOR_BUFFER_BIT );

    glColor3f (0.0 , 1.0 , 0.0);
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();

    glPointSize(5);

    glTranslatef ( x, y, 0.0);

    glBegin ( GL_POINTS );
    glVertex2f (0.0, 0.0);
    glEnd ();

    glLoadIdentity ();
    glTranslatef ( tx, ty, 0.0);

    glColor3f (0.0 , 0.0 , 1.0);
    glBegin ( GL_POINTS );
    glVertex2f (0.0, 0.0);
    glEnd ();

    glutSwapBuffers ();

}

int main (int argc , char ** argv )
{
    fa = fopen("data.dat", "w");

    alfa = atof(argv[1]);
    velocity = atof(argv[2]);
    speed = atof(argv[3]);
    glutInit (& argc , argv );

    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (640 , 640);
    glutInitWindowPosition (200 , 50);
    glutCreateWindow (" OpenGL : CV4 ");
    glutDisplayFunc ( strela );
    glClearColor (0.0 , 0.0 , 0.0 , 0.0);
    glutReshapeFunc ( obsluhaResize );
    glutTimerFunc ( icaskrok , aktualizuj , 1);
    glutMainLoop ();
    fclose(fa);
    return 0;

}