#include "FPSCamera.h"

void OEngine::Cameras::FPSCamera::MoveForward(const float &dt)
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

void OEngine::Cameras::FPSCamera::MoveBackward(const float &dt)
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

void OEngine::Cameras::FPSCamera::StrafeLeft(const float &dt)
{
	_dCenterX = _dEyeX + sin(yChange+PI2);
	_dCenterZ = _dEyeZ + cos(yChange+PI2);

	float x, z;
	x = _dCenterX - _dEyeX;
	z = _dCenterZ - _dEyeZ;

	float magnitude = sqrt((x * x) + (z * z));

	_dEyeX += ((x/magnitude)*CAMERA_SPEED)*dt;
	_dEyeZ += ((z/magnitude)*CAMERA_SPEED)*dt;

	_dCenterX -= _dEyeX + sin(yChange+PI2);
	_dCenterZ -= _dEyeZ + cos(yChange+PI2);
}

void OEngine::Cameras::FPSCamera::StrafeRight(const float &dt)
{
	_dCenterX = _dEyeX + sin(yChange-PI2);
	_dCenterZ = _dEyeZ + cos(yChange-PI2);

	float x, z;
	x = _dCenterX - _dEyeX;
	z = _dCenterZ - _dEyeZ;

	float magnitude = sqrt((x * x) + (z * z));

	_dEyeX += ((x/magnitude)*CAMERA_SPEED)*dt;
	_dEyeZ += ((z/magnitude)*CAMERA_SPEED)*dt;

	_dCenterX -= _dEyeX + sin(yChange-PI2);
	_dCenterZ -= _dEyeZ + cos(yChange-PI2);
}

void OEngine::Cameras::FPSCamera::Update(const float &dt)
{
	if(OEngine::Input::InputManager::GetKeyState('W'))
	{
		MoveForward(dt);
	}
	if(OEngine::Input::InputManager::GetKeyState('S'))
	{
		MoveBackward(dt);
	}
	if(OEngine::Input::InputManager::GetKeyState('A'))
	{
		StrafeLeft(dt);
	}
	if(OEngine::Input::InputManager::GetKeyState('D'))
	{
		StrafeRight(dt);
	}

	yChange += (OEngine::Input::InputManager::GetMouseDeltaX()*0.01f)*MOUSE_SENSITIVITY;
	xChange += (OEngine::Input::InputManager::GetMouseDeltaY()*0.01f)*MOUSE_SENSITIVITY;

	if(xChange > PI2)
		xChange = PI2;

	if(xChange < -PI2)
		xChange = -PI2;

	_dCenterX = _dEyeX + sin(yChange);
	_dCenterZ = _dEyeZ + cos(yChange);
	_dCenterY = _dEyeY + tan(xChange);
}

void OEngine::Cameras::FPSCamera::Render()
{	
	gluLookAt(_dEyeX,_dEyeY,_dEyeZ,_dCenterX,_dCenterY,_dCenterZ,_dUpX,_dUpY,_dUpZ);
}
