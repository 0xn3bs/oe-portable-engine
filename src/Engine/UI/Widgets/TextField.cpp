#include "TextField.h"

void OE::UI::Widgets::TextField::OnKeyEvent(const int key, const int action)
{
	if(_bHasFocus)
	{
		if(key == 'C' && (glfwGetKey(GLFW_KEY_LCTRL)||glfwGetKey(GLFW_KEY_RCTRL)) && action == GLFW_PRESS)
		{
			std::string subCopy = std::string(_szCaption);

			if(_caretPos > _caretUpPos)
				subCopy = subCopy.substr(_caretUpPos+_iLeftBound, _caretPos-_caretUpPos);

			if(_caretPos < _caretUpPos)
				subCopy = subCopy.substr(_caretPos+_iLeftBound, _caretUpPos-_caretPos);

			OE::Input::InputManager::CopyToClipboard(subCopy.c_str());
		}

		if(key == 'V' && (glfwGetKey(GLFW_KEY_LCTRL)||glfwGetKey(GLFW_KEY_RCTRL)) && action == GLFW_PRESS)
		{
			std::string clipboard = std::string(OE::Input::InputManager::GetFromClipboard());

			if(_caretPos == _caretUpPos)
			{
				_szCaption.insert(_caretPos+_iLeftBound, clipboard.c_str());
				_caretPos+=clipboard.length();
				if(_caretPos>=(_v2fDimensions.x/16))
				{
					_iLeftBound += _caretPos - (_v2fDimensions.x/16);
					_caretPos -= _caretPos - (_v2fDimensions.x/16);
				}
				_caretUpPos = _caretPos;
			}

			if(_caretPos > _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin() + _caretUpPos + _iLeftBound, _szCaption.begin() + _caretPos + _iLeftBound,clipboard.c_str());
				_caretUpPos+=clipboard.length();
				if(_caretUpPos>=(_v2fDimensions.x/16))
				{
					_iLeftBound += _caretUpPos - (_v2fDimensions.x/16);
					_caretUpPos -= _caretUpPos - (_v2fDimensions.x/16);
				}
				_caretPos = _caretUpPos;
			}

			if(_caretPos < _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin() + _caretPos + _iLeftBound, _szCaption.begin() + _caretUpPos + _iLeftBound,clipboard.c_str());
				_caretPos+=clipboard.length();
				if(_caretPos>=(_v2fDimensions.x/16))
				{
					_iLeftBound += _caretPos - (_v2fDimensions.x/16);
					_caretPos -= _caretPos - (_v2fDimensions.x/16);
				}
				_caretUpPos = _caretPos;
			}

			_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
		}

		if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			_bCaretRender = true;
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
			_bCaretRender = true;
			if(_caretPos < _szCaption.length() && _caretPos < (_v2fDimensions.x/16))
			{
				_caretPos++;
				_caretUpPos = _caretPos;
			}
			else
			{
				if(_iLeftBound + _caretPos < _szCaption.length())
				{
					_iLeftBound++;
				}
			}

			_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
		}

		if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			_bCaretRender = true;
			if(*OnSubmit != NULL)
			{
				OnSubmit(_szCaption.c_str());
			}
			_szCaption = "";
			_szRenderedText = _szCaption;
			_caretPos = 0;
			_caretUpPos = 0;
			_iLeftBound = 0;
		}

		if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		{
			_bCaretRender = true;
			if(_szCaption.length()>0)
			{
				if(_caretPos == _caretUpPos)
				{
					if(_caretPos != 0 || _iLeftBound != 0)
						_szCaption.erase(_caretPos+_iLeftBound-1,1);

					if(_caretPos == (_v2fDimensions.x/16) && _iLeftBound > 0)
						_iLeftBound--;
					else
					if((_caretPos+_iLeftBound)<_szCaption.length() && _iLeftBound > 0)
						_iLeftBound--;
					else
					if(_caretPos > 0)
					{
						_caretPos--;
						_caretUpPos = _caretPos;
					}
				}

				if(_caretPos > _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin()+_caretUpPos+_iLeftBound,_szCaption.begin()+_caretPos+_iLeftBound);
					_caretPos = _caretUpPos;
				}

				if(_caretPos < _caretUpPos)
				{
					_szCaption.erase(_szCaption.begin()+_caretPos+_iLeftBound,_szCaption.begin()+_caretUpPos+_iLeftBound);
					_caretUpPos = _caretPos;
				}

				_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
			}
		}

		if(key == GLFW_KEY_DEL && action == GLFW_PRESS)
		{
			_bCaretRender = true;
			if(_caretPos == _caretUpPos)
			{
				if(_caretPos < (_v2fDimensions.x/16))
				{
					if(_caretPos + _iLeftBound < _szCaption.length())
					{
						_szCaption.erase(_szCaption.begin() + _caretPos+_iLeftBound,_szCaption.begin() + _caretPos+_iLeftBound+1);
						_caretUpPos = _caretPos;
					}
				}
				
			}
			else
			{
				if(_szCaption.length()>0)
				{
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
			
			_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
		}
	}
}

void OE::UI::Widgets::TextField::OnCharEvent(const int key, const int action)
{
	if(_bHasFocus && action == GLFW_PRESS)
	{
		_bCaretRender = true;
		if(_caretPos != _caretUpPos)
		{
			if(_caretPos > _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin()+_caretUpPos+_iLeftBound, _szCaption.begin()+_caretPos+_iLeftBound,1,key);
				if(_caretUpPos<_szCaption.length()-_iLeftBound)
					_caretUpPos++;
				_caretPos = _caretUpPos;
			}

			if(_caretPos < _caretUpPos)
			{
				_szCaption.replace(_szCaption.begin()+_caretPos+_iLeftBound, _szCaption.begin()+_caretUpPos+_iLeftBound,1,key);
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
				
				if(_caretPos!=(_v2fDimensions.x/16))
				{
					_caretPos++;
					_caretUpPos=_caretPos;
				}
			}
			else
			{
				_szCaption.insert(_szCaption.begin()+_caretPos+_iLeftBound, (char)key);
				
				if(_caretPos != (_v2fDimensions.x/16))
				{
					_caretPos++;
					_caretUpPos=_caretPos;
				}
				else
				if(_caretPos == (_v2fDimensions.x/16))
				{
					_iLeftBound++;
				}
			}
		}

		_szRenderedText = _szCaption.substr(_iLeftBound,_szCaption.length() - _iLeftBound);
	}
}
