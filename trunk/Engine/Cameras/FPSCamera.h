/*****************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
*****************************************************************************************/
#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#include <GL/glfw.h>
#include <math.h>
#include "Engine/Input/Input.h"
#include <iostream>

#define PI 3.14159265
#define CAMERA_SPEED 250

namespace Odorless
{
	namespace Engine
	{
		namespace Cameras
		{
			class Camera : public Odorless::Engine::Input::InputListener
			{
			public:
				Camera(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ,
					double upX, double upY, double upZ)
				{
					_dEyeX = eyeX;
					_dEyeY = eyeY;
					_dEyeZ = eyeZ;
					_dCenterX = centerX;
					_dCenterY = centerY;
					_dCenterZ = centerZ;
					_dUpX = upX;
					_dUpY = upY;
					_dUpZ = upZ;
					xAngle = 0;
					yAngle = 0;
					zAngle = 0;
					if(Odorless::Engine::Input::InputManager::IsInitialized()==false)
						std::cerr << "Warning: Camera - The Input Manager has not yet been initialized!" << std::endl;
					else
						Odorless::Engine::Input::InputManager::AddInputListener(this);
				}
				~Camera()
				{
				}

				void OnKeyEvent(const int key, const int action)
				{
				};

				void MoveForward(const float &dt)
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

				void MoveBackward(const float &dt)
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

				void StrafeLeft(const float &dt)
				{
					_dCenterX = _dEyeX + sin(yAngle+0.5*PI);
					_dCenterZ = _dEyeZ + cos(yAngle+0.5*PI);

					float x, y, z;
					x = _dCenterX - _dEyeX;
					z = _dCenterZ - _dEyeZ;

					float magnitude = sqrt((x * x) + (z * z));

					_dEyeX += ((x/magnitude)*CAMERA_SPEED)*dt;
					_dEyeZ += ((z/magnitude)*CAMERA_SPEED)*dt;

					_dCenterX -= _dEyeX + sin(yAngle+0.5*PI);
					_dCenterZ -= _dEyeZ + cos(yAngle+0.5*PI);
				}

				void StrafeRight(const float &dt)
				{
					_dCenterX = _dEyeX + sin(yAngle-0.5*PI);
					_dCenterZ = _dEyeZ + cos(yAngle-0.5*PI);

					float x, y, z;
					x = _dCenterX - _dEyeX;
					z = _dCenterZ - _dEyeZ;

					float magnitude = sqrt((x * x) + (z * z));

					_dEyeX += ((x/magnitude)*CAMERA_SPEED)*dt;
					_dEyeZ += ((z/magnitude)*CAMERA_SPEED)*dt;

					_dCenterX -= _dEyeX + sin(yAngle-0.5*PI);
					_dCenterZ -= _dEyeZ + cos(yAngle-0.5*PI);
				}

				void Update(const float &dt)
				{
					//int mouseDeltaX = Odorless::Engine::Input::InputManager::GetMouseDeltaX;
					//int mouseDeltaY = Odorless::Engine::Input::InputManager::GetMouseDeltaY;
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
					yAngle -= (Odorless::Engine::Input::InputManager::GetMouseDeltaX()*0.01f);
					xAngle -= (Odorless::Engine::Input::InputManager::GetMouseDeltaY()*0.01f);
					_dCenterX = _dEyeX + sin(yAngle);
					_dCenterZ = _dEyeZ + cos(yAngle);
					_dCenterY = _dEyeY + tan(xAngle);
				}

				void Render()
				{
					gluLookAt(_dEyeX,_dEyeY,_dEyeZ,_dCenterX,_dCenterY,_dCenterZ,_dUpX,_dUpY,_dUpZ);
				}

			private:
				double xAngle, yAngle, zAngle;
				double _dEyeX, _dEyeY, _dEyeZ;
				double _dCenterX, _dCenterY, _dCenterZ;
				double _dUpX, _dUpY, _dUpZ;
			};
		}
	}
}
#endif