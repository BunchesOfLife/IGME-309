#include "TravelNode.h"
using namespace Simplex;

void Simplex::TravelNode::Release(void)
{
	weight = 0.0f;
	isStart = false;
	isEnd = false;
	links.clear();
	SafeDelete(mesh);
}

void Simplex::TravelNode::Swap(TravelNode & other)
{
	std::swap(weight, other.weight);
	std::swap(isStart, other.isStart);
	std::swap(isEnd, other.isEnd);
	std::swap(links, other.links);
	std::swap(location, other.location);
	mesh = new Mesh();
	mesh->GenerateSphere(weight, 6, C_GREEN);
}

Simplex::TravelNode::TravelNode(float inWeight, vector3 inLocation)
{
	weight = inWeight;
	location = inLocation;
	mesh = new Mesh();
	mesh->GenerateSphere(weight, 6, C_GREEN);
}

Simplex::TravelNode::TravelNode(TravelNode const & other)
{
	weight = other.weight;
	isStart = other.isStart;
	isEnd = other.isEnd;
	location = other.location;
	for (TravelNode* n : other.links) {
		links.push_back(n);
	}
	mesh = new Mesh();
	mesh->GenerateSphere(weight, 6, C_GREEN);
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

void Simplex::TravelNode::AddLink(TravelNode * other)
{
	links.push_back(other);
}

bool Simplex::TravelNode::compareLocation(vector3 inLocation)
{
	if (location.x == inLocation.x && location.y == inLocation.y && location.z == inLocation.z)
		return true;
	return false;
}
