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
#include "mat.h"
#include "PushButton.h"
#include "Slider.h"
#include <time.h>

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



float spinAngle = 0;
float spinSpeed = 0;

//camera starting settings
float cam_x = 0.0;
float cam_y = -1.0;
float cam_z = 0.0;

//lighting settings
GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //white light
GLfloat lightPos0[] = {0.0f, 0.0f, 1.5f, 1.0f}; 


float _scale = 0.9f;

Setting_list *settings;


//----------GLOBAL VARIABLES------------

float treeR, treeG, treeB;
float widthShrink, lengthShrink;
int startDepth, branches;
float startWidth, startHeight;
int randomOn;




Slider *slider_treeR = new Slider(2, 100, 100, 30,"Tree R", 0, 10);
Slider *slider_treeG = new Slider(2, 150, 100, 30,"Tree G", 0, 10);
Slider *slider_treeB = new Slider(2, 200, 100, 30,"Tree B1", 0, 10);

Slider *slider_startWidth = new Slider(2, 250, 100, 30,"Start W", 0, 10);
Slider *slider_startHeight = new Slider(2, 300, 100, 30,"Start H", 0, 10);

Slider *slider_startDepth = new Slider(2, 350, 100, 30,"Start Depth", 0, 10);
Slider *slider_branches = new Slider(2, 400, 100, 30,"Branches", 0, 10);

Slider *slider_widthShrink = new Slider(2, 450, 100, 30,"W Shrink", 0, 10);
Slider *slider_lengthShrink = new Slider(2, 500, 100, 30,"H Shrink", 0, 10);



PushButton *button_update = new PushButton(110, 250, 50, 30, "Update");
PushButton *button4 = new PushButton(110, 250, 50, 30, "Random Num");

PushButton *button5 = new PushButton(110, 300, 50, 30, "Random Dir");




PushButton *button6 = new PushButton(110, 350, 50, 30, "Random Len");


Slider *slider_randomOn = new Slider(2, 550, 100, 30,"Randomness", 0, 2);

void setGlobalsFromSliders()
{
	treeR = (float)(slider_treeR->getValue())/10;
	treeG = (float)(slider_treeG->getValue())/10;
	treeB = (float)(slider_treeB->getValue())/10;
	

	startWidth = (float)(slider_startWidth->getValue())/10;
	startHeight = slider_startHeight->getValue();
	
	widthShrink = 1.1 - (float)(slider_widthShrink->getValue()) / 10;
	lengthShrink = 1.1 - (float)(slider_lengthShrink->getValue()) / 10;


	startDepth = slider_startDepth->getValue();
	branches = slider_branches->getValue();
	
	randomOn = 2 - slider_randomOn->getValue();
}


void initGlobals()
{
	treeR = 1;
	treeG = 1;
	treeB = 1;
	startWidth = 0.3;
	startHeight = 3;
	widthShrink = 0.9;
	lengthShrink = 0.9;
	startDepth = 3;
	branches = 5;
}



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



bool left_mouse;

