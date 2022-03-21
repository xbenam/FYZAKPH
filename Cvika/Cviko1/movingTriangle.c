#include <GL/gl.h>
#include <GL/glut.h>

const int icaskrok = 25; // v milisekundach
const float Lmax = 20.0; // rozmer sceny v smere X
float posunX = 0.0; // zaciatocna hodnota posuvu (uvazujme len v smere X)
int smer = 1;

void aktualizuj (const int ihod )
{

    posunX += 0.05 * smer;
    glutPostRedisplay ();
    if (posunX >= 0.4 * Lmax || posunX <= -0.4 * Lmax)
        smer *= -1;
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
    glColor3f (0.0 , 0.0 , 1.0);
    glMatrixMode ( GL_MODELVIEW ); // dobre , ale nie je nutne to tu volat
    //glMatrixMode(GL_PROJECTION); // zle
    //----------------------------------------------------------------------
    // Tu (vdaka volbe GL_MODELVIEW) nam glLoadIdentity resetuje maticu
    // robiacu ten PREPOCET SURADNIC objektov na scene , ktory je potrebny
    // LEN KVOLI POHYBU TYCH OBJEKTOV.
    // Tato matica teda nema nic spolocne s projekciou a vyjadruje len zmeny
    // suradnic objektov , pretoze tie objekty sa nejako hybu.
    //
    // Po resete treba pravdaze maticu naplnit potrebnymi cislami.
    // O to sa tu postara procedura glTranslatef.
    //----------------------------------------------------------------------
    glLoadIdentity ();
    //-------------------------------------------------------------------
    // Pomocou glTranslatef() posuvame cely trojuholnok jedinym prikazom.
    // Je to tak efektivnejsie. Ale bolo vhodne najprv ukazat aj ten
    // elementarny neefektivny sposob.
    //-------------------------------------------------------------------
    glTranslatef ( posunX , 0.0 , 0.0);
    glBegin ( GL_TRIANGLES );

    glVertex2f ( -0.1* Lmax , -0.1* Lmax );
    glVertex2f ( 0.1* Lmax , -0.1* Lmax );
    glVertex2f ( 0.0* Lmax , 0.1* Lmax );

    glEnd ();
    glutSwapBuffers ();

}
int main (int argc , char ** argv )
{

    glutInit (& argc , argv );

    // glutInitDisplayMode(GLUT_RGBA); // nie je nutne pisat

    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (1080 , 640);
    glutInitWindowPosition (200 , 150);
    glutCreateWindow (" OpenGL : trojuholnik ");
    glutDisplayFunc ( kresliTrojuh2D );
    glClearColor (0.8 , 0.3 , 0.3 , 0.3);
    glutReshapeFunc ( obsluhaResize );
    glutTimerFunc ( icaskrok , aktualizuj , 0);
    glutMainLoop ();
    return 0;

}