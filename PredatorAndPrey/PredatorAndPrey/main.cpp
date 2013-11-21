#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include "Settings.h"
#include "ini.h"
#include <random>
#include <time.h>
#include <iomanip>
#include "text3d.h"
#include <algorithm>
#include <string>
#include "Node.h"
#include "vec.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//#define ZOOM_VALUE -15.0f
//#define GLUT_UPDATE_MILLIS 20

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define TOP_BOUNDARY -5
#define RIGHT_BOUNDARY 7
#define BOTTOM_BOUNDARY 5
#define LEFT_BOUNDARY -7
#define Z_PLANE_BOUNDARY 0


#define ROT_AMOUNT 1.2
#define PAN_AMOUNT 0.3
#define PI 3.1415926

using namespace std;


float player1_x = 0;
float player1_y = 0;
float player1_xSpeed = 0;
float player1_ySpeed = 0;
float player1_radius = 0.5;
float player1_budget = 10;//PLAYER_STARTING_BUDGET;

float player2_x;
float player2_y;
float player2_radius = 0.5;

float spinAngle = 0;
float spinSpeed = 0;

//camera starting settings
float cam_x = 0.0;
float cam_y = -1.0;
float cam_z = 0.0;

//lighting settings
GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //white light
GLfloat lightPos0[] = {0.0f, 0.0f, 1.5f, 1.0f}; 

float _scale = 0.5f;

float *budget;
Setting_list *settings;


typedef unsigned char   Bool;
typedef enum
{
    ENTER    = 3,
    TAB        = 9,
    RETURN    = 13,
    ESC        = 27,
    SPACE    = 32,
    DEL        = 127,
    UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW,
    NUM_KEY_CODES
} KeyCode;

Bool    key[NUM_KEY_CODES], keyDown[NUM_KEY_CODES], keyUp[NUM_KEY_CODES];






typedef struct
{
    float width;
	float height;
	int children;
} Info;


Node *treeRecurse(Node *currentNode, int depth, float width, float height)
{
	int branches=3;
	float shrink=0.7;
	Node *current = currentNode;
	for(int i=0; i<depth; i++)
	{
		current->addChild(width, height);
		for(int j=0; j<branches; j++)
		{
			treeRecurse(current->getChildren(), depth-1, width*shrink, height*shrink);
		}

	}
	return currentNode;
}


Node *treeRecurse(Node *currentNode, vec3 position, vec3 direction, float width, int depth)
{
	int branches=4;
	float shrink=0.7;
	Node *current = currentNode;
	for(int i=0; i<depth; i++)
	{
		current->addChild(position, direction, width*shrink);
		for(int j=0; j<branches; j++)
		{
			//(j* 2 * PI)*(j/branches)
			float newX = sin(j * 2 * PI / branches);
			float newY = cos(j * 2 * PI / branches);
			//float newX = 0;
			//float newY = 0;
			vec3 newPosition = position+direction;
			vec3 newDirection = direction+ vec3(newX, newY, 0);


			float angle = acos(dot(direction, newDirection)/(length(direction)*length(newDirection)));
			//glPushMatrix();
			//glTranslatef(newPosition.x, newPosition.y, newPosition.z);
			//glutSolidCone(width, length(newDirection), 15, 15);
			//glPopMatrix();
			treeRecurse(current->getChildren(), newPosition, newDirection, width*shrink, depth-1);
		}

	}
	return currentNode;
}


void drawUpsideDownCone(float base, float height, float slices, float stack)
{

	glPushMatrix();

	glTranslatef(0,0,height);
	glRotatef(180,1,1,0);
	glutSolidCone(base, height, slices, stack);

	glPopMatrix();
}

void treeDraw(Node *currentNode)
{
	if(currentNode != NULL)
	{
		

		Node *children = currentNode->getChildren();
		Node *currentChild = children;
		for(int i=0; i<currentNode->getNumChildren()-1; i++)
		{

			glPushMatrix();
			vec3 direction = currentNode->getDirection();
			//gluLookAt(currentNode->getX(), currentNode->getY(), currentNode->getZ(), currentNode->getX()-1, currentNode->getY()-1, currentNode->getZ()-1, direction.x, direction.y, direction.z);
			
			float angle = acos(dot(currentNode->getDirection(), currentChild->getDirection())/(length(currentNode->getDirection())*length(currentChild->getDirection()))) * (180)/PI;
			
			vec3 orthogVec = cross(currentNode->getDirection(), currentChild->getDirection());
			glTranslatef(currentChild->getX(), currentChild->getY(), currentChild->getZ());
			glRotatef(angle, orthogVec.x, orthogVec.y, orthogVec.z);
			drawUpsideDownCone(currentNode->getWidth(), currentNode->getHeight(), 15, 15);
			glPopMatrix();


			treeDraw(currentChild);
			currentChild = currentChild->getSiblings();
		}
	}
}

