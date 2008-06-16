#include "FPSCamera.h"

void Odorless::Engine::Cameras::FPSCamera::MoveForward(const float &dt)
{
	float x, y, z;
	x = _dCenterX - _dEyeX;
	y = _dCenterY - _dEyeY;
	z = _dCenterZ - _dEyeZ;

	float magnitude = sqrt((x * x) + (y * y) + (z * z));

	_dEyeX += ((x/magnitude)*CAMERA_SPEED)*dt;
	_dEyeY += ((y/magnitude)*CAMERA_SPEED)*dt;
	_dEyeZ += ((z/magnitude)*CAMERA_SPEED)*dt;
}

void Odorless::Engine::Cameras::FPSCamera::MoveBackward(const float &dt)
{
	float x, y, z;
	x = _dCenterX - _dEyeX;
	y = _dCenterY - _dEyeY;
	z = _dCenterZ - _dEyeZ;

	float magnitude = sqrt((x * x) + (y * y) + (z * z));

	_dEyeX -= ((x/magnitude)*CAMERA_SPEED)*dt;
	_dEyeY -= ((y/magnitude)*CAMERA_SPEED)*dt;
	_dEyeZ -= ((z/magnitude)*CAMERA_SPEED)*dt;
}

void Odorless::Engine::Cameras::FPSCamera::StrafeLeft(const float &dt)
{
	_dCenterX = _dEyeX + sin(yChange+PI*0.5);
	_dCenterZ = _dEyeZ + cos(yChange+PI*0.5);

	float x, y, z;
	x = _dCenterX - _dEyeX;
	z = _dCenterZ - _dEyeZ;

	float magnitude = sqrt((x * x) + (z * z));

	_dEyeX += ((x/magnitude)*CAMERA_SPEED)*dt;
	_dEyeZ += ((z/magnitude)*CAMERA_SPEED)*dt;

	_dCenterX -= _dEyeX + sin(yChange+PI*0.5);
	_dCenterZ -= _dEyeZ + cos(yChange+PI*0.5);
}

void Odorless::Engine::Cameras::FPSCamera::StrafeRight(const float &dt)
{
	_dCenterX = _dEyeX + sin(yChange-PI*0.5);
	_dCenterZ = _dEyeZ + cos(yChange-PI*0.5);

	float x, y, z;
	x = _dCenterX - _dEyeX;
	z = _dCenterZ - _dEyeZ;

	float magnitude = sqrt((x * x) + (z * z));

	_dEyeX += ((x/magnitude)*CAMERA_SPEED)*dt;
	_dEyeZ += ((z/magnitude)*CAMERA_SPEED)*dt;

	_dCenterX -= _dEyeX + sin(yChange-PI*0.5);
	_dCenterZ -= _dEyeZ + cos(yChange-PI*0.5);
}

void Odorless::Engine::Cameras::FPSCamera::Update(const float &dt)
{
	if(Odorless::Engine::Input::InputManager::GetKeyState('W'))
	{
		MoveForward(dt);
	}
	if(Odorless::Engine::Input::InputManager::GetKeyState('S'))
	{
		MoveBackward(dt);
	}
	if(Odorless::Engine::Input::InputManager::GetKeyState('A'))
	{
		StrafeLeft(dt);
	}
	if(Odorless::Engine::Input::InputManager::GetKeyState('D'))
	{
		StrafeRight(dt);
	}
	yChange += (Odorless::Engine::Input::InputManager::GetMouseDeltaX()*0.01f)*MOUSE_SENSITIVITY;
	xChange += (Odorless::Engine::Input::InputManager::GetMouseDeltaY()*0.01f)*MOUSE_SENSITIVITY;

	if(xChange > PI*0.5)
		xChange = PI*0.5;

	if(xChange < -PI*0.5)
		xChange = -PI*0.5;

	_dCenterX = _dEyeX + sin(yChange);
	_dCenterZ = _dEyeZ + cos(yChange);
	_dCenterY = _dEyeY + tan(xChange);
}

void Odorless::Engine::Cameras::FPSCamera::Render()
{	
	gluLookAt(_dEyeX,_dEyeY,_dEyeZ,_dCenterX,_dCenterY,_dCenterZ,_dUpX,_dUpY,_dUpZ);
}