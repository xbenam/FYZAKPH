#include <stdio.h>
#include <math.h>
#include <GL/glut.h>




float resize = 1;
const float gzry = 9.80665; //m/sˆ2
const float dt = 0.01; //s
const float PI = 3.1415;


static int count = 0;
void writeToFile(float  h0, float t, float v, float alpha, float x, float y, float vx, float vy){  // funkcia na pisanie dat do suboru .dat
    FILE * fp;

    if(count == 0) {
        fp = fopen("t_x_y_vx_vy_v.dat", "w");
        fprintf (fp, "# Zvolene hodnoty h0 = %.f, v0 = %.f m/s a alpha0 = %.f°\n",h0, v, alpha);
        fprintf (fp, "# t[s]   x[m]   y[m]    vx[m/s]   vy[m/s]   v[m/s]\n");
        count++;
    }
    else {
        fp = fopen("t_x_y_vx_vy_v.dat", "a");
    }

    fprintf (fp, "%.2f     %.2f   %.2f    %.2f    %.2f      %.2f\n",t, x, y, vx, vy, v);
    fclose(fp);
}



float t = 0;
float alpha0, v0, x, y, vx0, vy0, vx, vy, h0, cas_letu;

float vyratajVysku (){                      // ratam z akej pozicie veze bude gula vystrelena na zaklade zadanej vysky
    // 10 (vyska) = -2.0
    // 5 (napriklad) = ?
    float vyska = -((-2.0 / 10) * h0 + 2.0) + 0.05;
    return vyska;
}

float skaluj (float param) {
    float out = (param/5);
    return out;
}

float vyratajCasLetu (float a, float b, float c){   // funkcia na vyratanie celkovej hodnoty casu letu

    float discriminant, r1, r2;
    discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        r1 = (-b + sqrt(discriminant)) / (2 * a);
        r2 = (-b - sqrt(discriminant)) / (2 * a);
    }

    else if (discriminant==0) {
        r1 = r2 = -b / (2 * a);
    }

    //printf ("Korene %f %f \n",r1, r2);

    if (r1>0){
        return r1;
    }
    else {
        return r2;
    }
}



float Lmax = 8.0;                              //velkost plochy v smere osi
const int icaskrok = 10;                        // cas v milisekundach
float posunX = 0.0;
float posunY = 0.0;
float sirkaX, vyskaY;






void vyrataj (const int ihod){                  // ratam drahu strely a zapisujem do suboru

    if (x<sirkaX){                          // ratam cas letu a nie vysky, aby som mohol vystrelit gulu aj zo zeme alebo spodurovne zeme
        writeToFile(h0, t, v0, alpha0, x, y, vx, vy);
        t += 0.01;
        vx = vx0;   //konst
        x = vx0*t;

        vy = vy0 - gzry*t;
        y = h0 + vy0*t - 0.5*gzry*pow(t,2.0);


        posunX = skaluj(x);
        posunY = skaluj(y - h0);

    } else {
        if (y < 0) {
            printf("Gula nikdy nevyleti na uroven zeme!\n");
            return;
        } else {
            printf("[Done]");
            return;
        }
    }
    glutPostRedisplay();                        // Prekreslujeme scenu
    glutTimerFunc(icaskrok, vyrataj, ihod);

}


void manageResize (int width, int height) {     // udrzuje mi pomer obrazkov k velkosti okna
    glViewport(0,0,width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width == 0) width++;
    if (sirkaX == 0) sirkaX++;

    float down = 0;
    if (h0<0) down = skaluj(h0);                // ak je vyska odkial vyleti gula zaporna, posuniem obraz dole


    float sideRatio = ((float)(height+vyskaY)/(width+sirkaX));
    gluOrtho2D(-4, 4+skaluj(sirkaX), -4*sideRatio, 4*sideRatio+skaluj(vyskaY));


}

