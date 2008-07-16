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

#include "Engine/GLFW/glfw.h"
#include "Engine/Maths/Vector.h"
#include "Engine/Input/Input.h"
#include <iostream>

#define CAMERA_SPEED 250
#define MOUSE_SENSITIVITY 0.2

namespace OE
{
	namespace Cameras
	{
		class FPSCamera : public OE::Input::InputListener
		{
		public:
			FPSCamera(OE::Maths::Vec3<double> eye, OE::Maths::Vec3<double> center, OE::Maths::Vec3<double> up)
			{
				_v3dEye = eye;
				_v3dCenter = center;
				_v3dUp = up;
			}

			FPSCamera(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ,
				double upX, double upY, double upZ)
			{
				_v3dEye.x = eyeX;
				_v3dEye.y = eyeY;
				_v3dEye.z = eyeZ;
				_v3dCenter.x = centerX;
				_v3dCenter.y = centerY;
				_v3dCenter.z = centerZ;
				_v3dUp.x = upX;
				_v3dUp.y = upY;
				_v3dUp.z = upZ;

				if(OE::Input::InputManager::IsInitialized()==false)
					std::cerr << "Warning: Camera - The Input Manager has not yet been initialized!" << std::endl;
				else
					OE::Input::InputManager::AddInputListener(this);
			}

			~FPSCamera()
			{
			}

			void OnKeyEvent(const int key, const int action)
			{
				if(key==GLFW_KEY_TAB && action == GLFW_PRESS)
				{
					OE::Input::InputManager::SetCursorVisible(!OE::Input::InputManager::IsCursorVisible());
					OE::Input::InputManager::SetForceToMouseReference(!OE::Input::InputManager::IsMouseForcedToReference());
				}
			};

			void MoveForward(const float dt);
			void MoveBackward(const float dt);
			void StrafeLeft(const float dt);
			void StrafeRight(const float dt);

			void Update(const float dt);
			void Render();

			OE::Maths::Vec3<double> GetPos()
			{
				return _v3dEye;
			}

			OE::Maths::Vec3<double> _v3dEye;
		private:
			
			OE::Maths::Vec3<double> _v3dCenter;
			OE::Maths::Vec3<double> _v3dUp;
			OE::Maths::Vec3<double> _v3dChange;
		};
	}
}

#endif
