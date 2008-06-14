#include "VMAP.h"

const int Odorless::Engine::Parsers::VMAP::ParseVMAP(const char *path)
{
	std::vector<std::string> _vLines;

	std::ifstream *fsFile = new std::ifstream();
	fsFile->open(path);

	if(!fsFile)
		return VMAP_LOAD_ERROR;

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

	//	Delete all the empty lines
	for(int i = 0; i < _vLines.size(); i++)
	{
		if(_vLines[i].size() <= 0)
		{
			_vLines.erase(_vLines.begin() + i);
			i--;
			continue;
		}
	}

	std::vector<_BRUSH> vecBrushes;

	return VMAP_LOAD_SUCCESS;
}