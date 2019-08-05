#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;

void Simplex::MyOctant::Init(void)
{
	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint i = 0; i < 8; i++) {
		m_pChild[i] = nullptr;
	}
}

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	//initialize variables
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	//start root here
	m_pRoot = this;
	m_lChild.clear();
	std::vector<vector3> lMinMax;

	//calculate the center of the collection of entities in the scene to make the root octant
	uint nObjects = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < nObjects; i++) {
		MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* pRigidBody = pEntity->GetRigidBody();
		lMinMax.push_back(pRigidBody->GetMinGlobal());
		lMinMax.push_back(pRigidBody->GetMaxGlobal());
	}
	MyRigidBody* pRigidBody = new MyRigidBody(lMinMax);

	vector3 vHalfWidth = pRigidBody->GetHalfWidth();
	float fMax = vHalfWidth.x;
	for (int i = 1; i < 3; i++) {
		if (fMax < vHalfWidth[i])
			fMax = vHalfWidth[i];
	}
	vector3 v3Center = pRigidBody->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(pRigidBody);

	//get size of max borders
	m_fSize = fMax * 2.0f;
	m_v3Center = v3Center;
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	m_uOctantCount++;
	
	ConstructTree(m_uMaxLevel);
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	//makes a branch/leaf octant
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_uChildren = other.m_uChildren;
	m_v3Max = other.m_v3Max;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_pParent = other.m_pParent;
	m_pRoot = other.m_pRoot;
	m_lChild = other.m_lChild;
	m_uLevel = other.m_uLevel;
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	for (uint i = 0; i < 8; i++) {
		m_pChild[i] = other.m_pChild[i];
	}
}

MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}

Simplex::MyOctant::~MyOctant(void)
{
}

void Simplex::MyOctant::Release(void)
{
	if (m_uLevel == 0)
		KillBranches();

	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++) {
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	uint entityCount = m_pEntityMngr->GetEntityCount();
	if(a_uRBIndex >= entityCount)
		return false;
	
	MyEntity* entity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* body = entity->GetRigidBody();
	vector3 min = body->GetMinGlobal();
	vector3 max = body->GetMaxGlobal();

	//AABB x/y/z checks
	if (m_v3Max.x < min.x)
		return false;
	if (m_v3Min.x > max.x)
		return false;
	if (m_v3Max.y < min.y)
		return false;
	if (m_v3Min.y > max.y)
		return false;
	if (m_v3Max.z < min.z)
		return false;
	if (m_v3Min.z > max.z)
		return false;

	return true;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//draw octant of specific index
	if (m_uID == a_nIndex) {
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_nIndex);
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	//recursively display children
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_v3Color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//recursively add wire cubes to draw to the render list based on the tree
	uint numLeaves = m_lChild.size();
	for (uint i = 0; i < numLeaves; i++) {
		m_lChild[i]->DisplayLeafs(a_v3Color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::ClearEntityList(void)
{
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->ClearEntityList();
	}
	m_EntityList.clear();
}

void Simplex::MyOctant::Subdivide(void)
{
	//dont divide if node at max level
	if (m_uLevel >= m_uMaxLevel) return;
	//dont divide if node already divided
	if (m_uChildren != 0) return;

	m_uChildren = 8;

	//resize for new children
	float size = m_fSize / 4.0f;
	float diameter = size * 2.0f;
	vector3 center = m_v3Center;
	center.x -= size;
	center.y -= size;
	center.z -= size;

	//back left bottom
	m_pChild[0] = new MyOctant(center, diameter);

	//back left top
	center.y += diameter;
	m_pChild[1] = new MyOctant(center, diameter);

	//back right top
	center.x += diameter;
	m_pChild[2] = new MyOctant(center, diameter);

	//back right bottom
	center.y -= diameter;
	m_pChild[3] = new MyOctant(center, diameter);

	//front right bottom
	center.z += diameter;
	m_pChild[4] = new MyOctant(center, diameter);

	//front right top
	center.y += diameter;
	m_pChild[5] = new MyOctant(center, diameter);

	//front left top
	center.x -= diameter;
	m_pChild[6] = new MyOctant(center, diameter);

	//front left bottom
	center.y -= diameter;
	m_pChild[7] = new MyOctant(center, diameter);

	//link parents and children
	for (uint i = 0; i < 8; i++) {
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		//continue to divide if not reached ideal count within octant
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
			m_pChild[i]->Subdivide();
	}
}

MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	if (a_nChild > 7) return nullptr;
	return m_lChild[a_nChild];
}

MyOctant * Simplex::MyOctant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return m_uChildren == 0;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	//count number of entities colliding with the octant
	uint count = 0;
	uint entityCount = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < entityCount; i++) {
		if (IsColliding(i))
			count++;
		if (count > a_nEntities)
			return true;
	}
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	//recursively loop through the tree cleaning off the ends
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	m_uChildren = 0;
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//only make tree off root
	if (m_uLevel != 0) return;

	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	//clear out any current tree
	ClearEntityList();
	KillBranches();
	m_lChild.clear();

	if (ContainsMoreThan(m_uIdealEntityCount))
		Subdivide();

	AssignIDtoEntity();
	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	//recurse until finding leaf
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->AssignIDtoEntity();
	}
	if (IsLeaf()) {
		uint entityCount = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < entityCount; i++) {
			if (IsColliding(i)) {
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}

void Simplex::MyOctant::ConstructList(void)
{
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->ConstructList();
	}
	if (m_EntityList.size() > 0)
		m_pRoot->m_lChild.push_back(this);
}
