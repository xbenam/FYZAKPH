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
const int icaskrok = 1000/50;

float x = 0.0;
float y = 3.0;
float z = 0.0;

float mass = 0.0;
float force = 0.0;
float resistance = 0.0;

float xAcceleration = 0.0;
float zAcceleration = 0.0;
float xSpeed = 0.0;
float zSpeed = 0.0;
float xSpeedMax = 0.0;
float zSpeedMax = 0.0;


float gravity = -9.81;

float maxY = -9999.0;
float maxX = 0;
float maxZ = 0;

float rotate = 0;
float xRotation = 0.0;
float yRotation = 0.0;
float zRotation = 0.0;


void aktualizuj (const int ihod )
{
    glutPostRedisplay ();
    
    if ((ihod / 250) % 4 == 0){
        if(ihod <= 250){
            xAcceleration = ((force - resistance * xSpeed) / 2.0) / mass;
        }
        else{
            xAcceleration = (force - resistance * xSpeed) / mass;
            zAcceleration = 0;
        }
        zRotation = -1;
        rotate += (xSpeed+zSpeed) / M_PI;
    }

    if ((ihod / 250) % 4 == 1){
        if(ihod <= 500){
            zAcceleration = ((force - resistance * zSpeed) / 2.0) / mass;
        }
        else{
            zAcceleration = (force - resistance * zSpeed) / mass;
        }
        xAcceleration = 0;
        xRotation = -1;
        rotate += (xSpeed+zSpeed) / M_PI;
    }

    if ((ihod / 250) % 4 == 2){
        xAcceleration = (((-force) - resistance * xSpeed)) / mass;
        zAcceleration = 0;
        zRotation = 1;
        rotate -= (xSpeed+zSpeed) / M_PI;
    }        

    if ((ihod / 250) % 4 == 3){
        zAcceleration = (((-force) - resistance * zSpeed)) / mass;
        xAcceleration = 0;
        xRotation = 1;
        rotate -= (xSpeed+zSpeed) / M_PI;
    }
        
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // // glOrtho(-50, 150, -50, 50, -300, 300); 
    // gluPerspective(90.0f, 1, 0.1f, 1000.0f);
    // gluLookAt(  
    //         abs(maxX)*0.5, 25.0, 25.0,
    //         x, y, z,
    //         0.0, 1.0, 0.0
    //         );
    // glMatrixMode(GL_MODELVIEW);

    x = x + xSpeed * (icaskrok / 1000.0) + 0.5 * xAcceleration * pow((icaskrok / 1000.0), 2);
    z = z + zSpeed * (icaskrok / 1000.0) + 0.5 * zAcceleration * pow((icaskrok / 1000.0), 2);
    // printf("%d\t %f\t%f\t%f\t%f\n",ihod, x, z, xAcceleration, zAcceleration);
    xSpeed = xSpeed + xAcceleration * (icaskrok / 1000.0);
    zSpeed = zSpeed + zAcceleration * (icaskrok / 1000.0);
    glutTimerFunc ( icaskrok , aktualizuj , ihod +1);
}

void obsluhaResize (int sirka , int vyska )
{

    glViewport(0, 0, sirka, vyska);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(maxX >= 0){
        glOrtho(-maxX*0.5, maxX*0.5, -maxZ*0.5, maxZ*0.5, -300, 300); 
    }
    else{
        glOrtho(maxX*0.8, 3, maxX*0.1, -maxX*0.1, -300, 300); 
    }
    // gluPerspective(90.0f, 1, 0.1f, 1000.0f);
    gluLookAt(  
            0, 10.0, 10.0,
            x, y, z,
            0.0, 1.0, 0.0
            );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void strela ()
{
    glClear ( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

    glMatrixMode ( GL_MODELVIEW );

    //X-axis
    glLoadIdentity ();
    glColor3f (1.0 , 0.0 , 0.0);
    glBegin ( GL_LINES );
    glVertex3f (-10000, 0,0);
    glVertex3f (10000, 0,0);
    glEnd ();

    //Y-axis
    glLoadIdentity ();
    glColor3f (0.0 , 1.0 , 0.0);
    glBegin ( GL_LINES );
    glVertex3f (0, -10000, 0);
    glVertex3f (0, 10000, 0);
    glEnd ();

    //Z-axis
    glLoadIdentity ();
    glColor3f (0.0 , 0.0 , 1.0);
    glBegin ( GL_LINES );
    glVertex3f (0, 0, -10000);
    glVertex3f (0, 0, 10000);
    glEnd ();

    //plane
    glLoadIdentity ();
    glColor3f (0.3 , 0.3 , 0.3);
    glBegin ( GL_QUADS );
    glVertex3f (5000, -0.5, -5000);
    glVertex3f (-5000, -0.5, -5000);
    glVertex3f (-5000, -0.5, 5000);
    glVertex3f (5000, -0.5, 5000);
    glEnd ();

    //ball
    glLoadIdentity ();
    glColor3f (0.0 , 1.0 , 0.0);
    glPushMatrix();
    glTranslatef ( x, y, z);
    glRotatef(rotate, xRotation, yRotation, zRotation);
    glutWireSphere(3, 12, 12);

    glPopMatrix(); 
    glutSwapBuffers ();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f }; 
 
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f }; 

int main (int argc , char ** argv )
{
    mass = atof(argv[1]);
    force = atof(argv[2]);
    resistance = atof(argv[3]);
    
    int time = 1;
    xAcceleration = force/mass;
    zAcceleration = force/mass;
    while(time <= 500){
        maxX = maxX + xSpeed * (icaskrok / 1000.0) + 0.5 * xAcceleration * pow((icaskrok / 1000.0), 2);
        maxZ = maxZ + zSpeed * (icaskrok / 1000.0) + 0.5 * zAcceleration * pow((icaskrok / 1000.0), 2);
        xSpeed = xSpeed + xAcceleration * (icaskrok / 1000.0);
        zSpeed = zSpeed + zAcceleration * (icaskrok / 1000.0);
        time++;
    }
    // printf("%f",maxX);
    xSpeed = 0.0;
    zSpeed = 0.0;
    xAcceleration = 0.0;
    zAcceleration = 0.0;

    glutInit (& argc , argv );
    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (860 , 630);
    
    glutInitWindowPosition (50 , 50);
    glutCreateWindow (" OpenGL : CV8 ");

    glutDisplayFunc ( strela );
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
 
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); 
 
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
 
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 
    glClearColor (0.0 , 0.0 , 0.0 , 0.0);
    glutReshapeFunc ( obsluhaResize );
    Sleep(1000);
    glutTimerFunc ( icaskrok , aktualizuj , 1);
    glutMainLoop ();

    return 0;
}
