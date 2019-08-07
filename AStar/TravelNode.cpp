#include "TravelNode.h"
using namespace Simplex;

void Simplex::TravelNode::Release(void)
{
	weight = 0.0f;
	isStart = false;
	isEnd = false;
	links.clear();
}

void Simplex::TravelNode::Swap(TravelNode & other)
{
	std::swap(weight, other.weight);
	std::swap(isStart, other.isStart);
	std::swap(isEnd, other.isEnd);
	std::swap(links, other.links);
}

Simplex::TravelNode::TravelNode(float inWeight, vector3 inLocation)
{
	weight = inWeight;
	location = inLocation;
}

Simplex::TravelNode::TravelNode(TravelNode const & other)
{
	weight = other.weight;
	isStart = other.isStart;
	isEnd = other.isEnd;
	for (TravelNode* n : other.links) {
		links.push_back(n);
	}
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
