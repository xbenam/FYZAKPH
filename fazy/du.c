#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const float Lmax = 40.0;
const double zuY = 85.0; // zorny uhol pozdlz Y
const double dnear = 10.0;
const double dfar = 20.0;

void obsluhaResize (int sirka , int vyska )
{

    glViewport (0, 0, sirka , vyska );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    if ( vyska == 0) vyska ++;
    const float sir_ku_vys = ((float) sirka )/ vyska ;
    //---------------------------------------------------------------------
    // gluPerspective(fovY, sir_ku_vys , dnear , dfar);
    //
    // fovY = field of view along Y = sirka uhla pohladu pozdlz Y
    // (zorny uhol pozdlz Y)
    // dnear = vzdialenost pozorovatela od blizsej orezavacej roviny
    // dfar = vzdialenost pozorovatela od vzdialenejsej orezavacej roviny
    // Musi teda byt 0 < dnear < dfar.
    // Pozorovatel vidi len svet zo ZAPORNYMI Z-ovymi suradnicami.
    // Preto zn = -dnear , zf = -dfar (suradnice tych rovin).
    //
    // gluPerspective nasobi doterajsiu transformacnu maticu , cize
    // transformacna matica sa zmeni. Aby sme tym nedostali nejaku
    // neziaducu transformaciu , ale len perspektivnu projekciu , bolo vyssie
    // treba spravit reset pomocou glLoadIdentity.
    //
    // Nasobenim matice a vektora sa vsak neda spravit pozadovana
    // transformacia (zobrazenie , ,mapping ') uplne. (Neda sa spravit delenie
    // hodnotou Z.) Ale OpenGL stroj sa kdesi pre nas neviditelne postara
    // o to, aby za to delenie spravilo.
    //---------------------------------------------------------------------
    gluPerspective (zuY , sir_ku_vys , dnear , dfar );

}
void kresliRovnobezky3D ()
{

    glClear ( GL_COLOR_BUFFER_BIT );

    // glClear(GL_DEPTH_BUFFER_BIT); // nie je nutne nastavit

    glMatrixMode ( GL_MODELVIEW );
    glColor3f (0.0 , 0.0 , 0.0);
    //------------------------------------------------------------------
    // Ked stojime na kolajniciach a pozerame ponad ne do dialky , tak sa
    // zda, ze sa zbiehaju. Moze za to perspektivna projekcia.
    // Ciary vytvorene v tomto programe su presne take: su rovnobezne ,
    // ale zda sa, ze sa zbiehaju.
    //
    // Ak by sme tie ciary pre zmenu nakreslili vyssie (y = +0.2*Lmax),
    // tak by sa zasa zdalo , ze su to trolejove vedenia nad nami
    // a ze sa tiez zbiehaju (poznamenal Peter Bokes).
    //------------------------------------------------------------------
    glBegin ( GL_LINES );

    glVertex3f ( -0.1* Lmax , -0.2* Lmax , -10.0);
    glVertex3f ( -0.1* Lmax , -0.2* Lmax , -12.0);
    glVertex3f (+0.1* Lmax , -0.2* Lmax , -10.0);
    glVertex3f (+0.1* Lmax , -0.2* Lmax , -12.0);

    glEnd ();
    glutSwapBuffers ();

}
int main (int argc , char ** argv )
{

    glutInit (& argc , argv );

    // glutInitDisplayMode(GLUT_RGBA); // nie je nutne pisat

    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (640 , 640);
    glutInitWindowPosition (200 , 150);
    glutCreateWindow (" OpenGL : rovnobezky ");
    glutDisplayFunc ( kresliRovnobezky3D );
    glutReshapeFunc ( obsluhaResize );
    glClearColor (1.0 , 1.0 , 1.0 , 0); // farba pozadia biela
    glutMainLoop ();
    return 0;

}