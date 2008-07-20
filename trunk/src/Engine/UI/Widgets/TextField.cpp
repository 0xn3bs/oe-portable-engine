#include "TextField.h"

void OE::UI::Widgets::TextField::OnKeyEvent(const int key, const int action)
{
	if(_bHasFocus)
	{
		if(key == 'C' && (glfwGetKey(GLFW_KEY_LCTRL)||glfwGetKey(GLFW_KEY_RCTRL)))
		{
			std::string subCopy = std::string(_szCaption);

			if(_caretPos > _caretUpPos)
				subCopy = subCopy.substr(_caretUpPos, _caretPos-_caretUpPos);

			if(_caretPos < _caretUpPos)
				subCopy = subCopy.substr(_caretPos, _caretUpPos-_caretPos);

			OE::Input::InputManager::CopyToClipboard(subCopy.c_str());
		}

		if(key == 'V' && (glfwGetKey(GLFW_KEY_LCTRL)||glfwGetKey(GLFW_KEY_RCTRL)))
		{
			std::string clipboard = std::string(OE::Input::InputManager::GetFromClipboard());

			if(_caretPos == _caretUpPos)
			{
				_szCaption.insert(_caretPos, clipboard.c_str());
				_caretPos+=clipboard.length();
				_caretUpPos = _caretPos;
			}

			if(_caretPos > _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin() + _caretUpPos, _szCaption.begin() + _caretPos,clipboard.c_str());
				_caretUpPos+=clipboard.length();
				_caretPos = _caretUpPos;
			}

			if(_caretPos < _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin() + _caretPos, _szCaption.begin() + _caretUpPos,clipboard.c_str());
				_caretPos+=clipboard.length();
				_caretUpPos = _caretPos;
			}
		}

		if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			if(_caretPos > 0)
			{
				_caretPos--;
				_caretUpPos = _caretPos;
			}
		}
		if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			if(_caretPos < _szCaption.length())
			{
				_caretPos++;
				_caretUpPos = _caretPos;
			}
		}
		if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			if(*OnSubmit != NULL)
			{
				OnSubmit(_szCaption.c_str());
			}
			_szCaption = "";
			_caretPos = 0;
			_caretUpPos = 0;
		}
		if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		{
			if(_szCaption.length()>0)
			{
				if(_caretPos == _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin() + _caretPos-1,_szCaption.begin() + _caretPos);
					_caretPos--;
					_caretUpPos = _caretPos;
				}

				if(_caretPos > _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin() + _caretUpPos,_szCaption.begin() + _caretPos);
					_caretPos = _caretUpPos;
				}

				if(_caretPos < _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin() + _caretPos,_szCaption.begin() + _caretUpPos);
					_caretUpPos = _caretPos;
				}
			}
		}
		if(key == GLFW_KEY_DEL && action == GLFW_PRESS)
		{
			if(_caretPos == _caretUpPos)
			{
				if(_caretPos < _szCaption.length())
				{
					_szCaption.erase(_szCaption.begin() + _caretPos,_szCaption.begin() + _caretPos+1);
				}
			}
			else
			{
				if(_szCaption.length()>0)
				{
					if(_caretPos == _caretUpPos)
					{
						_szCaption.erase(_szCaption.begin() + _caretPos-1,_szCaption.begin() + _caretPos);
						_caretPos--;
						_caretUpPos = _caretPos;
					}

					if(_caretPos > _caretUpPos)
					{
						_szCaption.erase(_szCaption.begin() + _caretUpPos,_szCaption.begin() + _caretPos);
						_caretPos = _caretUpPos;
					}

					if(_caretPos < _caretUpPos)
					{
						_szCaption.erase(_szCaption.begin() + _caretPos,_szCaption.begin() + _caretUpPos);
						_caretUpPos = _caretPos;
					}
				}
			}
		}
	}
}

void OE::UI::Widgets::TextField::OnCharEvent(const int key, const int action)
{
	if(_bHasFocus && action == GLFW_PRESS)
	{
		if(_caretPos != _caretUpPos)
		{
			if(_caretPos > _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin() + _caretUpPos, _szCaption.begin() + _caretPos,1,key);
				_caretUpPos++;
				_caretPos = _caretUpPos;
			}

			if(_caretPos < _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin() + _caretPos, _szCaption.begin() + _caretUpPos,1,key);
				_caretPos++;
				_caretUpPos = _caretPos;
			}
		}
		else
		{
			_szCaption.insert(_szCaption.begin()+_caretPos, (char)key);
			_caretPos++;
			_caretUpPos = _caretPos;
		}
	}
}
