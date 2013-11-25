#include "vec.h"


using namespace Angel;

class Node
{
	
	float width, height;
	int numChildren;
	Node *children;
	Node *sibling;
	vec3 position;
	vec3 direction;
	vec3 color;
	vec3 leafColor;
	float leafSize;

public:
	Node();
	Node(float height, float width);
	Node(vec3 position, vec3 direction, float width);

	
	void addChild(Node *node);
	void addChild(float height, float width);
	Node* addChild(vec3 position, vec3 direction, float width);
	
	Node* getChildren();
	Node* getSiblings();

	
	int getNumChildren();
	float getWidth();
	float getHeight();
	float getX();
	float getY();
	float getZ();
	vec3 getPosition();
	vec3 getDirection();
	
	void setColor(vec3 color);
	void setLeafColor(vec3 leafColor);
	
	vec3 getColor();
	vec3 getLeafColor();
	float getLeafSize();
	void setLeafSize(float leafSize);

};
