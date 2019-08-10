#include "AppClass.h"
using namespace Simplex;

void Application::InitVariables(void)
{
	m_sProgrammer = "Nora Murren - bsm4978@g.rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 5.0f, 25.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
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
				n->distToEnd = (float)sqrt(pow(size - x, 2) + pow(size - y, 2) + pow(size - z, 2));
				pathMaze[x][y][z] = n;
			}
		}
	}

	//link neighboring nodes
	for (uint x = 0; x < size; x++) {
		for (uint y = 0; y < size; y++) {
			for (uint z = 0; z < size; z++) {
				pathMaze[x][y][z]->xPos = (x < size - 1) ? pathMaze[x + 1][y][z] : nullptr;
				pathMaze[x][y][z]->xPos = (x > 0) ? pathMaze[x - 1][y][z] : nullptr;

				pathMaze[x][y][z]->yPos = (y < size - 1) ? pathMaze[x][y + 1][z] : nullptr;
				pathMaze[x][y][z]->yPos = (y > 0) ? pathMaze[x][y - 1][z] : nullptr;

				pathMaze[x][y][z]->yPos = (z < size - 1) ? pathMaze[x][y][z + 1] : nullptr;
				pathMaze[x][y][z]->yPos = (z > 0) ? pathMaze[x][y][z - 1] : nullptr;
			}
		}
	}
	

	//-------------------------------------------------------


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