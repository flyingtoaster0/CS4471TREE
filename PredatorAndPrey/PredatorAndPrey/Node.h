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

public:
	Node();
	Node(float height, float width);
	Node(vec3 position, vec3 direction, float width);

	
	void addChild(Node *node);
	void addChild(float height, float width);
	void addChild(vec3 position, vec3 direction, float width);
	
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

};
