#include "Node.h"
#include <iostream>

Node::Node()
{
	
	this->width = 0;
	this->height = 0;
	this->numChildren = 0;
	this->children = NULL;
	this->sibling = NULL;
}
Node::Node(float height, float width)
{
	
	this->width = width;
	this->height = height;
	this->numChildren = 0;
	this->children = NULL;
	this->sibling = NULL;
}

Node::Node(vec3 position, vec3 direction, float width)
{
	this->width = width;
	this->position = position;
	this->direction = direction;
	this->children = NULL;
	this->sibling = NULL;
	this->numChildren = 0;

}

void Node::addChild(Node *node)
{
	if(this->children == NULL)
	{
		this->children = node;
	}
}



Node* Node::addChild(vec3 position, vec3 direction, float width)
{
	if(this->children == NULL)
	{
		this->children = new Node(position, direction, width);
		this->children->sibling = NULL;
		
		this->children->setColor(this->getColor());
		this->children->setLeafColor(this->getLeafColor());
		this->children->setLeafSize(this->getLeafSize());

		return this->children;
	}
	else
	{
		Node *placeholder = this->children;
		while(placeholder->sibling)
		{
			placeholder = placeholder->sibling;
		}

		placeholder->sibling = new Node(position, direction, width);
		placeholder->sibling->setColor(this->getColor());
		placeholder->sibling->setLeafColor(this->getLeafColor());
		placeholder->sibling->setLeafSize(this->getLeafSize());
		return placeholder->sibling;
	}

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
	if(this->getChildren() == NULL)
		return 0;
	else
	{
		Node *current = this->getChildren();
		int n=1;
		while(current->sibling)
		{
			current = current->sibling;
			n++;
		}

		return n;
	}
}

/*
int Node::getNumChildren()
{
	return this->numChildren;
}*/
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


void Node::setColor(vec3 color)
{
	this->color = color;
}
void Node::setLeafColor(vec3 leafColor)
{
	this->leafColor = leafColor;
}

void Node::setLeafSize(float leafSize)
{
	this->leafSize = leafSize;
}
float Node::getLeafSize()
{
	return this->leafSize;
}

vec3 Node::getColor()
{
	return this->color;
}
vec3 Node::getLeafColor()
{
	return this->leafColor;
}