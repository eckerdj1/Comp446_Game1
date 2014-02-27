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

#include <math.h>
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
	void setNewObstacleCluster();
 
private:

	vector<GameObject> fallingBlocks;
	vector<GameObject> bullets;
	Vector3 left, right, forward, back, up, down, zero;

	////// New Stuff added by Steve //////
	Box playerBox;
	Player player;
	int numberOfObstacles;
	vector<Box*> obstacleBoxes;
	vector<Obstacle> obstacles;
	/////New obstacle code: Daniel J. Ecker////
	float floorMovement;
	int clusterSize, clusterSizeVariation, clusterSeparation;
	int cubeSeparation, lineJiggle, cubeJiggle, clusterJiggle;
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

	//New Spectrum HUD stuff by Andy
	Box specHudBox[6];
	Box cursorBox;
	GameObject spectrum[6];
	GameObject cursor;

	float getMultiplier();


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
	
	audio->playCue(MAIN_TRACK);

	srand(time(0));

	left = Vector3(1,0,0);
	right = Vector3(-1,0,0);
	forward = Vector3(0,0,-1);
	back = Vector3(0,0,1);
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	zero = Vector3(0,0,0);

	////// New Stuff added by Steve //////
	numberOfObstacles = 50;
	float obstacleScale = 2.5f;
	float playerScale = 2.67f;
	Vector3 oScale(obstacleScale, obstacleScale, obstacleScale);
	Vector3 pScale(playerScale, playerScale, playerScale);
	playerBox.init(md3dDevice, playerScale, WHITE);
	player.init(&playerBox, sqrt(playerScale * 2.0f), Vector3(0, 1, 0), Vector3(0, 0, 0), 10, pScale, audio);
	player.linkInput(input);

	int posZ = 0;
	int posX = 0;
	int chance = 0;
	int r = 0;
	float floorSpeed = floor.getSpeed();
	for (int i=0; i<numberOfObstacles; ++i)
	{	
		Box* box = new Box();
		box->init(md3dDevice, obstacleScale, GREEN);
		obstacleBoxes.push_back(box);
		Obstacle o;
		o.init(box, sqrt(5.0f), Vector3(0,0,200), Vector3(0,0,-1), 0, Vector3(oScale));
		o.setInActive();
		obstacles.push_back(o);
	}
	///Set obstacle cluster variables
	clusterSize = 1;
	clusterSizeVariation = 3;
	clusterSeparation = 50;
	cubeSeparation = 30;
	lineJiggle = 3;
	cubeJiggle = 3;
	clusterJiggle = 10;
	floorMovement = 0.0f;

	//New spectrum HUD by Andy
	specHudBox[0].init(md3dDevice, .5f, 1.0f, 1.0f, RED, YELLOW);
	specHudBox[1].init(md3dDevice, .5f, 1.0f, 1.0f, YELLOW, GREEN);
	specHudBox[2].init(md3dDevice, .5f, 1.0f, 1.0f, GREEN, CYAN);
	specHudBox[3].init(md3dDevice, .5f, 1.0f, 1.0f, CYAN, BLUE);
	specHudBox[4].init(md3dDevice, .5f, 1.0f, 1.0f, BLUE, MAGENTA);
	specHudBox[5].init(md3dDevice, .5f, 1.0f, 1.0f, MAGENTA, RED);
	cursorBox.init(md3dDevice, .15f, 1.0f, .75f, BLACK, BLACK);

	Vector3 specPos = Vector3(11.0f, 25.0f, -5.0f);
	spectrum[0].init(&specHudBox[0], 1.0f,specPos + Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,0.0f,0.0f), 0, Vector3(0.0f,0.0f,0.0f));
	spectrum[1].init(&specHudBox[1], 1.0f,specPos + Vector3(2.0f,0.0f,0.0f), Vector3(0.0f,0.0f,0.0f), 0, Vector3(0.0f,0.0f,0.0f));
	spectrum[2].init(&specHudBox[2], 1.0f,specPos + Vector3(4.0f,0.0f,0.0f), Vector3(0.0f,0.0f,0.0f), 0, Vector3(0.0f,0.0f,0.0f));
	spectrum[3].init(&specHudBox[3], 1.0f,specPos + Vector3(6.0f,0.0f,0.0f), Vector3(0.0f,0.0f,0.0f), 0, Vector3(0.0f,0.0f,0.0f));
	spectrum[4].init(&specHudBox[4], 1.0f,specPos + Vector3(8.0f,0.0f,0.0f), Vector3(0.0f,0.0f,0.0f), 0, Vector3(0.0f,0.0f,0.0f));
	spectrum[5].init(&specHudBox[5], 1.0f,specPos + Vector3(10.0f,0.0f,0.0f), Vector3(0.0f,0.0f,0.0f), 0, Vector3(0.0f,0.0f,0.0f));
	cursor.init(&cursorBox,1.0f,specPos + Vector3(-.80f, -1.0f, 0.0f), Vector3(0.0f,0.0f,0.0f), 0, Vector3(0.0f,0.0f,0.0f));

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

	float gameTime = mTimer.getGameTime();


	if (!gameOver)
	{
		////// New Stuff added by Steve //////
		player.move(dt);
		player.update(dt);

		//new clustered cube code
		floorMovement += floor.getSpeed() * dt;
		if (floorMovement > clusterSeparation)
		{
			floorMovement = 0.0f;
			setNewObstacleCluster();
		}

		for (int i = 0; i < numberOfObstacles; i++) {
			obstacles[i].setSpeed(floor.getSpeed());
			float zPos = obstacles[i].getPosition().z;
			if (zPos > 100 && zPos < 130)
				for (int f=0; f<floor.size(); ++f)
				{
					if (floor.section(f).contains(Vector3(0, -2, zPos)))
					{
						DXColor compliment = floor.section(f).colorAtPoint(zPos);
						compliment.r = 1.0f - compliment.r;
						compliment.g = 1.0f - compliment.g;
						compliment.b = 1.0f - compliment.b;
						obstacles[i].setColor(compliment);
						break;
					}
				}
			obstacles[i].update(dt);
			if (player.isWithin(12.0f, &obstacles[i]))
			{
				if (player.collided(&obstacles[i]))
				{
					gameOver = true;
					audio->playCue(GAME_OVER);
				}
			}
		}

		for(int i = 0; i < 6; i++) {
			spectrum[i].update(dt);
		}

		float cursorPos = player.getWheelVal();
		cursor.setPosition(Vector3(10.2f,24.0f,-5.0f) + 2*Vector3(cursorPos, 0.0f, 0.0f));
		cursor.update(dt);

		score.setMultiplier(getMultiplier());

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
	}


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

	//draw the floor
	foo[0] = 0;
	mfxFLIPVar->SetRawValue(&foo[0], 0 , sizeof(int));
	floor.draw(mView, mProj, mfxWVPVar, mTech);

	////// New Stuff added by Steve //////
	mWVP = player.getWorldMatrix()  *mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	player.setMTech(mTech);
	player.draw();

	for (int i = 0; i < numberOfObstacles; i++) {
		mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
		mWVP = obstacles[i].getWorldMatrix() * mView * mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		obstacles[i].setMTech(mTech);
		obstacles[i].draw();
	}

	//Spectrum HUD
	for(int i = 0; i < 6; i++) {
		mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
		D3DXMATRIX a;
		D3DXMatrixRotationY(&a, 1.573f);
		mWVP = a * spectrum[i].getWorldMatrix() * mView * mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		spectrum[i].setMTech(mTech);
		spectrum[i].draw();
	}
	mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	mWVP = cursor.getWorldMatrix() * mView * mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	cursor.setMTech(mTech);
	cursor.draw();
	
	//////////////////////////////////////

	/////Text Drawing Section
	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	RECT R1 = {0, 0, 800, 600};
	RECT R2 = {0, 540, 800, 600};

	std::wostringstream outs;  
	
	outs.precision(6);
	string Hud = score.getString();

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
	if (gameTime < 3.0f)
	{
		mFont->DrawText(0, L"Move your Box LEFT and RIGHT with A & D to avoid hitting the obstacles", -1, &R2, DT_CENTER | DT_VCENTER, BLACK);
	}
	else if (gameTime < 6.0f)
	{
		mFont->DrawText(0, L"Change the color of your Box by pressing the J and L keys.", -1, &R2, DT_CENTER | DT_VCENTER, BLACK);
	}
	else if (gameTime < 9.0f)
	{
		mFont->DrawText(0, L"The closer the color of your cube is to the floor, the higher the score multiplier!", -1, &R2, DT_CENTER | DT_VCENTER, BLACK);
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
 
void ColoredCubeApp::setNewObstacleCluster()
{
	float obstacleScale = 2.5f;
	float startZ = 125.0f;
	float currentZ = startZ;
	float laneSize = 6.0f;
	Vector3 oScale(obstacleScale, obstacleScale, obstacleScale);
	bool lane[5];
	for (int i=0; i<5; ++i)
	{
		lane[i] = false;
	}

	
	int cs = clusterSize + rand() % clusterSizeVariation;
	while(cs)	//put lines of cubes in the same cluster
	{	
		int cubesOnLine = rand() % 4 + 1;
		while (cubesOnLine) //puts cubes on the same line
		{
			float thisZ = currentZ + (rand() % lineJiggle);
			int pickLane = rand() % 5;
			while (lane[pickLane])
			{
				pickLane = rand() % 5;
			}
			float thisX = -12.0f + pickLane * laneSize;
			lane[pickLane] = true;
			cubesOnLine--;
			for (int i=0; i<obstacles.size(); ++i)
			{
				if (obstacles[i].isNotActive())
				{
					obstacles[i].setActive();
					obstacles[i].setPosition(Vector3(thisX, 1, thisZ));
					obstacles[i].setSpeed(floor.getSpeed());
					break;
				}
			}
			
		}
		for (int i=0; i<5; ++i)
		{
			lane[i] = false;
		}
		currentZ += (int)(cubeSeparation + rand() % cubeJiggle);
		cs--;
	}
}

float ColoredCubeApp::getMultiplier() {
	DXColor floorC = RED;
	DXColor playerC = player.getColor();
	
	for(int i = 0; i < floor.size(); i++) {
		if(floor.getTile(i)->contains(Vector3(0.0f,-2.0f,0.0f))) {
			floorC = floor.getTile(i)->colorAtPoint(0.0f);
		}
	}

	float dr,dg,db;
	double dt;
	dr = abs(floorC.r - playerC.r);
	dg = abs(floorC.g - playerC.g);
	db = abs(floorC.b - playerC.b);
	
	dt = dr + dg + db;
	dt *= 10;
	int t = (int)dt;
	dt = (double)t/10;

	float mult = 4 - dt;
	return mult;
}
