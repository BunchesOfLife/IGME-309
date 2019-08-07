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

	float distToEnd; //direct distance to destination
	float weight; //weight value to move onto this node

	//3D location
	vector3 location;

	//list of linked nodes
	std::vector<TravelNode*> links;

	TravelNode(float inWeight, vector3 inLocation);
	TravelNode(TravelNode const& other);
	TravelNode& operator=(TravelNode const& other);
	~TravelNode(void);

};

}
#endif //__TRAVELNODECLASS_H_