const int steps = 60;
void nakresliScenu (){
    float angle = 3.1415 * 2.f / steps;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float uprava_vyska = vyratajVysku();    // doratavam z akej pozicie veze bude akoze gula vystrelena
    //printf ("Uprava h = %.2f", uprava_vyska);


    //obdlznik - veza
    glColor3f(0.5, 0.35, 0.05);
    glRectf(-3.0f, 0.0f, -2.5f, -2.0f);


    // trojuholnik - strecha
    glBegin(GL_TRIANGLES);
    glColor3f(255, 0, 0);
    glVertex2f(-3.1f,0.0f);
    glVertex2f(-2.4f,0.0f);
    glVertex2f(-2.75f,0.3f);
    glEnd();


    // gula - kamen
    float Xpos = 0, Ypos = 0, radius = 0.05f;
    glClearColor(1.0,1.0,1.0,0);

    float prevX = Xpos;
    float prevY = Ypos - radius;

    for (int i=0; i<=steps; i++){
        float newX = radius * sin(angle*i);
        float newY = -radius * cos(angle*i);
        glBegin(GL_TRIANGLES);
        glColor3f(0,0,0);

        glVertex3f(-2.5f+posunX,0.0f+uprava_vyska+skaluj(radius)+posunY,0.0f);
        glVertex3f(prevX-2.5f+posunX, prevY+uprava_vyska+skaluj(radius)+posunY, 0.0f);
        glVertex3f(newX-2.5f+posunX, newY+uprava_vyska+skaluj(radius)+posunY, 0.0f);
        glEnd();

        prevX = newX;
        prevY = newY;
    }
    glutSwapBuffers();
}





int main (int argc, char **argv) {

    printf ("Zadaj pociatocnu vysku: ");
    scanf ("%f", &h0);
    printf ("Zadaj pociatocnu rychlost (idealne 0 - 60): ");
    scanf ("%f", &v0);
    printf ("Zadaj uhol: ");
    scanf ("%f", &alpha0);

    vx0 = v0*cos(alpha0*PI/180);
    vy0 = v0*sin(alpha0*PI/180);

    vx = vx0;   //konst
    x = vx0*t;

    vy = vy0 - gzry*t;
    y = h0 + vy0*t - 0.5*gzry*pow(t,2.0);


    cas_letu = vyratajCasLetu((-0.5*gzry),vy0, h0)-dt;

    float h;
    if (h0<0) {                                 // cas letu gule pri zapornej h0 ratam ako rozdiel casu letu, kolko by letel z vysky 0
        h = 0.00001;                            // minus cas letu gule nez vyleti zo zapornej vysky po vysku 0 (vystrelena zospodu do zeme)
        float cas = vyratajCasLetu((-0.5*gzry),vy0, h)-dt;
        cas_letu = cas - cas_letu;
    }
    printf ("Cas letu: %.2f\n", cas_letu);



    sirkaX = cas_letu*vx;                         // upravujem velkost okna na dlzku dopadu
    printf ("Max dlzka: %f\n",sirkaX);

    float casY = vy0/gzry;
    vyskaY = h0+vy0*casY-0.5*gzry*pow(casY,2.0);  // upravujem velkost okna na sirku dopadu
    printf ("Max vyska: %f\n",vyskaY);\
    //printf ("%.2f\n",skalujSpat(dlzkaX));
    float scale = ((float)(800)/(500));   // skalujem velkost okna na rozsah trajektorie



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    //glutInitWindowSize(800+skalujSpat(sirkaX),500+skalujSpat(vyskaY));
    glutInitWindowSize(800,500);
    glutInitWindowPosition(200,100);
    glutCreateWindow("Hello, world of physics!");

    glutDisplayFunc(nakresliScenu);
    glClearColor(0.9,0.9,0.9,0);
    glutReshapeFunc(manageResize);

    glutTimerFunc(icaskrok, vyrataj, 1);
    glutMainLoop();

    return 0;
}
