#include "TravelNode.h"
using namespace Simplex;

void Simplex::TravelNode::Release(void)
{
	weight = 0.0f;
	isStart = false;
	isEnd = false;
	SafeDelete(mesh);
}

void Simplex::TravelNode::Swap(TravelNode & other)
{
	std::swap(weight, other.weight);
	std::swap(isStart, other.isStart);
	std::swap(isEnd, other.isEnd);
	std::swap(location, other.location);
	mesh = new Mesh();
	mesh->GenerateSphere(weight, 6, color);
}

Simplex::TravelNode::TravelNode(float inWeight, vector3 inLocation)
{
	weight = inWeight;
	location = inLocation;
	mesh = new Mesh();
	mesh->GenerateSphere(weight, 6, color);
}

Simplex::TravelNode::TravelNode(TravelNode const & other)
{
	weight = other.weight;
	isStart = other.isStart;
	isEnd = other.isEnd;
	location = other.location;
	mesh = new Mesh();
	mesh->GenerateSphere(weight, 6, color);
}

TravelNode & Simplex::TravelNode::operator=(TravelNode const & other)
{
	if (this != &other)
	{
		Release();
		TravelNode temp(other);
		Swap(temp);
	}
	return *this;
}

Simplex::TravelNode::~TravelNode(void)
{
}

bool Simplex::TravelNode::compareLocation(TravelNode* other)
{
	vector3 inLocation = other->location;
	if (location.x == inLocation.x && location.y == inLocation.y && location.z == inLocation.z)
		return true;
	return false;
}

void Simplex::TravelNode::changeColor(vector3 newColor)
{
	color = newColor;
	mesh->GenerateSphere(weight, 6, color);
}
