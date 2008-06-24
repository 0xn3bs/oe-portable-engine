/***************************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
***************************************************************************************************/
#include "FPSCamera.h"
#include "Engine/Textures/TextureManager.h"

void OE::Cameras::FPSCamera::MoveForward(const float &dt)
{
	OE::Maths::Vec3<double> direction;

	direction = _v3dCenter - _v3dEye;

	float magnitude = direction.Magnitude();

	_v3dEye += ((direction/magnitude)*CAMERA_SPEED)*dt;
}

void OE::Cameras::FPSCamera::MoveBackward(const float &dt)
{
	OE::Maths::Vec3<double> direction;

	direction = _v3dCenter - _v3dEye;

	float magnitude = direction.Magnitude();

	_v3dEye -= ((direction/magnitude)*CAMERA_SPEED)*dt;
}

void OE::Cameras::FPSCamera::StrafeLeft(const float &dt)
{
	_v3dCenter.x = _v3dEye.x + sin(_v3dChange.y+PI2);
	_v3dCenter.z = _v3dEye.z + cos(_v3dChange.y+PI2);

	OE::Maths::Vec3<double> direction;
	direction = _v3dCenter - _v3dEye;
	direction.y = 0;	//	Our y direction should not affect our strafe speed when pitched.

	float magnitude = direction.Magnitude();

	_v3dEye.x += ((direction.x/magnitude)*CAMERA_SPEED)*dt;
	_v3dEye.z += ((direction.z/magnitude)*CAMERA_SPEED)*dt;

	_v3dCenter.x -= _v3dEye.x + sin(_v3dChange.y+PI2);
	_v3dCenter.z -= _v3dEye.z + cos(_v3dChange.y+PI2);
}

void OE::Cameras::FPSCamera::StrafeRight(const float &dt)
{
	_v3dCenter.x = _v3dEye.x + sin(_v3dChange.y-PI2);
	_v3dCenter.z = _v3dEye.z + cos(_v3dChange.y-PI2);

	OE::Maths::Vec3<double> direction;
	direction = _v3dCenter - _v3dEye;
	direction.y = 0;	//	Our y direction should not affect our strafe speed when pitched.

	float magnitude = direction.Magnitude();

	_v3dEye.x += ((direction.x/magnitude)*CAMERA_SPEED)*dt;
	_v3dEye.z += ((direction.z/magnitude)*CAMERA_SPEED)*dt;

	_v3dCenter.x -= _v3dEye.x + sin(_v3dChange.y-PI2);
	_v3dCenter.z -= _v3dEye.z + cos(_v3dChange.y-PI2);
}

void OE::Cameras::FPSCamera::Update(const float &dt)
{
	if(OE::Input::InputManager::GetKeyState('W'))
	{
		MoveForward(dt);
	}
	if(OE::Input::InputManager::GetKeyState('S'))
	{
		MoveBackward(dt);
	}
	if(OE::Input::InputManager::GetKeyState('A'))
	{
		StrafeLeft(dt);
	}
	if(OE::Input::InputManager::GetKeyState('D'))
	{
		StrafeRight(dt);
	}

	_v3dChange.y += (OE::Input::InputManager::GetMouseDeltaX()*0.01f)*MOUSE_SENSITIVITY;
	_v3dChange.x += (OE::Input::InputManager::GetMouseDeltaY()*0.01f)*MOUSE_SENSITIVITY;

	if(_v3dChange.x > PI2)
		_v3dChange.x = PI2;

	if(_v3dChange.x < -PI2)
		_v3dChange.x = -PI2;

	_v3dCenter.x = _v3dEye.x + sin(_v3dChange.y);
	_v3dCenter.z = _v3dEye.z + cos(_v3dChange.y);
	_v3dCenter.y = _v3dEye.y + tan(_v3dChange.x);
}

void OE::Cameras::FPSCamera::Render()
{	
	gluLookAt(_v3dEye.x,_v3dEye.y,_v3dEye.z,_v3dCenter.x,_v3dCenter.y,_v3dCenter.z,_v3dUp.x,_v3dUp.y,_v3dUp.z);
}
