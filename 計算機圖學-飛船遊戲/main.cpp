

#ifdef __WIN32__
#include <windows.h>
#endif
#include<windows.h>
#include <GL/glut.h>
#include<thread>
#include "load_bmp.h"
#include "load_3ds.h"
#include "object.h"
#include "Vector3.h"
#include<sstream>

#define MAX_CHAR       128
obj_type1 object2 =
{
    {
        -10, -10,   // vertex v0
        10,  -10,   // vertex v1
        10,  10,  // vertex v2
        -10, 10  // vertex v3

    },
    {
        2,0, 1,  // polygon v0,v1,v4
        2, 3, 0  // polygon v1,v5,v4
    },
    {
        0.0, 1.0,  // mapping coordinates for vertex v0
        1.0, 1.0,  // mapping coordinates for vertex v1
        1.0, 0.0,  // mapping coordinates for vertex v2
        0.0, 0.0,  // mapping coordinates for vertex v3
    },
    0,
};

Vector3 cubPo1(-8, 7, 0);
Vector3 cubPo2(0, 14, 0);
Vector3 cubPo3(9, 10, 0);
Vector3 cscale1(6, 1, 1);
Vector3 cscale2(12, 1, 1);
Vector3 cscale3(3, 1, 1);
float moveSpeed = 0;
float dropSpeed = 0.02;
bool isContinue = true;
int gametime = 60;

char startingPart[] = "score: ";

float cubeSize = 1.0;
int score = 0;
Vector3 planeCoor(0,-7,0);
float planeAngle = 90;

int screen_width=640;
int screen_height=480;

int imageWidth;
int imageHeight;

unsigned char* imageData;

GLfloat light_ambient[]= { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat light_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_position[]= { 100.0f, 0.0f, -10.0f, 1.0f };

GLfloat mat_ambient[]= { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat mat_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular[]= { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat mat_shininess[]= { 1.0f };
#define MAX_CHAR       128

void TimerTextUpdate(int& timer)
{
    while (true)
    {
        if (isContinue)
        {
            timer--;
            if (timer <= 0)
            {
                isContinue = false;
            }
            score++;
            Sleep(1000);
        }

    }
}

void drawString(const char* str) {

    static int isFirstCall = 1;
    static GLuint lists;

    if (isFirstCall) { // 如果是第一次調用，執行初始化

        // 爲每一個ASCII字符產生一個顯示列表

        isFirstCall = 0;

        // 申請MAX_CHAR個連續的顯示列表編號

        lists = glGenLists(MAX_CHAR);

        // 把每個字符的繪製命令都裝到對應的顯示列表中

        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);

    }

    // 調用每個字符對應的顯示列表，繪製每個字符

    for (; *str != '\0'; ++str)
        glCallList(lists + *str);

}



void selectFont(int size, int charset, const char* face) {

    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,

        charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,

        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);

    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);

    DeleteObject(hOldFont);

}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color to black

    // Viewport transformation
    glViewport(0,0,screen_width,screen_height);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,5.0f,10000.0f); // We define the "viewing volume"
   
	//Lights initialization and activation
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);    
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);

    //Materials initialization and activation
	glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv (GL_FRONT, GL_SHININESS, mat_shininess);    

	//Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glEnable(GL_TEXTURE_2D); // Texture mapping ON
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST); // Enable the depth test (also called z buffer)

	//Objects loading
	ObjLoad ("fighter1.3ds","skull.bmp");
    object2.id_texture = LoadBitmap("bmp.bmp");
    
    
}
void resize (int p_width, int p_height)
{
	
    screen_width=p_width; // We obtain the new screen width values and store it
    screen_height=p_height; // Height value

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0,0,screen_width,screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,5.0f,10000.0f);

    glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}
