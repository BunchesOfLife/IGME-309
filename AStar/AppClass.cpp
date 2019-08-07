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

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)
		
	//-----generate maze matrix to path through--------------
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(1.0, 10.0);

	vector3 startLocation = vector3(0.0f, 0.0f, 0.0f);
	for (uint x = 0; x < 20; x++) {
		for (uint y = 0; y < 20; y++) {
			for (uint z = 0; z < 20; z++) {
				TravelNode* n = new TravelNode((float)dis(gen), vector3(x, y, z));
				if (x == 0 && y == 0 && z == 0)
					n->isStart = true;
				if (x == 19 && y == 19 && z == 19)
					n->isEnd = true;
				pathMaze.push_back(n);
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