void mouse(int button, int state, int x, int y)
{
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	float xFraction = WINDOW_WIDTH / windowWidth;
	float yFraction = WINDOW_HEIGHT / windowHeight;

	int mouseX = x * xFraction;
	int mouseY = y * yFraction;
	//The above is for correction in case of window resizing

	if(state == 0) //Down
	{
		button_update->mouseDown(mouseX, mouseY);
		button5->mouseDown(mouseX, mouseY);
		button6->mouseDown(mouseX, mouseY);
		slider_treeR->mouseDown(mouseX, mouseY);
		slider_treeG->mouseDown(mouseX, mouseY);
		slider_treeB->mouseDown(mouseX, mouseY);


		
		slider_startWidth->mouseDown(mouseX, mouseY);
		slider_startHeight->mouseDown(mouseX, mouseY);

		slider_startDepth->mouseDown(mouseX, mouseY);
		slider_branches->mouseDown(mouseX, mouseY);

		slider_widthShrink->mouseDown(mouseX, mouseY);
		slider_lengthShrink->mouseDown(mouseX, mouseY);



		
		slider_randomOn->mouseDown(mouseX, mouseY);

		left_mouse = true;
	}
	else if(state == 1) //Up
	{
		button_update->mouseUp(mouseX, mouseY);
		button5->mouseUp(mouseX, mouseY);
		button6->mouseUp(mouseX, mouseY);
		slider_treeR->mouseUp(mouseX, mouseY);
		slider_treeG->mouseUp(mouseX, mouseY);
		slider_treeB->mouseUp(mouseX, mouseY);

		slider_startWidth->mouseUp(mouseX, mouseY);
		slider_startHeight->mouseUp(mouseX, mouseY);

		slider_startDepth->mouseUp(mouseX, mouseY);
		slider_branches->mouseUp(mouseX, mouseY);

		slider_widthShrink->mouseUp(mouseX, mouseY);
		slider_lengthShrink->mouseUp(mouseX, mouseY);


		slider_randomOn->mouseDown(mouseX, mouseY);
		
		left_mouse = false;
	}
}

void onMotion(int x, int y)
{
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	float xFraction = WINDOW_WIDTH / windowWidth;
	float yFraction = WINDOW_HEIGHT / windowHeight;

	int mouseX = x * xFraction;
	int mouseY = y * yFraction;

	if(left_mouse)
	{
		slider_treeR->mouseDrag(mouseX, mouseY);
		slider_treeG->mouseDrag(mouseX, mouseY);
		slider_treeB->mouseDrag(mouseX, mouseY);

		
		slider_startWidth->mouseDrag(mouseX, mouseY);
		slider_startHeight->mouseDrag(mouseX, mouseY);

		slider_startDepth->mouseDrag(mouseX, mouseY);
		slider_branches->mouseDrag(mouseX, mouseY);

		slider_widthShrink->mouseDrag(mouseX, mouseY);
		slider_lengthShrink->mouseDrag(mouseX, mouseY);


		slider_randomOn->mouseDrag(mouseX, mouseY);


	}
}


float incidenceAngle(vec3 v1, vec3 v2)
{
	return acos(dot(v1, v2)/(length(v1)*length(v2))) * (180)/PI;

}


Node *treeRecurse(Node *currentNode, float width, int depth)
{
	//int branches=4;
	//float spread = 2;
	Node *current = currentNode;
	//for(int i=0; i<depth; i++)
	vec3 newPosition;

	
	if(depth>0)
	{
		
		for(int j=0; j<branches; j++)
		{
			float newX = cos((2 * PI) * (((float)j) / (float)branches)) ;
			float newY = sin((2 * PI) * (((float)j) / (float)branches)) ;
			
			float newZ = 0;

			int p=rand();
			if(randomOn)
			{
				newX *= (rand() % 10 + 1);
				newY *= (rand() % 10 + 1);
			}

			vec3 change;
			vec3 newDirection;
			
			vec3 nextPosition = vec3(currentNode->getPosition().x+currentNode->getDirection().x, currentNode->getPosition().y+currentNode->getDirection().y, currentNode->getPosition().z + currentNode->getDirection().z);
			
			vec3 nextDirection = currentNode->getDirection() ;
			
			change = normalize(vec3(newX, newY, newZ));
			newDirection = nextDirection+normalize(change);
			newDirection = normalize(newDirection) * currentNode->getHeight() * lengthShrink;

			Node *nextBranch = currentNode->addChild(newPosition, newDirection, width*widthShrink);
			treeRecurse(nextBranch, width*widthShrink, depth-1);
		}

	}
	return currentNode;
}


