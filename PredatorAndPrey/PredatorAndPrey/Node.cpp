#include "Node.h"
#include <iostream>

Node::Node()
{
	
	this->width = 0;
	this->height = 0;
	this->numChildren = 0;
	this->children = NULL;
}
Node::Node(float height, float width)
{
	
	this->width = width;
	this->height = height;
	this->numChildren = 0;
	this->children = NULL;
}

Node::Node(vec3 position, vec3 direction, float width)
{
	this->width = width;
	this->position = position;
	this->direction = direction;
	this->children = NULL;
	this->numChildren = 0;

}

void Node::addChild(Node *node)
{
	if(this->children == NULL)
	{
		this->children = node;
	}
}

void Node::addChild(float width, float height)
{
	if(this->children == NULL)
	{
		this->children = new Node(width, height);
		this->children->sibling = NULL;
	}
	else
	{
		Node *placeholder = this->children;
		for(int i=0; i<this->getNumChildren()-1; i++)
			placeholder = placeholder->sibling;
		placeholder->sibling = new Node(width, height);
	}

	this->numChildren++;
}


void Node::addChild(vec3 position, vec3 direction, float width)
{
	if(this->children == NULL)
	{
		this->children = new Node(position, direction, width);
		this->children->sibling = NULL;
	}
	else
	{
		Node *placeholder = this->children;
		for(int i=0; i<this->getNumChildren()-1; i++)
			placeholder = placeholder->sibling;
		placeholder->sibling = new Node(position, direction, width);
	}

	this->numChildren++;
}


//float Node::getHeight()
//{
//	return this->height;
//}
float Node::getHeight()
{
	return length(direction);
}
float Node::getWidth()
{
	return this->width;
}
int Node::getNumChildren()
{
	return this->numChildren;
}
Node* Node::getChildren()
{
	return this->children;
}
Node* Node::getSiblings()
{
	return this->sibling;
}


float Node::getX()
{
	return this->position.x;
}
float Node::getY()
{
	return this->position.y;
}
float Node::getZ()
{
	return this->position.z;
}
vec3 Node::getPosition()
{
	return this->position;
}
vec3 Node::getDirection()
{
	return this->direction;
}