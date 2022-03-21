#include <GL/gl.h>
#include <GL/glut.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

FILE *fa;
const int icaskrok = 25; // v milisekundach
const float Lmax = 20.0; // rozmer sceny v smere X
float posunX = 0.0; // zaciatocna hodnota posuvu (uvazujme len v smere X)
float posunX1 = 0.0; // zaciatocna hodnota posuvu (uvazujme len v smere X)
float posunX2 = 0.0; // zaciatocna hodnota posuvu (uvazujme len v smere X)
float v1 = 0.0; 
float v2 = 0.0;
float deltaTime = 0.0;
int smer = 1;
int smer1 = 1;
int smer2 = 1;


void aktualizuj (const int ihod )
{
   
    fprintf(fa,"%d %f %f\n",ihod, posunX1, posunX2);
    posunX1 += v1 * smer1;
    if (ihod >= deltaTime) 
        posunX2 += v2 * smer2;
    glutPostRedisplay ();
    // if (posunX1 - distance / 2 <= -0.5 * Lmax || posunX1 - distance / 2 >= 0.5 * Lmax)
    //     smer1 *= -1;
    // if (posunX2 + distance / 2 <= -0.5 * Lmax || posunX2 + distance / 2 >= 0.5 * Lmax)
    //     smer2 *= -1;
    glutTimerFunc ( icaskrok , aktualizuj , ihod +1);
    

}
void obsluhaResize (int sirka , int vyska )
{

    glViewport (0, 0, sirka , vyska );
    glMatrixMode ( GL_PROJECTION );
    //glLoadIdentity();
    if ( sirka == 0) sirka ++;
    const float pomstr = ((float) vyska )/ sirka ;
    //gluOrtho2D(-0.5*Lmax, 0.5*Lmax, -0.5*Lmax*pomstr , 0.5*Lmax*pomstr);
    //=================================================================
    // Namiesto zakomentovanych riadov s glLoadIdentity a gluOrtho2D
    // je ekvivalentne pouzit glLoadMatrixf so spravne pripravenou
    // maticou. Reset pomocou glLoadIdentity nie je potrebny , lebo
    // my tu maticu prikazom glLoadMatrixf rovno nastavime na taku,
    // aka je potrebna.
    // Ta matica len preskaluje suradnice sceny tak, aby boli
    // z intervalov <-1, 1>.
    //=================================================================
    float mat [16]; // Maticu treba ulozit do 1D pola po stlpcoch ,

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

void kresliTrojuh2D ()
{

    glClear ( GL_COLOR_BUFFER_BIT );
    glColor3f (0.0 , 1.0 , 0.0);
    glMatrixMode ( GL_MODELVIEW ); // dobre , ale nie je nutne to tu volat
    glLoadIdentity ();

    glTranslatef ( posunX1 , 0.0 , 0.0);
    glBegin ( GL_LINES );
    glVertex2f ( -0.5 * Lmax, -0.05* Lmax );
    glVertex2f ( -0.5 * Lmax, -0.04* Lmax );
    glEnd ();

    glLoadIdentity();

    glColor3f (0.0 , 1.0 , 1.0);
    glTranslatef ( posunX2 , 0.0 , 0.0);
    glBegin ( GL_LINES );
    glVertex2f ( -0.5 * Lmax, -0.05* Lmax );
    glVertex2f ( -0.5 * Lmax, -0.04* Lmax );
    glEnd ();

    glutSwapBuffers ();

}

int main (int argc , char ** argv )
{
    
    fa = fopen("data.dat", "w");
    glutInit (& argc , argv );

    v1 = atof(argv[1]);
    v2 = atof (argv[2]);
    deltaTime  = atof (argv[3]);
    
    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (1080 , 640);
    glutInitWindowPosition (200 , 150);
    glutCreateWindow (" OpenGL : Du 2.2 ");
    glutDisplayFunc ( kresliTrojuh2D );
    glClearColor (0.0 , 0.0 , 0.0 , 0.0);
    glutReshapeFunc ( obsluhaResize );
    glutTimerFunc ( icaskrok , aktualizuj , 0);
    glutMainLoop ();
    fclose(fa);
    return 0;

}