Node *myTree;

void buildMyTree()
{
	float startWidth=1;
	float startHeight=1;
	int branches=3;
	int depth=3;
	int shrink=0.5;

	
//	Node *head = new Node(startWidth,startHeight);
	Node *head = new Node(vec3(0,0,0), vec3(0,0,3), startWidth);
	Node *current = head;

	//glRotatef(100*spinAngle + 270, 1.0, 0.0, 0.0);

	myTree = treeRecurse(current, vec3(0,0,0), vec3(0,0,3), startWidth, depth);
	
	
	//head->addChild(1, 1);
	//head->addChild(2, 2);
	//head->addChild(3, 3);
	int x = 6;
}


void drawTriangle(float *v1, float *v2, float *v3) 
{
	glBegin(GL_TRIANGLES); 
	glNormal3fv(v1); 
	glVertex3fv(v1);    
	glNormal3fv(v2); 
	glVertex3fv(v2);    
	glNormal3fv(v3); 
	glVertex3fv(v3);    
	glEnd(); 
}

void subdivide(vec3 v1, vec3 v2, vec3 v3, float r, long depth)
{
	
	vec3 v12 = vec3();
	vec3 v23 = vec3();
	vec3 v31 = vec3();
	GLint i;

	if (depth == 0)
	{
		drawTriangle(v1, v2, v3);
		return;
	}

	for (int i=0; i<3; i++)
	{
		v12[i] = (v1[i]+v2[i])/2.0f;
		v23[i] = (v2[i]+v3[i])/2.0f;
		v31[i] = (v3[i]+v1[i])/2.0f;
	}


	//Normalize the midpoint vectors and multiply them by the radius
	//This controls the size of the sphere

	v12 = (v12/length(v12)) * r;
	v23 = (v23/length(v23)) * r;
	v31 = (v31/length(v31)) * r;

	subdivide(v1, v12, v31, r, depth-1);
	subdivide(v2, v23, v12, r, depth-1);
	subdivide(v3, v31, v23, r, depth-1);
	subdivide(v12, v23, v31, r, depth-1);
}



void drawSphere(float x, float y, float z, float r)
{
	glPushMatrix();
	glTranslatef(x,y,z);

	float depth=3;
	vec3 v1;
	vec3 v2;
	vec3 v3;

	//Each set of this is one quadrant (8 in 3D)
	v1 = vec3(r, 0, 0);
	v2 = vec3(0, r, 0);
	v3 = vec3(0, 0, r);
	subdivide(v1, v2, v3, r, depth);

	v1 = vec3(-r, 0, 0);
	v2 = vec3(0, r, 0);
	v3 = vec3(0, 0, r);
	subdivide(v1, v2, v3, r, depth);

	v1 = vec3(r, 0, 0);
	v2 = vec3(0, -r, 0);
	v3 = vec3(0, 0, r);
	subdivide(v1, v2, v3, r, depth);

	v1 = vec3(r, 0, 0);
	v2 = vec3(0, r, 0);
	v3 = vec3(0, 0, -r);
	subdivide(v1, v2, v3, r, depth);

	v1 = vec3(-r, 0, 0);
	v2 = vec3(0, -r, 0);
	v3 = vec3(0, 0, r);
	subdivide(v1, v2, v3, r, depth);

	v1 = vec3(r, 0, 0);
	v2 = vec3(0, -r, 0);
	v3 = vec3(0, 0, -r);
	subdivide(v1, v2, v3, r, depth);

	v1 = vec3(-r, 0, 0);
	v2 = vec3(0, r, 0);
	v3 = vec3(0, 0, -r);
	subdivide(v1, v2, v3, r, depth);

	v1 = vec3(-r, 0, 0);
	v2 = vec3(0, -r, 0);
	v3 = vec3(0, 0, -r);
	subdivide(v1, v2, v3, r, depth);

	glPopMatrix();
}