void myKeyboard(unsigned char key, int x, int y)
{
    if (isContinue)
    {
        switch (key)
        {
        case 'W':case 'w':
            planeCoor.y += 0.5;
            break;
        case 'S':case's':
            planeCoor.y -= 0.5;
            break;
        case 'A':case 'a':
            planeCoor.x -= 0.5;
            break;
        case 'D':case 'd':
            planeCoor.x += 0.5;
            break;
        default:
            break;
        }
        glutPostRedisplay();
    }
}
void airplane(float scale,Vector3 position)
{
    int j;
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(scale, scale, scale);
    glRotatef(planeAngle,1,0,0);
    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (j = 0; j < object.polygons_qty; j++)
    {
        //----------------- FIRST VERTEX -----------------
        //Normal coordinates of the first vertex
        glNormal3f(object.normal[object.polygon[j].a].x,
            object.normal[object.polygon[j].a].y,
            object.normal[object.polygon[j].a].z);
        // Texture coordinates of the first vertex
        glTexCoord2f(object.mapcoord[object.polygon[j].a].u,
            object.mapcoord[object.polygon[j].a].v);
        // Coordinates of the first vertex
        glVertex3f(object.vertex[object.polygon[j].a].x,
            object.vertex[object.polygon[j].a].y,
            object.vertex[object.polygon[j].a].z);

        //----------------- SECOND VERTEX -----------------
        //Normal coordinates of the second vertex
        glNormal3f(object.normal[object.polygon[j].b].x,
            object.normal[object.polygon[j].b].y,
            object.normal[object.polygon[j].b].z);
        // Texture coordinates of the second vertex
        glTexCoord2f(object.mapcoord[object.polygon[j].b].u,
            object.mapcoord[object.polygon[j].b].v);
        // Coordinates of the second vertex
        glVertex3f(object.vertex[object.polygon[j].b].x,
            object.vertex[object.polygon[j].b].y,
            object.vertex[object.polygon[j].b].z);

        //----------------- THIRD VERTEX -----------------
        //Normal coordinates of the third vertex
        glNormal3f(object.normal[object.polygon[j].c].x,
            object.normal[object.polygon[j].c].y,
            object.normal[object.polygon[j].c].z);
        // Texture coordinates of the third vertex
        glTexCoord2f(object.mapcoord[object.polygon[j].c].u,
            object.mapcoord[object.polygon[j].c].v);
        // Coordinates of the Third vertex
        glVertex3f(object.vertex[object.polygon[j].c].x,
            object.vertex[object.polygon[j].c].y,
            object.vertex[object.polygon[j].c].z);
    }
    glEnd();
    glPopMatrix();
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
    glTranslatef(0.0,0.0,-20);
    if (isContinue)
    {
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHTING);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-10.5f, 7.5f);

        std::stringstream s;
        char temp[1024];
        s << score;
        s >> temp;
        drawString("score: ");

        glRasterPos2f(-8.5f, 7.5f);
        drawString(temp);

        glRasterPos2f(6.0f, 7.5f);
        drawString("remaining time: ");

        std::stringstream x;
        char temp1[1024];
        x << gametime;
        x >> temp1;

        glRasterPos2f(9.5f, 7.5f);
        drawString(temp1);

        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);

        glBindTexture(GL_TEXTURE_2D, object.id_texture);
        glEnable(GL_TEXTURE_2D);
        airplane(0.15, planeCoor);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(cubPo1.x, cubPo1.y-moveSpeed, cubPo1.z);
        glScalef(cscale1.x, cscale1.y, cscale1.z);
        glutSolidCube(cubeSize);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(cubPo2.x, cubPo2.y - moveSpeed, cubPo2.z);
        glScalef(cscale2.x, cscale2.y, cscale2.z);
        glutSolidCube(cubeSize);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(cubPo3.x, cubPo3.y - moveSpeed, cubPo3.z);
        glScalef(cscale3.x, cscale3.y, cscale3.z);
        glutSolidCube(cubeSize);
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, object2.id_texture);
        /*if (isContinue)
            backgroundTranslateDeduct += 0.05;
        else
            backgroundTranslateDeduct = 0;
        if (backgroundTranslateDeduct > 900)backgroundTranslateDeduct = 1;*/
        glTranslatef(0.0, 0.0, -20.0);
        glScalef(1, 1, 1);
        glBegin(GL_TRIANGLES);
        for (int l_index = 0; l_index < 2; l_index++)
        {
            glTexCoord2f(object2.mapcoord[object2.polygon[l_index].a].u,
                object2.mapcoord[object2.polygon[l_index].a].v);

            glVertex2f(object2.vertex[object2.polygon[l_index].a].x,
                object2.vertex[object2.polygon[l_index].a].y);

            glTexCoord2f(object2.mapcoord[object2.polygon[l_index].b].u,
                object2.mapcoord[object2.polygon[l_index].b].v);

            glVertex2f(object2.vertex[object2.polygon[l_index].b].x,
                object2.vertex[object2.polygon[l_index].b].y);

            glTexCoord2f(object2.mapcoord[object2.polygon[l_index].c].u,
                object2.mapcoord[object2.polygon[l_index].c].v);

            glVertex2f(object2.vertex[object2.polygon[l_index].c].x,
                object2.vertex[object2.polygon[l_index].c].y);
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        

        glFlush(); 
        glutSwapBuffers(); 
        moveSpeed += dropSpeed+score/1000.0;
        if (moveSpeed > 22)
        {
            moveSpeed = 0;
        }
        if (planeCoor.x + 0.5 > cubPo1.x - (cscale1.x / 2) && planeCoor.x - 0.5 < cubPo1.x + (cscale1.x / 2))
        {
            if (planeCoor.y + 0.7 > cubPo1.y - moveSpeed - (cscale1.y / 2) && planeCoor.y - 0.7 < cubPo1.y - moveSpeed + (cscale1.y / 2))
            {
                isContinue = false;
            }
        
        }
        if (planeCoor.x + 0.5 > cubPo2.x - (cscale2.x / 2) && planeCoor.x - 0.5 < cubPo2.x + (cscale2.x / 2))
        {
            if (planeCoor.y + 0.7 > cubPo2.y - moveSpeed - (cscale2.y / 2) && planeCoor.y - 0.7 < cubPo2.y - moveSpeed + (cscale2.y / 2))
            {
                isContinue = false;
            }
        }
        if (planeCoor.x + 0.5 > cubPo3.x - (cscale3.x / 2) && planeCoor.x - 0.5 < cubPo3.x + (cscale3.x / 2))
        {
            if (planeCoor.y + 0.7 > cubPo3.y - moveSpeed - (cscale3.y / 2) && planeCoor.y - 0.7 < cubPo3.y - moveSpeed + (cscale3.y / 2))
            {
                isContinue = false;
            }
        }
    }
    else
    {
        selectFont(48, ANSI_CHARSET, "Comic Sans MS");
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHTING);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-5.0f, 0.0f);

        drawString("Boom !!Game Over!!");


        glRasterPos2f(-5.0f, 2.0f);

        drawString("You scored: ");
        glRasterPos2f(-1.5f, 2.0f);

        std::stringstream s;
        char temp[1024];
        s << score;
        s >> temp;
        drawString(temp);

        glutSwapBuffers();
    }
}
int main(int argc, char **argv)
{
    std::thread t1(TimerTextUpdate, std::ref(gametime));
    glutInit(&argc, argv);    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screen_width,screen_height);
    glutInitWindowPosition(0,0);
    glutCreateWindow("www.spacesimulator.net - 3d engine tutorials - To exit press ESC");    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc (resize);
    glutKeyboardFunc (myKeyboard);
    
    init();
    glutMainLoop();   
}
