//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================


#include "d3dApp.h"
#include "Box.h"
#include "GameObject.h"
#include "Line.h"
#include "Quad.h"
#include <d3dx9math.h>
#include "LineObject.h"
#include "Score.h"
#include "Player.h"
#include "Obstacle.h"
#include "Floor.h"

#include <ctime>
#include <vector>
#include <string>
using std::string;
using std::vector;
using std::time;
using std::srand;
using std::rand;


#define toString(x) Text::toString(x)

class ColoredCubeApp : public D3DApp
{
public:
	ColoredCubeApp(HINSTANCE hInstance);
	~ColoredCubeApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

private:
	void buildFX();
	void buildVertexLayouts();
 
private:

	Quad quad1;
	Line line;
	Box whiteBox, redBox, blueBox, greenBox, crimBox, dRedBox, dBlueBox;
	GameObject gameObject1, gameObject2, gameObject3, spinner;
	vector<GameObject> fallingBlocks;
	vector<GameObject> bullets;
	LineObject xLine, yLine, zLine;
	Vector3 left, right, forward, back, up, down, zero;


	////// New Stuff added by Steve //////
	Box playerBox;
	Player player;
	int numberOfObstacles;
	Box obstacleBox;
	vector<Obstacle> obstacles;
	Line rLine, gLine, bLine;
	//////////////////////////////////////
	///Floor
	Floor floor;

	float fallRatePerSecond;
	float avgFallSpeed;
	float elapsed;
	float bulletElapsed;
	float bulletsPerSecond;

	float floorSectionLength;
	

	int playerBlock;
	int lives;
	int ammo;
	bool lifeGained;
	bool activeMessage;
	std::wstring message;
	float messageTimer;

	Score score;

	bool gameOver;


	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	//my addition
	ID3D10EffectVariable* mfxFLIPVar;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	//my edits
	D3DXMATRIX worldBox1, worldBox2;

	float mTheta;
	float mPhi;


};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//#endif


	ColoredCubeApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

ColoredCubeApp::ColoredCubeApp(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mfxWVPVar(0), mTheta(0.0f), mPhi(PI*0.25f)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 
}