void setLighting()
{
	if(key['g']){
		lightPos0[1] += 0.1;
	}
	if(key['b']){
		lightPos0[1] -= 0.1;

	}
	if(key['v']){
		lightPos0[0] -= 0.1;
	}
	if(key['n']){
		lightPos0[0] += 0.1;
	}

	if(key['r']){
		lightPos0[2] -= 0.1;
	}
	if(key['t']){
		lightPos0[2] += 0.1;
	}
	
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glColor3f(1.0, 1.0, 1.0);
	drawSphere(lightPos0[0], lightPos0[1], lightPos0[2], 0.2);
}


void optionsMenu(int value)
{
	if(value == 2)
	{
	glEnable(GL_LIGHT0);
	lightColor0[0] = lightColor0[1] = lightColor0[2] = lightColor0[3] = 1.0f;
	}
	if(value == 3)
	{
	glEnable(GL_LIGHT0);
	lightColor0[0] = lightColor0[1] = lightColor0[2] = lightColor0[3] = 0.7f;
	}
	if(value == 4)
	{
	glEnable(GL_LIGHT0);
	lightColor0[0] = lightColor0[1] = lightColor0[2] = lightColor0[3] = 0.3f;
	}
	if(value == 5)
	{
	glDisable(GL_LIGHT0);
	}
	if(value == 6)
		exit(0);
}

