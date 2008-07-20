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

			_szRenderedText = _szCaption;
		}

		if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			if(_caretPos > 0)
			{
				_caretPos--;
				_caretUpPos = _caretPos;
			}
			else
			{
				if(_iLeftBound>0)
					_iLeftBound--;
			}

			_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
		}
		if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			if(_caretPos < _szCaption.length() && _caretPos < (_v2fDimensions.x/16))
			{
				_caretPos++;
				_caretUpPos = _caretPos;
			}
			else
			{
				if(_iLeftBound + _caretPos< _szCaption.length())
				{
					_iLeftBound++;
				}
			}

			_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
		}
		if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			if(*OnSubmit != NULL)
			{
				OnSubmit(_szCaption.c_str());
			}
			_szCaption = "";
			_szRenderedText = _szCaption;
			_caretPos = 0;
			_caretUpPos = 0;
		}
		if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		{
			if(_szCaption.length()>0)
			{
				if(_caretPos == _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin() + _caretPos+_iLeftBound-1,_szCaption.begin() + _caretPos+_iLeftBound);
					_caretPos--;
					_caretUpPos = _caretPos;
				}

				if(_caretPos > _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin() + _caretUpPos+_iLeftBound,_szCaption.begin() + _caretPos+_iLeftBound);
					_caretPos = _caretUpPos;
				}

				if(_caretPos < _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin() + _caretPos+_iLeftBound,_szCaption.begin() + _caretUpPos+_iLeftBound);
					_caretUpPos = _caretPos;
				}

				_szRenderedText = _szCaption;
			}
		}
		if(key == GLFW_KEY_DEL && action == GLFW_PRESS)
		{
			if(_caretPos == _caretUpPos)
			{
				if(_caretPos < _szCaption.length())
				{
					_szCaption.erase(_szCaption.begin() + _caretPos+_iLeftBound,_szCaption.begin() + _caretPos+_iLeftBound+1);
				}
			}
			else
			{
				if(_szCaption.length()>0)
				{
					if(_caretPos == _caretUpPos)
					{
						_szCaption.erase(_szCaption.begin() + _caretPos+_iLeftBound-1,_szCaption.begin() + _caretPos+_iLeftBound);
						_caretPos--;
						_caretUpPos = _caretPos;
					}

					if(_caretPos > _caretUpPos)
					{
						_szCaption.erase(_szCaption.begin() + _caretUpPos+_iLeftBound,_szCaption.begin() + _caretPos+_iLeftBound);
						_caretPos = _caretUpPos;
					}

					if(_caretPos < _caretUpPos)
					{
						_szCaption.erase(_szCaption.begin() + _caretPos+_iLeftBound,_szCaption.begin() + _caretUpPos+_iLeftBound);
						_caretUpPos = _caretPos;
					}
				}
			}
			
			_szRenderedText = _szCaption;
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
				if(_caretUpPos<_szCaption.length()-_iLeftBound)
					_caretUpPos++;
				_caretPos = _caretUpPos;
			}

			if(_caretPos < _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin() + _caretPos, _szCaption.begin() + _caretUpPos,1,key);
				if(_caretPos<_szCaption.length()-_iLeftBound)
					_caretPos++;
				_caretUpPos = _caretPos;
			}
		}
		else
		{
			if(_caretPos + _iLeftBound < (_v2fDimensions.x/16))
			{
				_szCaption.insert(_szCaption.begin()+_caretPos+_iLeftBound, (char)key);
				if(_caretPos<_szCaption.length()-_iLeftBound)
					_caretPos++;
			}
			else
			{
				if(_szCaption.length() > (_v2fDimensions.x/16))
				{
					_iLeftBound++;
				}
				_szCaption.insert(_szCaption.begin()+_caretPos+_iLeftBound-1, (char)key);
			}

			_caretUpPos = _caretPos;
		}

		_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
	}
}