ColoredCubeApp::~ColoredCubeApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void ColoredCubeApp::initApp()
{
	D3DApp::initApp();

	srand(time(0));

	left = Vector3(1,0,0);
	right = Vector3(-1,0,0);
	forward = Vector3(0,0,-1);
	back = Vector3(0,0,1);
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	zero = Vector3(0,0,0);
	
	whiteBox.init(md3dDevice, 1.0f, WHITE);
	redBox.init(md3dDevice, 1.0f, RED);
	blueBox.init(md3dDevice, 1.0f, BLUE);
	greenBox.init(md3dDevice, 1.0f, GREEN);
	crimBox.init(md3dDevice, 0.8f, CRIMSON);
	dRedBox.init(md3dDevice, 0.8f, DARKRED);
	dBlueBox.init(md3dDevice, 0.25f, DARKBLUE);

	line.init(md3dDevice, 10.0f, GREEN);

	////// New Stuff added by Steve //////
	gLine.init(md3dDevice, 10.0f, GREEN);
	rLine.init(md3dDevice, 10.0f, RED);
	bLine.init(md3dDevice, 10.0f, BLUE);
	xLine.init(&rLine, Vector3(0,0,0), 10);
	xLine.setPosition(Vector3(0,0,0));
	yLine.init(&gLine, Vector3(0,0,0), 10);
	yLine.setPosition(Vector3(0,0,0));
	yLine.setRotationZ(ToRadian(90));
	zLine.init(&bLine, Vector3(0,0,0), 10);
	zLine.setPosition(Vector3(0,0,0));
	zLine.setRotationY(ToRadian(90));
	numberOfObstacles = 40;
	float obstacleScale = 2.5f;
	playerBox.init(md3dDevice, obstacleScale, WHITE);
	player.init(&playerBox, sqrt(obstacleScale * 2.0f), Vector3(0, 0, 0), Vector3(0, 0, 0), 10, 0);
	player.linkInput(input);

	int posZ = 0;
	int posX = 0;
	int chance = 0;
	int r = 0;
	float floorSpeed = floor.getSpeed();
	obstacleBox.init(md3dDevice, obstacleScale, GREEN);
	for (int i = 0; i < numberOfObstacles; i++) {
		Obstacle newObstacle;
		if ((i%5) == 0) {
			posZ = rand()%150;
			posZ += 200;
			r = rand()%5;
			switch(r) {
				case 0:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 1:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 2:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(0, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 3:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 4:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
			}
		}
		if ((i%5) == 1) {
			int posZ = rand()%150;
			posZ += 400;
			r = rand()%5;
			switch(r) {
				case 0:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 1:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 2:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(0, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 3:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 4:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
			}
		}
		if ((i%5) == 2) {
			int posZ = rand()%150;
			posZ += 600;
			r = rand()%5;
			switch(r) {
					case 0:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 1:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 2:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(0, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 3:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 4:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
			}
		}
		if ((i%5) == 3) {
			int posZ = rand()%150;
			posZ += 700;
			r = rand()%5;
			switch(r) {
					case 0:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 1:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 2:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(0, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 3:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 4:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
			}
		}
		if ((i%5) == 4) {
			int posZ = rand()%150;
			posZ += 800;
			r = rand()%5;
			switch(r) {
					case 0:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 1:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(-6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 2:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(0, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 3:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(6, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
				case 4:
					newObstacle.init(&obstacleBox, sqrt(obstacleScale * 2.0f), Vector3(12, 0, posZ), Vector3(0, 0, -1), floorSpeed, obstacleScale);
					break;
			}
		}

		obstacles.push_back(newObstacle);
	}
	//////////////////////////////////////


	gameObject1.init(&whiteBox, sqrt(2.0f), Vector3(-10,0,0), Vector3(0,0,0), 0,1);
	gameObject2.init(&redBox, sqrt(2.0f), Vector3(4,0,0), Vector3(0,0,0), 0,1);
	gameObject3.init(&redBox, sqrt(2.0f), Vector3(-4,0,0), Vector3(0,0,0), 0,1);

	floor.init(md3dDevice);

	gameOver = false;
	activeMessage = false;
	messageTimer = 0.0f;

	buildFX();
	buildVertexLayouts();

}

void ColoredCubeApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void ColoredCubeApp::updateScene(float dt)
{
	D3DApp::updateScene(dt);
	
	/*Old Code that may still be useful//
	//float gameTime = mTimer.getGameTime();
	//if (input->wasKeyPressed(PlayerRightKey) || input->wasKeyPressed(VK_RIGHT))
	//{
	//	playerBlock++;
	//}
	//if (input->wasKeyPressed(PlayerLeftKey) || input->wasKeyPressed(VK_LEFT))
	//{
	//	playerBlock--;
	//}
	//if (playerBlock < 10)
	//	playerBlock = 10;
	//if (playerBlock > 30)
	//	playerBlock = 30;

	//if (gameTime > 7.7f && !gameOver) //five seconds?
	//{
	//	bulletElapsed += dt;
	//	if (input->getMouseLButton() && bulletElapsed > (1.0f / bulletsPerSecond))
	//	{
	//		for (int i=0; i<bullets.size(); ++i)
	//		{
	//			if (bullets[i].isNotActive())
	//			{
	//				bullets[i].setActive();
	//				ammo--;
	//				bullets[i].setPosition(Vector3(-40 + 2 * playerBlock, -14, 0));
	//				bulletElapsed = 0.0f;
	//				break;
	//			}
	//		}
	//	}
	//	for (int i=0; i<bullets.size(); ++i)
	//	{
	//		bullets[i].update(dt);
	//		if (bullets[i].isActive())
	//		{	
	//			if (bullets[i].getPosition().y > 20)
	//			{
	//				bullets[i].setInActive();
	//				ammo++;
	//			}
	//		}
	//	}


	//	//falling blocks
	//	if (gameTime > 6 && (int)gameTime % 5 == 0)
	//		fallRatePerSecond += 0.0000777;
	//	elapsed += dt; //start counting elapsed time

	//	if (elapsed > (1.0f / fallRatePerSecond))
	//	{
	//		elapsed = 0.0f;
	//		for (int i=0; i<fallingBlocks.size(); ++i)
	//		{
	//			if (fallingBlocks[i].isNotActive())
	//			{
	//				fallingBlocks[i].setActive();
	//				float s = avgFallSpeed + (rand() % 4 - 1);
	//				fallingBlocks[i].setSpeed(s);
	//				int xPos = rand() % 11 - 5;
	//				fallingBlocks[i].setPosition(Vector3(xPos * 2,20,0));
	//				break;
	//			}
	//		}
	//	}
	//	for (int i=0; i<fallingBlocks.size(); ++i)
	//	{
	//		fallingBlocks[i].update(dt);
	//		if (fallingBlocks[i].isActive())
	//		{
	//			if (fallingBlocks[i].getBoxBottom() < -14.0f) // block hit the floor
	//			{
	//				fallingBlocks[i].setInActive();
	//				lives--;
	//				activeMessage = true;
	//				message = L"STEAMED! Gallon lost.";
	//			}
	//		}
	//	}
	//	//end of falling blocks

	//	//bullet/block collision
	//	for (int i=0; i<fallingBlocks.size(); ++i)
	//	{
	//		if (fallingBlocks[i].isActive())
	//		{
	//			for (int j=0; j<bullets.size(); ++j)
	//			{
	//				if (bullets[j].isActive())
	//				{
	//					if (bullets[j].collided(&fallingBlocks[i]))
	//					{
	//						bullets[j].setInActive();
	//						ammo++;
	//						fallingBlocks[i].setInActive();
	//						score.addPoints(1);
	//						lifeGained = false;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	if (!lifeGained && score.getInt() % 100 == 0)
	//	{
	//		lives++;
	//		lifeGained = true;
	//		activeMessage = true;
	//		message = L"Milestone! Extra gallon generated!";
	//	}
	//	if (activeMessage)
	//	{
	//		messageTimer += dt;
	//		if (messageTimer > 4.0f)
	//		{
	//			messageTimer = 0.0f;
	//			activeMessage = false;
	//		}
	//	}
	//	if (lives == 0)
	//	{
	//		gameOver = true;
	//	}
	//	if (lives == 1)
	//	{
	//		activeMessage = true;
	//		message = L"DANGER!\nOne gallon left!";
	//	}
	//}*/

	
	
	////// New Stuff added by Steve //////
	player.move();
	player.update(dt);

	for (int i = 0; i < 40; i++) {
		obstacles[i].update(dt);
	}

	xLine.update(dt);
	yLine.update(dt);
	zLine.update(dt);
	//////////////////////////////////////
	// Floor test code //



	/*for (int i=0; i<floor.size(); ++i)
	{
		floor[i].update(dt);
		float zPos = floor[i].getPosition().z;
		if (zPos < -50)
			floor[i].setPosition(Vector3(0, -2, zPos + floor.size() * floorSectionLength));
	}*/
	//Changes By: Daniel J. Ecker
	floor.update(dt);

	

	// Build the view matrix.
	D3DXVECTOR3 pos(0.0f,45.0f,-50.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
	input->clearAll();
}

void ColoredCubeApp::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);

	// set some variables for the shader
	int foo[1];
	foo[0] = 0;
	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	//setting the color flip variable in the shader
	mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));

	//draw the lines
	

	//draw the quad using the "old" method
	

	////draw the boxes
	//mWVP = gameObject1.getWorldMatrix()  *mView*mProj;
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//gameObject1.setMTech(mTech);
	//gameObject1.draw();

	//mWVP = gameObject2.getWorldMatrix()*mView*mProj;
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//foo[0] = 0;
	//mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//gameObject2.setMTech(mTech);
	//gameObject2.draw();
	//mWVP = gameObject3.getWorldMatrix()*mView*mProj;
	//foo[0] = 0;
	//mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//gameObject3.setMTech(mTech);
	//gameObject3.draw();
     
	////draw the spinning box
	//if (ToRadian(spinAmount*40) > PI)
	//	foo[0] = 1;
	//else
	//	foo[0] = 0;
	//mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//Matrix spin;
	//RotateY(&spin, ToRadian(spinAmount*40));
	//Matrix translate;
	//Translate(&translate, 5, 0, 0);
	//mWVP = spinner.getWorldMatrix() *translate * spin  *mView*mProj;
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//spinner.setMTech(mTech);
	//spinner.draw();

	////draw the wall of boxes
	//foo[0] = 0;
	//mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//for (int i=0; i<fallingBlocks.size(); ++i)
	//{
	//	mWVP = fallingBlocks[i].getWorldMatrix()*mView*mProj;
	//	mfxWVPVar->SetMatrix((float*)&mWVP);
	//	fallingBlocks[i].setMTech(mTech);
	//	fallingBlocks[i].draw();
	//}
	////draw the bullets
	//foo[0] = 0;
	//for (int i=0; i<bullets.size(); ++i)
	//{
	//	mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//	mWVP = bullets[i].getWorldMatrix() * mView * mProj;
	//	mfxWVPVar->SetMatrix((float*)&mWVP);
	//	bullets[i].setMTech(mTech);
	//	bullets[i].draw();
	//}

	////draw the falling blocks
	//foo[0] = 0;
	//for (int i=0; i<fallingBlocks.size(); ++i)
	//{
	//	mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//	mWVP = fallingBlocks[i].getWorldMatrix() * mView * mProj;
	//	mfxWVPVar->SetMatrix((float*)&mWVP);
	//	fallingBlocks[i].setMTech(mTech);
	//	fallingBlocks[i].draw();
	//}

	//draw the floor
	foo[0] = 0;
	mfxFLIPVar->SetRawValue(&foo[0], 0 , sizeof(int));
	floor.draw(mView, mProj, mfxWVPVar, mTech);

	////// New Stuff added by Steve //////
	mWVP = player.getWorldMatrix()  *mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	player.setMTech(mTech);
	player.draw();

	for (int i = 0; i < 40; i++) {
		mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
		mWVP = obstacles[i].getWorldMatrix() * mView * mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		obstacles[i].setMTech(mTech);
		obstacles[i].draw();
	}


	mWVP = zLine.getWorldMatrix() *mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	zLine.setMTech(mTech);
	zLine.draw();

	mWVP = yLine.getWorldMatrix() *mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	yLine.setMTech(mTech);
	yLine.draw();

	mWVP = xLine.getWorldMatrix()*mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	xLine.setMTech(mTech);
	xLine.draw();

	
	//////////////////////////////////////

	/////Text Drawing Section
	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	RECT R1 = {0, 0, 800, 600};
	RECT R2 = {0, 540, 800, 600};

	std::wostringstream outs;  
	
	outs.precision(6);
	string Hud = score.getString() + "\nBlobs Available: " + toString(ammo) + "\nGallons Left: " + toString(lives);

	/*outs << score.getString() << L"\n";
	outs << L"Blobs Available: " << ammo << L"\n";
	outs << L"Gallons Left: " << lives;
	std::wstring text = outs.str();
	mFont->DrawText(0, text.c_str(), -1, &R, DT_NOCLIP, BLACK);*/
	timesNew.draw(Hud, Vector2(5, 5));
	if (gameOver)
	{
		mFont->DrawText(0, L"Game Over!", -1, &R1, DT_CENTER | DT_VCENTER, BLACK);
	}
	float gameTime = mTimer.getGameTime();
	if (gameTime < 4.5f)
	{
		mFont->DrawText(0, L"Move your Water Cannon LEFT and RIGHT with A & D or with arrows.", -1, &R2, DT_CENTER | DT_VCENTER, BLACK);
	}
	else if (gameTime < 9.0f)
	{
		mFont->DrawText(0, L"Shoot enhanced water blobs at FALLING LAVA with the LEFT mouse button.", -1, &R2, DT_CENTER | DT_VCENTER, BLACK);
	}
	else if (gameTime < 14.0f)
	{
		mFont->DrawText(0, L"Water blobs are limited.  Use them wisely.\nDon't let all your water evaporate!", -1, &R2, DT_CENTER | DT_VCENTER, BLACK);
	}
	if (activeMessage)
	{
		mFont->DrawText(0, message.c_str(), -1, &R2, DT_CENTER | DT_VCENTER, BLACK);
	}
	

	mSwapChain->Present(0, 0);
}

void ColoredCubeApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("ColorTech");
	
	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxFLIPVar = mFX->GetVariableByName("flip");

}

void ColoredCubeApp::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}
 