void createMenu()
{
	int submenu, menu;
	submenu = glutCreateMenu(optionsMenu);
	glutAddMenuEntry("High", 2);
	glutAddMenuEntry("Medium", 3);
	glutAddMenuEntry("Low", 4);
	glutAddMenuEntry("Off", 5);

	menu = glutCreateMenu(optionsMenu);
	glutAddSubMenu("Lighting", submenu);
	glutAddMenuEntry("Quit", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

float zoom_x = 0;
float zoom_y = 0;
float zoom_z = 0;


void setCamera()
{
	if(key['j']){
		cam_x += ROT_AMOUNT;
	}
	if(key['u']){
		cam_x -= ROT_AMOUNT;
	}
	if(key['h']){
		cam_y -= ROT_AMOUNT;
	}
	if(key['k']){
		cam_y += ROT_AMOUNT;
	}
	if(key['y']){
		cam_z -= ROT_AMOUNT;
	}
	if(key['i']){
		cam_z += ROT_AMOUNT;
	}


	
	if(key['d']){
		zoom_x += PAN_AMOUNT;
	}
	if(key['a']){
		zoom_x -= PAN_AMOUNT;
	}
	if(key['w']){
		zoom_y += PAN_AMOUNT;
	}
	if(key['s']){
		zoom_y -= PAN_AMOUNT;
	}
	if(key['e']){
		zoom_z += PAN_AMOUNT;
	}
	if(key['q']){
		zoom_z -= PAN_AMOUNT;
	}
	

	
	glTranslatef(zoom_x, zoom_y, zoom_z);
	//we can definitely improve the functionality using actual rotations, but for now, this works. 
	
			  
}

void loadSettings(Setting_list *settings)
{
	//setting up the reader
	typedef INI <string, string, string> ini_t;
	ini_t ini("./settings.ini");
	
	//Various non-player related settings
	ini.select("OPENGL");
	settings->GLUT_UPDATE_MILLIS = 20; //ini.get<int>("GLUT_UPDATE_MILLIS", 20);
	settings->ZOOM_VALUE = -15.0;//ini.get<float>("ZOOM_VALUE", -15.0);
	//Player-related Settings.
	ini.select("PLAYER");
	settings->PLAYER_ACCELERATION = ini.get<float>("PLAYER_ACCELERATION", 0.01);
	settings->PLAYER_FRICTION = ini.get<float>("PLAYER_FRICTION", 0.95);
	settings->PLAYER_MAX_RADIUS = ini.get<float>("PLAYER_MAX_RADIUS",0.5);
	settings->PLAYER_MAX_SPEED = ini.get<float>("PLAYER_MAX_SPEED", 0.5);

	ini.select("OPTIONS");
	settings->PLAYER_STARTING_BUDGET = ini.get<float>("PLAYER_STARTING_BUDGET", 10.0);
	settings->PILL_COUNT = ini.get<int>("PILL_COUNT", 5);
	settings->PILL_SIZE = ini.get<float>("PILL_SIZE", 0.2);
}


void resetKeyboardInput(void)
{
    int        i;
    
    for (i = 0; i < NUM_KEY_CODES; i++)
    {
        keyDown[i] = false;
        keyUp[i] = false;
    }
}

void keyboard(unsigned char rawKeyCode, int x, int y)
{
	switch (rawKeyCode)
    {
		case 'w':
			key['w'] = true;
            keyDown['w'] = true;
            break;
		case 'a':
			key['a'] = true;
            keyDown['a'] = true;
            break;
		case 'q':
			key['q'] = true;
            keyDown['q'] = true;
            break;
		case 'e':
			key['e'] = true;
            keyDown['e'] = true;
            break;
		case 's':
			key['s'] = true;
            keyDown['s'] = true;
            break;
		case 'd':
			key['d'] = true;
            keyDown['d'] = true;
            break;
		case 'f':
			key['f'] = true;
            keyDown['f'] = true;
            break;
		case '/':
			key['/'] = true;
            keyDown['/'] = true;
            break;
		case 'h':
			//camera left
			key['h'] = true;
			keyDown['h'] = true;
			break;
		case 'k':
			//camera right
			key['k'] = true;
			keyDown['k'] = true;
			break;
		case 'u':
			//camera up
			key['u'] = true;
			keyDown['u'] = true;
			break;
		case 'j':
			//camera down
			key['j'] = true;
			keyDown['j'] = true;
			break;

		case 'g':
			//light source up
			key['g'] = true;
			keyDown['g'] = true;
			break;
		case 'b':
			//light source down
			key['b'] = true;
			keyDown['b'] = true;
			break;
		case 'v':
			//light source left
			key['v'] = true;
			keyDown['v'] = true;
			break;
		case 'n':
			//light source right
			key['n'] = true;
			keyDown['n'] = true;
			break;
		case 'y':
			//light source right
			key['y'] = true;
			keyDown['y'] = true;
			break;
		case 'i':
			//light source right
			key['i'] = true;
			keyDown['i'] = true;
			break;
		case 'r':
			//light source right
			key['r'] = true;
			keyDown['r'] = true;
			break;
		case 't':
			//light source right
			key['t'] = true;
			keyDown['t'] = true;
			break;
	}
}

void keyboardUp(unsigned char rawKeyCode, int x, int y)
{
	switch (rawKeyCode)
    {
		case 'w':
			key['w'] = false;
            keyUp['w'] = false;
            break;
		case 'a':
			key['a'] = false;
            keyUp['a'] = false;
            break;
		case 'q':
			key['q'] = false;
            keyUp['q'] = false;
            break;
		case 'e':
			key['e'] = false;
            keyUp['e'] = false;
            break;
		case 's':
			key['s'] = false;
            keyUp['s'] = false;
            break;
		case 'd':
			key['d'] = false;
            keyUp['d'] = false;
            break;
		case 'f':
			key['f'] = false;
            keyUp['f'] = false;
            break;
		case '/':
			key['/'] = false;
            keyUp['/'] = false;
            break;
		case 'h':
			//camera left
			key['h'] = false;
			keyDown['h'] = false;
			break;
		case 'k':
			//camera right
			key['k'] = false;
			keyDown['k'] = false;
			break;
		case 'u':
			//camera up
			key['u'] = false;
			keyDown['u'] = false;
			break;
		case 'j':
			//camera down
			key['j'] = false;
			keyDown['j'] = false;
			break;
		case 'g':
			//light source up
			key['g'] = false;
			keyDown['g'] = false;
			break;
		case 'b':
			//light source down
			key['b'] = false;
			keyDown['b'] = false;
			break;
		case 'v':
			//light source left
			key['v'] = false;
			keyDown['v'] = false;
			break;
		case 'n':
			//light source right
			key['n'] = false;
			keyDown['n'] = false;
			break;
		case 'y':
			//light source right
			key['y'] = false;
			keyDown['y'] = false;
			break;
		case 'i':
			//light source right
			key['i'] = false;
			keyDown['i'] = false;
			break;
		case 'r':
			//light source right
			key['r'] = false;
			keyDown['r'] = false;
			break;
		case 't':
			//light source right
			key['t'] = false;
			keyDown['t'] = false;
			break;
	}
}

void keyboardSpecial(int rawKeyCode, int x, int y)
{
    switch (rawKeyCode)
    {
        case GLUT_KEY_LEFT:
            key[LEFT_ARROW] = true;
            keyDown[LEFT_ARROW] = true;
            break;
        case GLUT_KEY_UP:
            key[UP_ARROW] = true;
            keyDown[UP_ARROW] = true;
            break;
        case GLUT_KEY_RIGHT:
            key[RIGHT_ARROW] = true;
            keyDown[RIGHT_ARROW] = true;
            break;
        case GLUT_KEY_DOWN:
            key[DOWN_ARROW] = true;
            keyDown[DOWN_ARROW] = true;
            break;
    }
}

void keyboardSpecialUp(int rawKeyCode, int x, int y)
{
    switch (rawKeyCode)
    {
        case GLUT_KEY_LEFT:
            key[LEFT_ARROW] = false;
            keyUp[LEFT_ARROW] = true;
            break;
        case GLUT_KEY_UP:
            key[UP_ARROW] = false;
            keyUp[UP_ARROW] = true;
            break;
        case GLUT_KEY_RIGHT:
            key[RIGHT_ARROW] = false;
            keyUp[RIGHT_ARROW] = true;
            break;
        case GLUT_KEY_DOWN:
            key[DOWN_ARROW] = false;
            keyUp[DOWN_ARROW] = true;
            break;
    }
}

//Initializes 3D rendering - I guess this is useful for the rotating line
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	t3dInit();
	//glShadeModel(GL_SMOOTH); //Enable smooth shading
}

//Called when the window is resized
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}





