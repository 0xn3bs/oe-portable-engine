#include "INI.h"

int Odorless::Engine::Parsers::INI::GetInt(std::string section, std::string variable)
{
	std::string value = _mapINIData[section][variable];
	const char* tempBuffer = value.c_str();
	int result = atoi(tempBuffer);
	return result;
}

bool Odorless::Engine::Parsers::INI::GetBool(std::string section, std::string variable)
{
	std::string value = _mapINIData[section][variable];

	if(value=="true")
		return true;
	return false;
}

double Odorless::Engine::Parsers::INI::GetDouble(std::string section, std::string variable)
{
	std::string value = _mapINIData[section][variable];
	const char* tempBuffer = value.c_str();
	double result = atof(tempBuffer);
	return result;
}

float Odorless::Engine::Parsers::INI::GetFloat(std::string section, std::string variable)
{
	std::string value = _mapINIData[section][variable];
	const char* tempBuffer = value.c_str();
	float result = (float)atof(tempBuffer);
	return result;
}

const int Odorless::Engine::Parsers::INI::ParseINI(const char* path)
{
	std::vector<std::string> _vLines;

	std::ifstream *fsFile = new std::ifstream();
	fsFile->open(path);

	if(!fsFile)
		return INI_LOAD_ERROR;

	//	Calculate the length of the file.
	fsFile->seekg(0, std::ios::end);
	_uiLength = fsFile->tellg();
	fsFile->seekg(0, std::ios::beg);

	while(fsFile->tellg() < _uiLength)
	{
		std::string line;
		std::getline(*fsFile,line);
		_vLines.push_back(line);
	}

	fsFile->close();

	delete fsFile;

	//	Remove leading and trailing whitespace.

	for(unsigned int i = 0; i < _vLines.size(); i++)
	{
		//	Erase empty lines from the vector.
		if(_vLines[i].size()<=0)
		{
			_vLines.erase(_vLines.begin()+i);
			i--;
			continue;
		}
		std::string::iterator itCharacter = _vLines[i].begin();
		while(*(itCharacter)==' ')
		{
			_vLines[i].erase(itCharacter);
			itCharacter=_vLines[i].begin();
		}
		itCharacter = _vLines[i].end()-1;
		while(*(itCharacter)==' ')
		{
			_vLines[i].erase(itCharacter);
			itCharacter = _vLines[i].end()-1;
		}
	}

	//	Remove invalid lines
	for(unsigned int i = 0; i < _vLines.size(); i++)
	{
		unsigned int equalIndex = _vLines[i].find_first_of("=");
		//	There's nothing important in this line so let's remove it.
		if(equalIndex==std::string::npos&&_vLines[i][0]!='[')
		{
			_vLines.erase(_vLines.begin()+i);
			i--;
			continue;
		}
	}

	//	Build a map of quotes
	std::map< int, std::vector<int> > vecQuoteIndices;
	for(unsigned int i = 0; i < _vLines.size(); i++)
	{
		for(unsigned int j = 0; j < _vLines[i].size(); j++)
		{
			if(_vLines[i][j]=='\"'&&_vLines[i][j-1]!='\\')
				vecQuoteIndices[i].push_back(j);
		}
	}

	//	Strip Comments and clean it up...
	for(unsigned int i = 0; i < _vLines.size(); i++)
	{
		unsigned int equalIndex = _vLines[i].find_first_of("=");
		unsigned int commentIndex = _vLines[i].find_first_of(';');
		unsigned int LBraceIndex = _vLines[i].find_first_of('[');
		unsigned int RBraceIndex = _vLines[i].find_first_of(']');

		//	This line is invalid, remove it.
		if(equalIndex == std::string::npos)
		{
			if(LBraceIndex == std::string::npos || RBraceIndex == std::string::npos)
			{
				_vLines.erase(_vLines.begin()+i);
				i--;
				continue;
			}

			if(commentIndex > LBraceIndex && commentIndex > RBraceIndex)
			{
				_vLines[i] = _vLines[i].substr(0, commentIndex);
				//	Strip whitespace
				_vLines[i] = _vLines[i].substr(0, _vLines[i].find_last_not_of(' ') + 1);
			}
			else 
				if(commentIndex != std::string::npos)
				{
					_vLines.erase(_vLines.begin()+i);
					i--;
					continue;
				}
		}
		else
		{
			if(commentIndex!=std::string::npos)
			{
				if(commentIndex<equalIndex)
				{
					_vLines.erase(_vLines.begin()+i);
					i--;
					continue;
				}
				else
				{
					while(commentIndex != std::string::npos)
					{
						if(vecQuoteIndices[i].size() > 0)
						{
							//	Is the delimiter between quotes?
							if(vecQuoteIndices[i][0] < commentIndex && vecQuoteIndices[i][1] > commentIndex)
							{
								if(commentIndex + 1 < _vLines[i].size())
									commentIndex = _vLines[i].find_first_of(';', commentIndex+1);
								else
									commentIndex = std::string::npos;
							}
							else
							{
								_vLines[i] = _vLines[i].substr(0, commentIndex);
								_vLines[i] = _vLines[i].substr(0, _vLines[i].find_last_not_of(' ') + 1);
								commentIndex = std::string::npos;
							}
						}
						else
						{
							_vLines[i] = _vLines[i].substr(0, commentIndex);
							_vLines[i] = _vLines[i].substr(0, _vLines[i].find_last_not_of(' ') + 1);
							commentIndex = std::string::npos;
						}
					}
				}
			}
		}
	}

	//	Parse
	std::string section="null";
	for(unsigned int i = 0; i < _vLines.size(); i++)
	{
		if(_vLines[i][0]=='[')
		{
			unsigned int endBrace = _vLines[i].find_last_of("]");
			if(endBrace!=std::string::npos)
			{
				if(endBrace>1)
				{
					section = _vLines[i].substr(1,endBrace-1);
				}
			}
		}
		for(unsigned int j = 0; j < _vLines[i].size(); j++)
		{
			if(_vLines[i][j]=='=')
			{
				if(j+1<_vLines[i].size())
				{
					std::string left = _vLines[i].substr(0,j);
					int il = left.find_first_not_of(' ');
					int ir = left.find_last_not_of(' ');
					left = left.substr(il, ir+1);
					//	Don't include the delimiter on the right side
					std::string right = _vLines[i].substr(j+1,_vLines[i].size()-j-1);
					il = right.find_last_of(' ');
					ir = right.find_last_not_of(' ');
					right = right.substr(il+1, ir);
					_mapINIData[section].insert(make_pair(left,right));
				}
			}
		}
	}
	return INI_LOAD_SUCCESS;
}