void drawUpsideDownCone(float base, float height, float slices, float stack)
{

	glPushMatrix();
	//glTranslatef(0,0,height);
	//glRotatef(180,1,1,1);
	glutSolidCone(base, height, slices, stack);

	glPopMatrix();
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



void drawSphere(float x, float y, float z, float r, int depth)
{
	glPushMatrix();
	glTranslatef(x,y,z);

	//float depth=3;
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



void treeDraw(Node *currentNode)
{
	if(currentNode)
	{
		
		glColor3f(treeR, treeG, treeB);
		drawUpsideDownCone(currentNode->getWidth(), currentNode->getHeight(), 15, 15);

		float angle;
		vec3 orthogVec;

			
		glTranslatef(0, 0, currentNode->getHeight());
			

		Node *children = currentNode->getChildren();
		Node *currentChild = children;

		while(currentChild)
		{
				
			glPushMatrix();
			angle = incidenceAngle(currentChild->getDirection(), vec3(0, 0, 1));
			orthogVec = cross(currentChild->getDirection(), vec3(0, 0, 1));
			glRotatef(angle, orthogVec.x, orthogVec.y, orthogVec.z);

			treeDraw(currentChild);
				
			glPopMatrix();
			currentChild = currentChild->getSiblings();

		}
		if(currentNode->getNumChildren() == 0)
		{
			glColor3f(0, 0.6, 0.2);
			drawSphere(0, 0, 0, 1, 1);
		}
	}
}

Node *myTree;

void buildMyTree()
{
	//float startHeight=3;
	//int branches=2;
	//int depth=3;

	setGlobalsFromSliders();
//	Node *head = new Node(startWidth,startHeight);
	Node *head = new Node(vec3(0,0,0), vec3(0,0,startHeight), startWidth);
	Node *current = head;

	myTree = treeRecurse(current, startWidth, startDepth);
	
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
	drawSphere(lightPos0[0], lightPos0[1], lightPos0[2], 0.2, 3);

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
/*
void renderStringToWindow(string s)
{
	void *font = GLUT_BITMAP_TIMES_ROMAN_24;
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}
*/


void drawTree()
{
	
	glColor3f(0.3, 0.3, 0.3);
	
	//glPushMatrix();
	glTranslatef(0,0,0);
	
	//glPopMatrix();
}


void setButtonActions()
{
	button_update->setAction(buildMyTree);
	button5->setAction(buildMyTree);
	button6->setAction(buildMyTree);
	slider_branches->setAction(buildMyTree);
	slider_startDepth->setAction(buildMyTree);
	slider_widthShrink->setAction(buildMyTree);
	slider_lengthShrink->setAction(buildMyTree);
	slider_startHeight->setAction(buildMyTree);
	slider_startWidth->setAction(buildMyTree);
	slider_treeB->setAction(buildMyTree);
	slider_treeG->setAction(buildMyTree);
	slider_treeR->setAction(buildMyTree);

}

void renderStringToWindow(string str, int x, int y)
{
	
	//glDisable(GL_LIGHTING);
	glColor3f(0.0, 0.0, 0.0);
	
	glRasterPos2i(x, y);
	stringstream  convert;
	convert << str <<'\0';
	string s = convert.str();
	void *font = GLUT_BITMAP_9_BY_15;
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
	//glEnable(GL_LIGHTING);
}

void drawButton(PushButton *button)
{

	glPushMatrix();
	glTranslatef(button->getX(), button->getY(), -5.0f);

	glBegin(GL_QUADS);
	glColor3f(button->getColor().x, button->getColor().y, button->getColor().z);
	
	//glColor3f(1.0f, 1.0f, 1.0f);

	glVertex2f(0, 0);
	glVertex2f(0 + button->getWidth(), 0);
	glVertex2f(0 + button->getWidth(), 0 + button->getHeight());
	
	glVertex2f(0, 0 + button->getHeight());


	/*
	glVertex2f(button->getX(), button->getY());
	glVertex2f(button->getX() + button->getWidth(), button->getY());
	glVertex2f(button->getX() + button->getWidth(), button->getY() + button->getHeight());
	
	glVertex2f(button->getX(), button->getY() + button->getHeight());
	*/
	glEnd();

	glPopMatrix();

	
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	renderStringToWindow(button->getLabel(), button->getX(), button->getY() + (button->getHeight() / 2));
}



void drawSlider(Slider *slider)
{

	glPushMatrix();
	glTranslatef(slider->getX(), slider->getY(), -5.0f);

	glBegin(GL_QUADS);

	
	glColor3f(1.0, 1.0, 1.0);
	
	glVertex2f(slider->sliderBarPos(), 0);
	glVertex2f(slider->sliderBarPos() + slider->getSliderBarWidth(), 0);
	glVertex2f(slider->sliderBarPos() + slider->getSliderBarWidth(), 0 + slider->getHeight());
	glVertex2f(slider->sliderBarPos(), 0 + slider->getHeight());



	glColor3f(slider->getColor().x, slider->getColor().y, slider->getColor().z);
	glVertex2f(0, 0);
	glVertex2f(slider->getWidth(), 0);
	glVertex2f(slider->getWidth(), slider->getHeight());
	glVertex2f(0, slider->getHeight());


	/*
	glVertex2f(button->getX(), button->getY());
	glVertex2f(button->getX() + button->getWidth(), button->getY());
	glVertex2f(button->getX() + button->getWidth(), button->getY() + button->getHeight());
	
	glVertex2f(button->getX(), button->getY() + button->getHeight());
	*/
	glEnd();
	glPopMatrix();

	
	
	

	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	renderStringToWindow(slider->getLabel(), slider->getX(), slider->getY());
	//renderStringToWindow(to_string(slider->getValue()), slider->getX() + slider->getWidth() - 9, slider->getY());
	renderStringToWindow(to_string(slider->getValue()), slider->getX() + slider->sliderBarPos(), slider->getY() + slider->getHeight() - 5);

	

	
}




void draw2Dthings()
{
	glDisable(GL_LIGHTING);
	
		glPushMatrix();
		glTranslatef(0, 0, -5.1f);
		glBegin(GL_QUADS);
			glColor3f(238.0 / 255.0,223 / 255.0, 204.0 / 255.0);
			glVertex2f(0.0, 0.0);
			glVertex2f(105.0, 0.0);
			glVertex2f(105, 500);
			glVertex2f(0.0, 500);
		glEnd();
	glPopMatrix();
	
	drawButton(button_update);
	drawButton(button5);
	drawButton(button6);
	drawSlider(slider_treeR);
	drawSlider(slider_treeG);
	drawSlider(slider_treeB);

	drawSlider(slider_startWidth);
	drawSlider(slider_startHeight);
	drawSlider(slider_startDepth);
	drawSlider(slider_branches);
	drawSlider(slider_widthShrink);
	drawSlider(slider_lengthShrink);
	drawSlider(slider_randomOn);



	glEnable(GL_LIGHTING);
}





GLfloat lightPos1[] = {0.0f, 0.0f, 15.0f, 1.0f}; 
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


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//glOrtho(-(2*GLUT_SCREEN_WIDTH/GLUT_SCREEN_HEIGHT), (2*GLUT_SCREEN_WIDTH/GLUT_SCREEN_HEIGHT), 2, 2, -15.0, 15.0);
	glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, -15.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);
	


	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();

	
	
	
	
	glPopMatrix();




	glNormal3f(0.0f, 0.0f, 1.0f);
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	draw2Dthings();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

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

	srand(time(NULL));
	settings = new Setting_list;
	loadSettings(settings);
	cam_z = settings->ZOOM_VALUE;


	initGlobals();
	setButtonActions();

	initFieldObjects();
	
	//buildMyTree();

	glutCreateWindow("PREDATOR VS. PREY 2 - MACHINE FOR PIGS");
	initRendering();

	//Set handler functions
	glutDisplayFunc(drawScene);

	glutMouseFunc(mouse);
    glutMotionFunc(onMotion);


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