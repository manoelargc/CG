#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;
// g++ followingTheBall.cpp -o app -lGL -lGLU -lglut -lm


/*===============================================================
CG-23.2
AVALIAÇÃO FINAL (17/12/23)

>Manoela Resende 2210100235

Utilizando o OpenGL através da biblioteca freeglut, desenvolva uma 
aplicação interativa em 3D. Utilize callbacks de mouse e/ou teclado, 
e permita que a sua aplicação carregue objetos do tipo Wavefront. 
* Visualizadores de objetos com rotação, translação e escala

================================================================
*/

//global
float cam_phi = 0.0, cam_theta = 0.0, cam_r = 10.0;
float sc = 1.0;
float xPosition = 0.0f;
float yPosition = 0.0f;
float zPosition = 0.0f;
float movementSpeed = 1.0f;
float alpha, theta;
int i;

//-----------
//illuminationm
//-----------

//ambient
GLfloat light0_ambient[] = {0.1, 0.1, 0.1, 1.0};

//diffuse
GLfloat light0_diffuse[] = {25.0, 2.0, 36.0, 0.0};
GLfloat light1_diffuse[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light0_position[] = {0.0, 100.0, -100.0, 0.0};

GLfloat light1_position[] = {0.0, 0.0, 10.0, 1.0};
GLfloat light2_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light2_position[] = {0.0, 0.0, -50.0, 11.0};
GLfloat light2_lookat[] = {1.0, 0.0, 0.0, 0.0};
GLfloat globalAmbient[] = {1.0, 1.0, 1.0, 1.0};


/**
 * This is a structure that represents the camera view.
 * It contains the eye position, the center position, and the up vector.
 */
typedef struct cameraView
{
    GLdouble eyeX;
    GLdouble eyeY;
    GLdouble eyeZ;
    GLdouble centerX;
    GLdouble centerY;
    GLdouble centerZ;
    GLdouble upX;
    GLdouble upY;
    GLdouble upZ;
} cameraView;

/**
 * This is a structure that represents a coordinate.
 * It contains the X, Y, and Z coordinates.
 */
typedef struct coordenada{
    GLdouble X;
    GLdouble Y;
    GLdouble Z;

} coordenada;



class myobj
{

public:
    coordenada coords;
    cameraView camera;
    vector<vector<float>> vertex;
    vector<vector<int>> faces;
    vector<vector<float>> normals;
    int numfaces;

    /**
     * This function loads an OBJ file into the object.
     * 
     * @param path - The path of the OBJ file.
     * @return 0 if the file was loaded successfully, -1 if there was an error.
     */
    int loadobj(string path)
    {
        numfaces = 0;
        string line;
        stringstream ss;
        ifstream myfile(path);
        if (myfile.good())
        {
            while (getline(myfile, line, '\n').good())
            {
                ss.str("");
                ss.clear();
                ss.str(line);
                string subs;
                vector<float> vfloat; //vetor pra guardar os vertice
                vector<int> fint;     //vetor pra guardar as arestas

                if (line[0] == 'v') //se vertice
                {
                    getline(ss, subs, ' '); //pula o caractere
                    while (ss.good())
                    {
                        getline(ss, subs, ' ');
                        vfloat.push_back(stof(subs));
                    }
                    vertex.push_back(vfloat);
                }

                if (line[0] == 'f') //se face
                {
                    numfaces++;
                    getline(ss, subs, ' ');
                    while (ss.good())
                    {
                        getline(ss, subs, ' ');
                        fint.push_back(stoi(subs));
                    }

                    faces.push_back(fint);
                }
            }
            myfile.close();
            for (int i = 0; i < numfaces; i++)
            {

                double Px = vertex[faces[i][1] - 1][0] - vertex[faces[i][0] - 1][0];
                double Py = vertex[faces[i][1] - 1][1] - vertex[faces[i][0] - 1][1];
                double Pz = vertex[faces[i][1] - 1][2] - vertex[faces[i][0] - 1][2];

                double Qx = vertex[faces[i][2] - 1][0] - vertex[faces[i][1] - 1][0];
                double Qy = vertex[faces[i][2] - 1][1] - vertex[faces[i][1] - 1][1];
                double Qz = vertex[faces[i][2] - 1][2] - vertex[faces[i][1] - 1][2];

                double Ci = Py * Qz - Pz * Qy;
                double Cj = Px * Qz - Pz * Qx;
                double Ck = Px * Qy - Py * Qx;

                double modulo = sqrt(Ci * Ci + Cj * Cj + Ck * Ck);
                vector<float> vec;
                vec.push_back(Ci / modulo);
                vec.push_back(Cj / modulo);
                vec.push_back(Ck / modulo);
                normals.push_back(vec);
            }
        }
        else
            cout << "Erro de leitura!" << endl;
        return 0;
    }
};

//setar a visualizacao da camera
cameraView camera = {0, 10.0, 500.0, 0, 0.0, 0.0, 0.0, 1.0, 0.0};

//declaração dos objetos
myobj obj; //obj1 = bola
myobj obj2; // obj2 = elefante


//------------
//metodos 
//------------
void timer(int value)
{
    /**
     * timer function
     * 
     * This function is called repeatedly by GLUT to update the display. 
     * It increments the theta variable by 1.0 and checks if the value of 
     * theta is greater than or equal to 100. If so, it resets the value to 0.0. 
     * Then, it calls glutPostRedisplay to update the display, and it sets the timer 
     * function to be called again in 10 milliseconds.
     * 
     * @param value - unused parameter
     */
    theta += 1.0;
    if (theta >= 100)
        theta = 0.0;
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}
void facedraw(myobj &obj)
{
    /**
     * Draws the faces of the given object.
     * 
     * @param obj - The object to draw.
     */
    int i;
    glPushMatrix();

    glColor4f(0.1, 1.5, 0.3, 0.0);

    for (i = 0; i < obj.numfaces; i++)
    {
        /**
         * Set the normal of the face to the normal of the face at the given index.
         * 
         * @param obj - The object to draw.
         * @param i - The index of the face to set the normal of.
         */
        glNormal3f(obj.normals[i][0], obj.normals[i][1], obj.normals[i][2]);

        glBegin(GL_TRIANGLES);

        /**
         * Draw a triangle using the three vertices of the face at the given index.
         * 
         * @param obj - The object to draw.
         * @param i - The index of the face to draw.
         */
        glVertex3f(obj.vertex[obj.faces[i][0] - 1][0], obj.vertex[obj.faces[i][0] - 1][1], obj.vertex[obj.faces[i][0] - 1][2]);
        glVertex3f(obj.vertex[obj.faces[i][1] - 1][0], obj.vertex[obj.faces[i][1] - 1][1], obj.vertex[obj.faces[i][1] - 1][2]);
        glVertex3f(obj.vertex[obj.faces[i][2] - 1][0], obj.vertex[obj.faces[i][2] - 1][1], obj.vertex[obj.faces[i][2] - 1][2]);

        glEnd();
    }
    glPopMatrix();
}

void display(void)
{
    /**
     * This function is called repeatedly by GLUT to update the display. 
     * It first clears the color and depth buffers using glClear. 
     * Then, it sets up the lighting and material properties using glLightfv, 
     * glMaterialfv, and glLightModelfv. 
     * It then sets the camera view using gluLookAt and scales, translates, and rotates the objects 
     * according to the camera position, scale, and rotation. 
     * Finally, it calls the facedraw function to draw the faces of the objects, 
     * and it flushes and swaps the buffers. 
     * 
     * @param void
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /**
     * This function sets the position of the third light source, which is a spotlight. 
     * The position is set to (0, 5, 5), the look-at position is set to (100, 100, 100), 
     * and the spotlight exponent is set to 10.0. 
     * 
     * @param GLenum light - The light to set the properties of.
     * @param GLenum pname - The property of the light to set.
     * @param const GLfloat *params - The value to set the property to.
     */
    glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_lookat);

    /**
     * This function sets the ambient lighting for the scene. 
     * The ambient lighting is set to (0.4, 0.4, 0.4, 1), which means that the 
     * scene will have a lighting color of 40% gray. 
     * 
     * @param GLenum pname - The property of the lighting model to set.
     * @param const GLfloat *params - The value to set the property to.
     */
    GLfloat global_ambient[] = {0.4, 0.4, 0.4, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    /**
     * This function sets the modelview matrix to the identity matrix. 
     * This means that the objects will be drawn in the world coordinate system, 
     * without any transformations applied. 
     * 
     * @param void
     */
    glLoadIdentity();

    /**
     * This function sets up the camera view. 
     * The eye is located at (camera.eyeX, camera.eyeY, camera.eyeZ), 
     * the center of the scene is located at (camera.centerX, camera.centerY, camera.centerZ), 
     * and the up vector is located at (camera.upX, camera.upY, camera.upZ). 
     * The function also sets the field of view to 45 degrees and the aspect ratio. 
     * 
     * @param GLdouble eyeX - The X coordinate of the eye position.
     * @param GLdouble eyeY - The Y coordinate of the eye position.
     * @param GLdouble eyeZ - The Z coordinate of the eye position.
     * @param GLdouble centerX - The X coordinate of the center position.
     * @param GLdouble centerY - The Y coordinate of the center position.
     * @param GLdouble centerZ - The Z coordinate of the center position.
     * @param GLdouble upX - The X coordinate of the up vector.
     * @param GLdouble upY - The Y coordinate of the up vector.
     * @param GLdouble upZ - The Z coordinate of the up vector.
     */
    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ, camera.centerX, camera.centerY, camera.centerZ, camera.upX, camera.upY, camera.upZ);

    /**
     * This function scales the objects according to the value of sc. 
     * The value of sc is decreased by 0.1 each time the page up key is pressed, 
     * and increased by 0.1 each time the page down key is pressed. 
     * 
     * @param GLfloat sc - The scale factor.
     */
    glScalef(4.0 - sc, 4.0 - sc, 4.0 - sc);

    /**
     * This function translates the objects according to the values of xPosition, yPosition, and zPosition. 
     * The objects will move in the direction that the camera is looking. 
     * 
     * @param GLdouble xPosition - The X translation.
     * @param GLdouble yPosition - The Y translation.
     * @param GLdouble zPosition - The Z translation.
     */
    glTranslated(xPosition, yPosition, zPosition);

    /**
     * This function rotates the objects around the X axis according to the value of cam_theta. 
     * The rotation is performed in the direction that the camera is looking. 
     * 
     * @param GLfloat cam_theta - The rotation angle around the X axis.
     * @param GLdouble 0.0 - The rotation axis.
     * @param GLdouble 1.0 - The rotation axis.
     * @param GLdouble 0.0 - The rotation axis.
     */
    glRotatef(cam_theta, 0.0, 1.0, 0.0);

    /**
     * This function rotates the objects around the Y axis according to the value of cam_phi. 
     * The rotation is performed in the direction perpendicular to the camera view. 
     * 
     * @param GLfloat cam_phi - The rotation angle around the Y axis.
     * @param GLdouble 1.0 - The rotation axis.
     * @param GLdouble 0.0 - The rotation axis.
     * @param GLdouble 0.0 - The rotation axis.
     */
    glRotatef(cam_phi, 1.0, 0.0, 0.0);

    /**
     * This function draws the faces of the objects using the facedraw function. 
     * The facedraw function sets the normal of the face to the normal of the face 
     * at the given index, and then draws a triangle using the three vertices of the face. 
     * 
     * @param myobj &obj - The object to draw.
     */
    facedraw(obj);

    /**
     * This function pushes the modelview matrix onto the stack, 
     * translates the objects according to the values of xPosition, yPosition, and zPosition, 
     * and then rotates the objects around the X axis according to the value of cam_theta. 
     * The rotation is performed in the direction that the camera is looking. 
     * The function then calls the facedraw function to draw the faces of the objects, 
     * and finally pops the modelview matrix off the stack. 
     * 
     * @param myobj &obj2 - The second object to draw.
     */
    glPushMatrix();
    glTranslated(xPosition, yPosition, zPosition);
    glRotatef(cam_theta, 0.0, 1.0, 0.0);
    facedraw(obj2);
    glPopMatrix();

    /**
     * This function flushes and swaps the buffers, 
     * which displays the updated scene on the screen. 
     * 
     * @param void
     */
    glFlush();
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}
void myReshape(int w, int h)
/**
 * This function is called when the window is resized. 
 * It sets the projection matrix to a perspective projection with the given field of view 
 * and aspect ratio, and it sets the viewport to the given width and height. 
 * 
 * @param int w - The new width of the window.
 * @param int h - The new height of the window.
 */
{
    float aspectRatio = (float)w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-50, 50, -50, 50, -50, 50);
    gluPerspective(45, aspectRatio, -20, 20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myInit(void)
{
    /**
     * This function initializes OpenGL and sets up the lighting and display properties.
     * 
     * @param void
     */
    glClearColor(0.0, 0.0, 0.0, 0.0); // This function sets the color of the background to black.
    glShadeModel(GL_FLAT); // This function sets the shading model to flat shading.
    glFrontFace(GL_CW); // This function sets the orientation of the front face to counter-clockwise.
    glCullFace(GL_FRONT); // This function enables culling of front-facing polygons.
    glEnable(GL_CULL_FACE); // This function enables culling.

    glClearDepth(100.0f); // This function sets the clear depth buffer value to 100.0.
    glEnable(GL_DEPTH_TEST); // This function enables depth testing.

    glDepthFunc(GL_LEQUAL); // This function sets the depth function to less than or equal.
    glDepthRange(200, -200); // This function sets the depth range to 200 units in front of the camera and -200 units behind the camera.

    glColorMaterial(GL_FRONT, GL_DIFFUSE); // This function sets the color material to diffuse for the front face.
    glEnable(GL_COLOR_MATERIAL); // This function enables the color material.

    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0}; // This array defines the specular color of the objects.
    GLfloat mat_shininess[] = {50.0}; // This array defines the shininess of the objects.
    GLfloat light_position[] = {100.0, -1000.0, -100.0, 1.0}; // This array defines the position of the first light source.
    glClearColor(1.0, 1.0, 1.0, 0.0); // This function sets the color of the background to white.

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); // This function sets the specular properties of the objects.
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // This function sets the shininess of the objects.
    //    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // This function sets the position of the first light source.
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); // This function sets the diffuse properties of the first light source.

    glEnable(GL_LIGHT0); // This function enables the first light source.
    glEnable(GL_LIGHTING); // This function enables lighting.
}

