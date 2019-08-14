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
	xPos = other.xPos;
	xNeg = other.xNeg;
	yPos = other.yPos;
	yNeg = other.yNeg;
	zPos = other.zPos;
	zNeg = other.zNeg;
	parent = other.parent;
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

bool Simplex::TravelNode::operator==(TravelNode const & other)
{
	if(location == other.location
		&& isEnd == other.isEnd
		&& isStart == other.isStart
		&& mesh == other.mesh
		&& color == other.color
		&& xPos == other.xPos
		&& xNeg == other.xNeg
		&& yPos == other.yPos
		&& yNeg == other.yNeg
		&& zPos == other.zPos
		&& zNeg == other.zNeg
		&& parent == other.parent)
		return true;
	return false;
}

Simplex::TravelNode::~TravelNode(void)
{
}

void Simplex::TravelNode::changeColor(vector3 newColor)
{
	color = newColor;
	mesh->GenerateSphere(weight, 6, color);
}

void Simplex::TravelNode::computeHeuristic(uint x, uint y, uint z, uint size)
{
	heuristic = (float)sqrt(pow(size - x, 2) + pow(size - y, 2) + pow(size - z, 2)) + weight;
}
