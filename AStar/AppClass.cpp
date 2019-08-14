#include "AppClass.h"
using namespace Simplex;

void Application::InitVariables(void)
{
	m_sProgrammer = "Nora Murren - bsm4978@g.rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(-5.0f, 5.0f, -10.0f), //Position
		vector3(5.0f, 5.0f, 5.0f),	//Target
		AXIS_Y);					//Up

	//m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	//-----generate maze matrix to path through--------------
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.1, 1.0); //weights will be between 0.1-1

	//generate a list of nodes with random weights in a 10x10x10 cube
	vector3 startLocation = vector3(0.0f, 0.0f, 0.0f);
	
	//create 3D array of nodes
	for (uint x = 0; x < size; x++) {
		for (uint y = 0; y < size; y++) {
			for (uint z = 0; z < size; z++) {
				TravelNode* n = new TravelNode((float)dis(gen), vector3(x * 2, y * 2, z * 2));
				if (x == 0 && y == 0 && z == 0) {
					n->isStart = true;
					n->changeColor(C_GREEN);
				}
				if (x == size - 1 && y == size - 1 && z == size - 1) {
					n->isEnd = true;
					n->changeColor(C_RED);
				}
				n->computeHeuristic(x, y, z, size);
				pathMaze[x][y][z] = n;
			}
		}
	}

	//link neighboring nodes
	for (uint x = 0; x < size; x++) {
		for (uint y = 0; y < size; y++) {
			for (uint z = 0; z < size; z++) {
				pathMaze[x][y][z]->xPos = (x < size - 1) ? pathMaze[x + 1][y][z] : nullptr;
				pathMaze[x][y][z]->xNeg = (x > 0) ? pathMaze[x - 1][y][z] : nullptr;

				pathMaze[x][y][z]->yPos = (y < size - 1) ? pathMaze[x][y + 1][z] : nullptr;
				pathMaze[x][y][z]->yNeg = (y > 0) ? pathMaze[x][y - 1][z] : nullptr;

				pathMaze[x][y][z]->yPos = (z < size - 1) ? pathMaze[x][y][z + 1] : nullptr;
				pathMaze[x][y][z]->yNeg = (z > 0) ? pathMaze[x][y][z - 1] : nullptr;
			}
		}
	}
	
	//-------------------------------------------------------

	//initialize the traveller
	traveller = new Mesh();
	traveller->GenerateWireCube(1.0f, C_BLUE);

	std::vector<TravelNode*> OpenList;
	std::vector<TravelNode*> ClosedList;

	OpenList.push_back(pathMaze[0][0][0]);

	bool isPathing = true;
	while (isPathing) {
		//find next node based on heuristic
		TravelNode* current = nullptr;
		float distCompare = 2000.0f;
		for (TravelNode* n : OpenList) {
			if (n->heuristic < distCompare) {
				current = n;
			}
		}
		//move to closed list
		ClosedList.push_back(current);
		OpenList.erase(std::remove(OpenList.begin(), OpenList.end(), current));

		//finish when end node added to closed list
		if (current->isEnd) {
			isPathing = false;
			//SafeDelete(current);
			break;
		}

		if (current->xPos != nullptr || std::find(ClosedList.begin(), ClosedList.end(), current->xPos) != ClosedList.end()) { //if not impassable or on closed list
			if (std::find(OpenList.begin(), OpenList.end(), current->xPos) == OpenList.end()) { //if not on open list, add it
				current->xPos->parent = current;
				OpenList.push_back(current);
			}
			else {
				if (current->xPos->heuristic < current->heuristic) //if on openlist, compare heuristic and change parent if better
					current->xPos->parent = current;
			}
		}
		if (current->xNeg != nullptr || std::find(ClosedList.begin(), ClosedList.end(), current->xNeg) != ClosedList.end()) { //if not impassable or on closed list
			if (std::find(OpenList.begin(), OpenList.end(), current->xNeg) == OpenList.end()) { //if not on open list, add it
				current->xNeg->parent = current;
				OpenList.push_back(current);
			}
			else {
				if (current->xNeg->heuristic < current->heuristic) //if on openlist, compare heuristic and change parent if better
					current->xNeg->parent = current;
			}
		}
		if (current->yPos != nullptr || std::find(ClosedList.begin(), ClosedList.end(), current->yPos) != ClosedList.end()) { //if not impassable or on closed list
			if (std::find(OpenList.begin(), OpenList.end(), current->yPos) == OpenList.end()) { //if not on open list, add it
				current->yPos->parent = current;
				OpenList.push_back(current);
			}
			else {
				if (current->yPos->heuristic < current->heuristic) //if on openlist, compare heuristic and change parent if better
					current->yPos->parent = current;
			}
		}
		if (current->yNeg != nullptr || std::find(ClosedList.begin(), ClosedList.end(), current->yNeg) != ClosedList.end()) { //if not impassable or on closed list
			if (std::find(OpenList.begin(), OpenList.end(), current->yNeg) == OpenList.end()) { //if not on open list, add it
				current->yNeg->parent = current;
				OpenList.push_back(current);
			}
			else {
				if (current->yNeg->heuristic < current->heuristic) //if on openlist, compare heuristic and change parent if better
					current->yNeg->parent = current;
			}
		}
		if (current->zPos != nullptr || std::find(ClosedList.begin(), ClosedList.end(), current->zPos) != ClosedList.end()) { //if not impassable or on closed list
			if (std::find(OpenList.begin(), OpenList.end(), current->zPos) == OpenList.end()) { //if not on open list, add it
				current->zPos->parent = current;
				OpenList.push_back(current);
			}
			else {
				if (current->zPos->heuristic < current->heuristic) //if on openlist, compare heuristic and change parent if better
					current->zPos->parent = current;
			}
		}
		if (current->zNeg != nullptr || std::find(ClosedList.begin(), ClosedList.end(), current->zNeg) != ClosedList.end()) { //if not impassable or on closed list
			if (std::find(OpenList.begin(), OpenList.end(), current->zNeg) == OpenList.end()) { //if not on open list, add it
				current->zNeg->parent = current;
				OpenList.push_back(current);
			}
			else {
				if (current->zNeg->heuristic < current->heuristic) //if on openlist, compare heuristic and change parent if better
					current->zNeg->parent = current;
			}
		}
		//SafeDelete(current);
	}

	//loop through closed list and make lerp path
	for (TravelNode* n : ClosedList) {
		pathStops.push_back(n->location);
		n->changeColor(C_BLUE);
	}

	ClosedList.clear();
	OpenList.clear();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	//m_pEntityMngr->Update();

	//Add objects to render list
	//m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	for (uint x = 0; x < size; x++) {
		for (uint y = 0; y < size; y++) {
			for (uint z = 0; z < size; z++) {
				matrix4 loc = glm::translate(pathMaze[x][y][z]->location);
				pathMaze[x][y][z]->mesh->Render(m4Projection, m4View, loc);
			}
		}
	}

	//---------traveller lerp code----------------------------

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;

	static uint stopsCount = pathStops.size();
	float animTime = 1.0f;
	static uint route = 0;
	vector3 start;
	vector3 end;
	if (route < stopsCount - 1) {
		start = pathStops[route];
		end = pathStops[route + 1];
	}
	else {
		start = pathStops.back();
		end = pathStops.front();
	}

	float percentage = MapValue(fTimer, 0.0f, animTime, 0.0f, 1.0f);

	v3CurrentPos = glm::lerp(start, end, percentage);

	if (percentage >= 1.0f) {
		route++;
		fTimer = m_pSystem->GetDeltaTime(uClock);
		route %= stopsCount;
	}

	traveller->Render(m4Projection, m4View, glm::translate(v3CurrentPos));

	//---------------------------------------------------------
		
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();
	
	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}

void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}