void handleKeypress(unsigned char key, int x, int y)
{
    /**
     * This function is called when a key is pressed on the keyboard. 
     * The function handles the up, down, left, right, page up, and page down keys, 
     * and updates the position of the objects accordingly. 
     * The function also calls glutPostRedisplay to update the display. 
     * 
     * @param unsigned char key - The ASCII value of the key that was pressed.
     * @param int x - The X coordinate of the mouse pointer.
     * @param int y - The Y coordinate of the mouse pointer.
     */
    switch (key)
    {
    case 'w': // up
        /**
         * This statement decreases the zPosition of the objects by the movementSpeed variable. 
         * The movementSpeed variable is set to 1.0 by default, but it can be changed in the main function. 
         */
        zPosition -= movementSpeed;
        break;
    case 's': // down
        /**
         * This statement increases the zPosition of the objects by the movementSpeed variable. 
         * The movementSpeed variable is set to 1.0 by default, but it can be changed in the main function. 
         */
        zPosition += movementSpeed;
        break;
    case 'a': // left
        /**
         * This statement decreases the xPosition of the objects by the movementSpeed variable. 
         * The movementSpeed variable is set to 1.0 by default, but it can be changed in the main function. 
         */
        xPosition -= movementSpeed;
        break;
    case 'd': // right
        /**
         * This statement increases the xPosition of the objects by the movementSpeed variable. 
         * The movementSpeed variable is set to 1.0 by default, but it can be changed in the main function. 
         */
        xPosition += movementSpeed;
        break;
    case '1': // Zoom out
        /**
         * This statement increases the zPosition of the objects by the movementSpeed variable. 
         * The movementSpeed variable is set to 1.0 by default, but it can be changed in the main function. 
         */
        zPosition += movementSpeed;
        break;
    case '2': // Zoom in 
        /**
         * This statement decreases the zPosition of the objects by the movementSpeed variable. 
         * The movementSpeed variable is set to 1.0 by default, but it can be changed in the main function. 
         */
        zPosition -= movementSpeed;
        break;
    }
    /**
     * This function updates the display by calling the display function. 
     * The display function redraws the scene and updates the screen. 
     */
    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y)
{
    /**
     * This function is called when a special key is pressed on the keyboard. 
     * The function handles the up, down, left, right, page up, and page down keys, 
     * and updates the camera position and scale accordingly. 
     * The function also calls glutPostRedisplay to update the display. 
     * 
     * @param int key - The ASCII value of the key that was pressed.
     * @param int x - The X coordinate of the mouse pointer.
     * @param int y - The Y coordinate of the mouse pointer.
     */
    switch (key)
    {
    case GLUT_KEY_UP:
        /**
         * This statement decreases the camera position along the Y axis by 2 units. 
         */
        cam_phi -= 2.0;
        break;
    case GLUT_KEY_DOWN:
        /**
         * This statement increases the camera position along the Y axis by 2 units. 
         */
        cam_phi += 2.0;
        break;
    case GLUT_KEY_LEFT:
        /**
         * This statement decreases the camera position along the X axis by 2 units. 
         */
        cam_theta -= 2.0;
        break;
    case GLUT_KEY_RIGHT:
        /**
         * This statement increases the camera position along the X axis by 2 units. 
         */
        cam_theta += 2.0;
        break;
    case GLUT_KEY_PAGE_UP:
        /**
         * This statement decreases the scale of the objects by 0.1. 
         */
        sc -= 0.1;
        break;
    case GLUT_KEY_PAGE_DOWN:
        /**
         * This statement increases the scale of the objects by 0.1. 
         */
        sc += 0.1;
        break;
    }
    /**
     * This function updates the display by calling the display function. 
     * The display function redraws the scene and updates the screen. 
     */
    glutPostRedisplay();
}


void setInitialObjectPosition() {
    /**
     * This function sets the initial position of the two objects.
     * The position of the first object is set to (0, 3, 150), and the position of the second object is set to (0, -3, 150).
     * The Y position of the second object is adjusted to separate the two objects.
     */
    obj.coords.X = 0.0;
    obj.coords.Y = 3.0;
    obj.coords.Z = 150.0;

    obj2.coords.X = 0.0;
    obj2.coords.Y = -3.0; // Adjust the Y position to separate the two objects
    obj2.coords.Z = 150.0;
}


int main(int argc, char **argv)
{

    setInitialObjectPosition();

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glEnable(GL_BLEND | GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    obj.loadobj("ball.obj");

    // Load a second object
    obj2.loadobj("elephant.obj");

    myInit();

    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(processSpecialKeys);

    glutTimerFunc(10, timer, 0);

    glutMainLoop();
    return 0;
}
