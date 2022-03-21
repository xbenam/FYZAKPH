#include <GL/gl.h>
#include <GL/glut.h>

#include <math.h>
#include <stdio.h>

const int icaskrok = 25; // v milisekundach
const float Lmax = 20.0; // rozmer sceny v smere X
float posunX1 = 0.0; 
float posunX2 = 0.0;
float v1 = 0.0; 
float v2 = 0.0;
float distance = 0.0;
int smer1 = 1;
int smer2 = -1;


void aktualizuj (const int ihod )
{

    posunX1 += v1 / 100 * smer1;
    posunX2 += v2 / 100 * smer2;
    glutPostRedisplay ();
    if (posunX1 - distance / 2 <= -0.5 * Lmax || posunX1 - distance / 2 >= 0.5 * Lmax)
        smer1 *= -1;
    if (posunX2 + distance / 2 <= -0.5 * Lmax || posunX2 + distance / 2 >= 0.5 * Lmax)
        smer2 *= -1;
    glutTimerFunc ( icaskrok , aktualizuj , ihod +1);


}
void obsluhaResize (int sirka , int vyska )
{

    glViewport (0, 0, sirka , vyska );
    glMatrixMode ( GL_PROJECTION );
    if ( sirka == 0) sirka ++;
    const float pomstr = ((float) vyska )/ sirka ;
    float mat [16]; 
    for (int ii = 0; ii < 16; ii ++) mat [ii] = 0.0;
    mat [0] = 2.0/ Lmax ; // skalovaci faktor pre x-ove suradnice
    mat [5] = 2.0/( Lmax * pomstr ); // pre y-ove
    mat [10] = 1.0; // pre z-ove
    mat [15] = 1.0; // ta pridavna jednotka , pravy dolny prvok matice
    glLoadMatrixf ( mat );

}

void dvaBody ()
{

    glClear ( GL_COLOR_BUFFER_BIT );
    glColor3f (0.0 , 1.0 , 0.0);
    glMatrixMode ( GL_MODELVIEW ); // dobre , ale nie je nutne to tu volat
    glLoadIdentity ();

    glTranslatef ( posunX1 , 0.0 , 0.0);
    glBegin ( GL_LINES );
    glVertex2f ( -distance / 2, -0.05* Lmax );
    glVertex2f ( -distance / 2, 0.05* Lmax );
    glEnd ();

    glLoadIdentity();

    glColor3f (0.0 , 1.0 , 1.0);
    glTranslatef ( posunX2 , 0.0 , 0.0);
    glBegin ( GL_LINES );
    glVertex2f (  distance / 2, -0.05* Lmax );
    glVertex2f (  distance / 2, 0.1* Lmax );
    glEnd ();

    glutSwapBuffers ();

}

int main (int argc , char ** argv )
{

    glutInit (& argc , argv );

    v1 = atof(argv[1]);
    v2 = atof (argv[2]);
    distance  = atof (argv[3]);

    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (800 , 480);
    glutInitWindowPosition (200 , 50);
    glutCreateWindow (" OpenGL : Du 2.1 ");
    glutDisplayFunc ( dvaBody );
    glClearColor (0.0 , 0.0 , 0.0 , 0.0);
    glutReshapeFunc ( obsluhaResize );
    glutTimerFunc ( icaskrok , aktualizuj , 0);
    glutMainLoop ();
    return 0;

}