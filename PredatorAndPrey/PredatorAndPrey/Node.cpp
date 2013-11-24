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