float _cameraAngle = 0.0f;

void renderStringToWindow(string s)
{
	void *font = GLUT_BITMAP_HELVETICA_10;
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}


void drawTree()
{
	
	glColor3f(0.3, 0.3, 0.3);
	
	//glPushMatrix();
	glTranslatef(0,0,0);
	
	//glPopMatrix();
}



//Draws the scene
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	setCamera();
	//gluLookAt(0, 
	//		  -1,
	//		  15,  //first three vars are for camera location
	//		  0.0,0.0,0.0, //2nd three vars are for where the camera is POINTING
	//		  0.0,0.0,1.0);  //third vars indicate the axis of rotation
	glTranslatef(0.0f, 0.0f, settings->ZOOM_VALUE); //Move forward 5 units
	glColor3f(1.0, 1.0, 1.0);
    
	setLighting();
    
	glColor3f(0.0, 0.0, 1.0);

	
	//drawTree();
	glPushMatrix();
	glTranslatef(0, 0, -10);
	glRotatef(cam_x, 1, 0, 0);
	glRotatef(cam_y, 0, 1, 0);
	glRotatef(cam_z, 0, 0, 1);
	treeDraw(myTree);
	glPopMatrix();

	
	//glNormal3f(0.0f, 0.0f, 1.0f);



	//Draws the budget. Need to clean this up a bit
	
	/*
	glPushMatrix();
	
	
	stringstream  convert;
	convert << "Current Budget: " << std::setprecision(3) <<*budget <<'\0';
	string s = convert.str();

	void * font = GLUT_BITMAP_HELVETICA_10;
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}


	glScalef(_scale, _scale, _scale);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(-17, 12.0, 0.0f / _scale);
	t3dDraw3D(s, -1, 0, 0.2f, 1.5f);
	glPopMatrix();

	*/
    /*
	convert.str("");
	convert << "Player 1: " << player1->getScore();
	s = convert.str();
	glPushMatrix();
	glTranslatef(-2, 12.0, 0.0f / _scale);
	t3dDraw3D(s, -1, 0, 0.2f, 1.5f);
	glPopMatrix();

	convert.str("");
	convert << "Player 2: " << player2->getScore();
	s = convert.str();
	glPushMatrix();
	glTranslatef(8, 12.0, 0.0f / _scale);
	t3dDraw3D(s, -1, 0, 0.2f, 1.5f);
	glPopMatrix();
	*/

	glPopMatrix();

	glutSwapBuffers();
}

void update(int value) {

	
	glutPostRedisplay(); //Tell GLUT that the display has changed
	
	//Tell GLUT to call update again in 'GLUT_UPDATE_MILLIS' milliseconds
	glutTimerFunc(settings->GLUT_UPDATE_MILLIS, update, 0);
}

void initFieldObjects()
{
}


int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);


	settings = new Setting_list;
	loadSettings(settings);
	cam_z = settings->ZOOM_VALUE;
	budget = new float(settings->PLAYER_STARTING_BUDGET);//settings->PLAYER_STARTING_BUDGET;

	initFieldObjects();
	
	buildMyTree();

	glutCreateWindow("PREDATOR VS. PREY 2 - MACHINE FOR PIGS");
	initRendering();

	//Set handler functions
	glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(keyboardSpecial);
    glutSpecialUpFunc(keyboardSpecialUp);
	createMenu();

	glutReshapeFunc(handleResize);
	
	glutTimerFunc(settings->GLUT_UPDATE_MILLIS, update, 0); //Add a timer
	
	glutMainLoop();
	return 0;
}