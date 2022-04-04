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

float x = 0.0;
float y = 0.0;
float z = 0.0;

float alfa = 0.0;
float velocity = 0.0;
float phi = 0.0;
float platform = 0.0;

float gravity = -9.81;

float maxY = -9999.0;
float maxX = -9999.0;
float maxZ = -9999.0;

float rotate = 15;


void aktualizuj (const int ihod )
{
    if(ihod==1)Sleep(500);
    if(y > 1.5){
        z += (velocity * cos(alfa * 3.14 / 180) * cos(phi * 3.14 / 180)) * 0.03;
        x += (velocity * cos(alfa * 3.14 / 180) * sin(phi * 3.14 / 180)) * 0.03;
        y += (velocity * sin(alfa * 3.14 / 180) + 0.5 * ( gravity * ihod)) * 0.03;
        rotate +=5;
        
        if (y <= 1.5) y = 1.5;
    }
    else return ;
    // x+=0.05;
    // y+=0.05;
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(-10, 10, -10.0, 10.0, -100, 100.0); 
    // // glTranslatef(-5,-5,-10);
    // // gluPerspective(90.0f, ar, 0.1f, 1000.0f);
    // gluLookAt(0.0,0.0,0.0,
    //         x,y,z,
    //         0,1,0);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(-300, 300, -300.0, 300.0, -300, 300.0); 
    // glTranslatef(-5,-5,-10);
    // gluPerspective(90.0f, 1.0f, 0.1f, 1000.0f);
    // gluLookAt(100.0, 10.0, 250.0,
    //         0.0, 0.0, 0.0,
    //         0.0, 1.0, 0.0);
    // glOrtho(-135, 135, -100.0, 100.0, -300, 1000.0); 
    // gluPerspective(90.0f, 1, 0.1f, 1000.0f);
    // gluLookAt(150, 150, 150,
    //         0.0, 0.0, 0.0,
    //         0.0,1.0,0.0);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluPerspective(90.0f, 1, 0.1f, 1000000.0f);   
    // gluLookAt( sqrt(pow(maxZ, 2) + pow(maxX, 2)), (maxY)*1.1, sqrt(pow(maxZ, 2) + pow(maxX, 2)),
    //         x, y, z,
    //         0.0,1.0,0.0);
    
    glutPostRedisplay ();
    glutTimerFunc ( icaskrok , aktualizuj , ihod +1);
}

void obsluhaResize (int sirka , int vyska )
{
    // const float ar = (float) sirka / (float) vyska; 
    // float ar = (float) 
    glViewport(0, 0, sirka, vyska);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluLookAt(
    // 0, 0, 2,
    // 0, 0, 0,
    // 0, -1, 0);
    // glOrtho(-10, 10, -10.0, 10.0, 1.0, 10.0); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ano = sqrt( pow(maxY, 2)+ (pow(maxZ, 2) + pow(maxX, 2)));
    glOrtho(-ano * sin(55 * 3.14 / 180) * 1.2, ano * sin(55 * 3.14 / 180) * 1.2, -ano * sin(45 * 3.14 / 180) * 1.2, ano * sin(45 * 3.14 / 180) * 1.2, -300, ano * 2); 
    // glTranslatef(0,-100,0);
    // glRotatef(1, 0, 1, 0);
    // gluPerspective(90.0f, 1, 0.1f, 1000000.0f);    
    gluLookAt((ano * sin(35 * 3.14 / 180)) * sin(45 * 3.14 / 180), ano * sin(45 * 3.14 / 180), (ano * sin(55 * 3.14 / 180)) * sin(45 * 3.14 / 180),
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
    
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

    //X-location
    glLoadIdentity ();
    glColor3f (0.0 , 1.0 , 1.0);
    glBegin ( GL_LINES );
    glVertex3f (x, 0, 0);
    glVertex3f (x, 0, z);
    glEnd ();

    //Z-location
    glLoadIdentity ();
    glColor3f (0.0 , 1.0 , 1.0);
    glBegin ( GL_LINES );
    glVertex3f (0, 0, z);
    glVertex3f (x, 0, z);
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
    glRotatef(rotate,1.5,-2,1);
    glutWireSphere(3,12,12);

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
    velocity = atof(argv[1]);
    alfa = atof(argv[2]);
    phi = atof(argv[3]);
    platform = atof(argv[4]);    

    y += platform;

    int time = 1;
    while(y > 1.5){
        z += (velocity * cos(alfa * 3.14 / 180) * cos(phi * 3.14 / 180)) * 0.03;
        x += (velocity * cos(alfa * 3.14 / 180) * sin(phi * 3.14 / 180)) * 0.03;
        y += (velocity * sin(alfa * 3.14 / 180) + 0.5 * ( gravity * time)) * 0.03;
        maxX = max(abs(x), maxX);
        maxY = max(abs(y), maxY);
        maxZ = max(abs(z), maxZ);
        time++;
    }
    x = 0.0;
    y = 0.0 + platform;
    z = 0.0;
    printf("x: %f\ty: %f\tz: %f\n", maxX, maxY , maxZ * 2);

    glutInit (& argc , argv );
    glutInitDisplayMode ( GLUT_DOUBLE );
    glutInitWindowSize (860 , 860);
    // if (ano >= maxY)
    //     glutInitWindowSize (860 , 860*(maxY/ano));
    // else
    //     glutInitWindowSize (640 * (ano/maxY) , 640);
    
    glutInitWindowPosition (50 , 50);
    glutCreateWindow (" OpenGL : CV6 ");
    
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
    glutTimerFunc ( icaskrok , aktualizuj , 1);
    glutMainLoop ();

    return 0;
}