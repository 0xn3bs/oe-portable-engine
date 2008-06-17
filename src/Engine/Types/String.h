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
#ifndef STRING_H_
#define STRING_H_

#include <ostream>
#include <memory>

namespace OE
{
	namespace Types
	{
		class String
		{
		private:
			char* _szData;
			unsigned int _uiLength;
		public:
			String();
			String(const char* s);
			~String();

			friend std::ostream& operator<<(std::ostream& os, const OE::Types::String& rightHand);

			void operator+=(const char* rhs)
			{
				Add(rhs);
			}

			const char* c_str() const;

			void Add(const char* s);
			void Remove(const char* string);
			void Replace(const char* t, const char* n);

			static unsigned int Length(const char* s);
			static bool IsUpper(const char c);
			static bool IsLower(const char c);

			void ToLower();
			void ToUpper();

			int IndexOf(const char c) const;
			int IndexOf(const char c, const unsigned int i) const;
			int IndexOf(const char* string) const;
		};
	};
};

#endif /*STRING_H_*/
