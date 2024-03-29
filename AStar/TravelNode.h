#ifndef __TRAVELNODECLASS_H_
#define __TRAVELNODECLASS_H_

#include "Simplex/Simplex.h"

namespace Simplex {

class TravelNode {

private:
	void Release(void);
	void Swap(TravelNode& other);

public:
	//to define start and end of path
	bool isStart = false;
	bool isEnd = false;

	float heuristic; //direct distance to destination + weight
	float weight; //weight value to move onto this node

	//3D location
	vector3 location;
	vector3 color = C_GRAY; //color based on start, end, and path

	//mesh representing node
	Mesh* mesh;

	//linked nodes
	TravelNode* yPos = nullptr;
	TravelNode* yNeg = nullptr;
	TravelNode* xPos = nullptr;
	TravelNode* xNeg = nullptr;
	TravelNode* zPos = nullptr;
	TravelNode* zNeg = nullptr;

	//pathing parent
	TravelNode* parent = nullptr;

	TravelNode(float inWeight, vector3 inLocation);
	TravelNode(TravelNode const& other);
	TravelNode& operator=(TravelNode const& other);
	bool operator==(TravelNode const& other);
	~TravelNode(void);

	void changeColor(vector3 newColor);
	void computeHeuristic(uint x, uint y, uint z, uint size);
};

}
#endif //__TRAVELNODECLASS_H_