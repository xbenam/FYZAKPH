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
const int icaskrok = 1000/30;
float Lmax = 2000.0;

float x = 0.0;
float y = 0.1;
float z = 0.0;

float tx = 0.0;
float ty = 0.0;

float alfa = 0.0;
float velocity = 0.0;
float speed = 0.0;
float platform = 0.0;

float gravity = -9.81;

int delay = 1;

float maxY = -9999.0;
float maxX = -9999.0;


void aktualizuj (const int ihod )
{
    if(ihod==1)Sleep(1500);
    glutPostRedisplay ();
    
    if(y < -(maxY / 2) + 2.5) {y = -(maxY / 2) + 2.5;return;}
    if(y > -(maxY / 2) + 2.5){
        x += (cos(alfa * 3.14 / 180) * velocity);
        y += (sin(alfa * 3.14 / 180) * velocity + 0.5 * gravity * ihod);
        z += 1;
    }
    if(tx >= -(maxX / 2))
            tx -= speed;
    
    glutTimerFunc ( icaskrok , aktualizuj , ihod +1);
}

void obsluhaResize (int sirka , int vyska )
{
    // const float ar = (float) sirka / (float) vyska; 
 
    glViewport(0, 0, sirka, vyska);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-maxX*1.1/2, maxX*1.1/2, -maxY*1.1/2, maxY*1.1/2, -100.0, 100.0); 
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    

}

void strela ()
{
    glClear ( GL_COLOR_BUFFER_BIT );

    glPointSize(5);
    glMatrixMode ( GL_MODELVIEW );

    glLoadIdentity ();

    glColor3f (1.0 , 1.0 , 1.0);

    glBegin ( GL_LINES );
    glVertex2f (-(maxX / 2), ty - 2.5);
    glVertex2f ((maxX / 2), ty - 2.5);
    glEnd ();

    glLoadIdentity ();

    glColor3f (1.0 , 1.0 , 1.0);

    glBegin ( GL_LINES );
    glVertex2f (-(maxX / 2), ty - 2.5);
    glVertex2f (-(maxX / 2), ty + platform);
    glEnd ();


    glLoadIdentity ();    
    
    glColor3f (1.0 , 0.0 , 0.5);
    glTranslatef ( tx, ty, 0.0);
    glBegin ( GL_POINTS );
    glVertex2f (0.0, 0.0);
    glEnd ();

    glLoadIdentity ();
    glColor3f (0.0 , 1.0 , 0.0);


    glPushMatrix();
    glTranslatef ( x, y, z);


    // glBegin ( GL_POINTS );
    // glVertex2f (0.0, 0.0);
    glutWireSphere(1000,5,5);
    // glEnd ();
    glPopMatrix(); 
    glutSwapBuffers ();
}

int main (int argc , char ** argv )
{
    alfa = atof(argv[1]);
    velocity = atof(argv[2]);
    speed = atof(argv[3]);
    platform = atof(argv[4]);    

    float kalk = ( ((velocity * sin(alfa * 3.14 / 180)) / (gravity * -1)) + sqrt((pow(velocity,2) * pow(sin(alfa * 3.14 / 180),2)) / pow((gravity * -1),2) + (2 * platform) / (gravity * -1))) * (velocity*cos(alfa * 3.14 / 180)+speed);

    printf("\nVypocitane d1:\t%f\n",kalk);

    int i = 1;

    maxY = -9999.0;
    maxX = -9999.0;

    y += platform;
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
        y = 0.1 + platform;
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
    printf("numericke d1:\t%f\nnumericke d2:\t%f", distance, maxX);
    
    maxX = distance;
    x = -(maxX / 2);
    y = -(maxY / 2) + 0.1 + platform;
    tx = (maxX / 2);
    ty = -(maxY / 2) + 0.1;

    glutInit (& argc , argv );
    glutInitDisplayMode ( GLUT_DOUBLE );
    if (maxX >= maxY)
        glutInitWindowSize (860 , 860*(maxY/maxX));
    else
        glutInitWindowSize (640 * (maxX/maxY) , 640);
    
    glutInitWindowPosition (50 , 50);
    glutCreateWindow (" OpenGL : CV5 ");
    
    glutDisplayFunc ( strela );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glClearColor (0.0 , 0.0 , 0.0 , 0.0);
    glutReshapeFunc ( obsluhaResize );
    glutTimerFunc ( icaskrok , aktualizuj , 1);
    glutMainLoop ();

    return 0;
}