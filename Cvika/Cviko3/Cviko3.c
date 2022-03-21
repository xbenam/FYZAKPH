#include <GL/gl.h>
#include <GL/glut.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

FILE *fa;
const int icaskrok =100;
const float Lmax = 20.0;
float posunY1 = -8.0;
float posunY2 = 0.0;
float v1 = 0.0;
float v2 = 0.0;
float distance = 0.0;
int smer1 = 1;
int smer2 = -1;
float gravity = -9.81;


void aktualizuj (const int ihod )
{
    glutPostRedisplay ();
    if(posunY1 > -8)
    {
        posunY1 = posunY1 + ((v1 * ihod) + (0.5 * gravity* pow(ihod, 2))) * 0.001;
    }
    else{
        posunY1 = -9.90;}

    if (ihod >= 13 && posunY2 > -9.90)
    {
        posunY2 = posunY2 + (0.5 * gravity* pow(ihod - 12, 2)) * 0.001;
    }
    if (posunY2 < -9.90)
        posunY2 = -9.91;
    fprintf(fa,"%d %f %f\n",ihod, posunY1, posunY2);
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

void zavod ()
{

    glClear ( GL_COLOR_BUFFER_BIT );
    glColor3f (0.0 , 1.0 , 0.0);
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();

    glPointSize(5);

    glTranslatef ( 0.0, posunY1, 0.0);

    glBegin ( GL_POINTS );
    glVertex2f (0.0, 0.0);
    glEnd ();

    glLoadIdentity();

    glColor3f (0.0 , 1.0 , 1.0);
    glTranslatef ( 0.0 , posunY2 , 0.0);

    glBegin ( GL_POINTS );
    glVertex2f (0.0, 0.0);
    glEnd ();

    glutSwapBuffers ();

}

int main (int argc , char ** argv )
{
    fa = fopen("data.dat", "w");
    glutInit (& argc , argv );

    v1 = atof(argv[1]);
    distance  = atof (argv[2]);

    posunY1 += distance;
    posunY2 += distance;

    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (640 , 640);
    glutInitWindowPosition (200 , 50);
    glutCreateWindow (" OpenGL : Du 2.2 ");
    glutDisplayFunc ( zavod );
    glClearColor (0.0 , 0.0 , 0.0 , 0.0);
    glutReshapeFunc ( obsluhaResize );
    glutTimerFunc ( icaskrok , aktualizuj , 0);
    glutMainLoop ();
    fclose(fa);
    return 0;

}