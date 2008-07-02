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
#ifndef INI_H_
#define INI_H_

#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace OE
{
	namespace Parsers
	{
		class INI
		{
		public:
			static const int INI_LOAD_ERROR = -1;
			static const int INI_LOAD_SUCCESS = 0;

			INI()
			{
			}
			~INI()
			{
			}

			const int ParseINI(const char* path);

			std::string GetString(const std::string &section, const std::string &variable)
			{
				return _mapINIData[section][variable];
			}

			int GetInt(const std::string &section, const std::string &variable);
			bool GetBool(const std::string &section, const std::string &variable);
			double GetDouble(const std::string &section, const std::string &variable);
			float GetFloat(const std::string &section, const std::string &variable);

		private:
			std::map< std::string, std::map< std::string,std::string> > _mapINIData;
			unsigned int _uiLength;
		};
	}
}

#endif /*INI_H_*/
