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
#include "String.h"
#include <cstdlib>

OE::Types::String::String()
{
	_szData = (char*)malloc(sizeof(char));
	_szData[0] = 0;
	_uiLength = Length(_szData);
}

OE::Types::String::String(const char* s)
{
	int sz = (Length(s)+1)*sizeof(char);
	_szData = (char*)malloc(sz);
	memcpy(_szData, s, sz);
	_szData[sz-1] = 0;
	_uiLength = Length(_szData);
}

OE::Types::String::~String()
{
	free(_szData);
}

std::ostream& operator <<(std::ostream& os, const OE::Types::String& rightHand)
{
	os << rightHand.c_str();
	return os;
}

const char* OE::Types::String::c_str() const
{
	return _szData;
}

void OE::Types::String::Add(const char* s)
{
	int slen = Length(s);
	_szData = (char*)realloc(_szData,(_uiLength+slen+1)*sizeof(char));
	memcpy(_szData+_uiLength,s,(slen+1)*sizeof(char));
	_uiLength = Length(_szData);
}

unsigned int OE::Types::String::Length(const char* s)
{
	int i = 0;
	while(s[i] != 0)
		++i;
	return i;
}

bool OE::Types::String::IsUpper(const char c)
{
	return (c > 64 && c < 91);
}

bool OE::Types::String::IsLower(const char c)
{
	return (c > 96 && c < 123);
}

void OE::Types::String::ToLower()
{
	for(unsigned int i = 0; i < _uiLength; ++i)
		if(IsUpper(_szData[i]))
			_szData[i] += 32;
}

void OE::Types::String::ToUpper()
{
	for(unsigned int i = 0; i < _uiLength; ++i)
		if(IsLower(_szData[i]))
			_szData[i] -= 32;
}

//	TODO: Complete Remove method, don't forget to add all overloaded methods.
void OE::Types::String::Remove(const char* string)
{
	const unsigned int len = Length(string);
	int index = IndexOf(string);
	const unsigned int sz = _uiLength - len;
	char* szTemp = (char*)malloc(sz+1);
	memcpy(szTemp,_szData,index);
	memcpy(szTemp+index,_szData+index+len,_uiLength-index);
	szTemp[sz] = 0;
	free(_szData);
	_szData = szTemp;
}

//	TODO: Complete Replace method, don't forget to add all overloaded methods.
void OE::Types::String::Replace(const char* t, const char* n)
{
	int index = IndexOf(t);

	int oldSz = Length(t);
	int newSz = Length(n);
	int fnlSz = (_uiLength - oldSz) + newSz;

	char* temp = (char*)malloc(fnlSz+1);
	Remove(t);
}

int OE::Types::String::IndexOf(const char c) const
{
	for(unsigned int i = 0; i < _uiLength; ++i)
		if(c == _szData[i])
			return i;
	return -1;
}

int OE::Types::String::IndexOf(const char c, unsigned int i) const
{
	for(; i < _uiLength; i++)
		if(c == _szData[i])
			return i;
	return -1;
}

int OE::Types::String::IndexOf(const char* string) const
{
	const unsigned int len = Length(string);

	if(len == 0)
		return -1;

	int pi = IndexOf(string[0]);

	for(unsigned int i = 0; i < len; ++i)
	{
		if(pi == -1)
			break;
		if(_szData[pi+i]==string[i])
		{
			if(i==len-1)
				return pi;
			continue;
		}
		else
		{
			i = 0; 
			pi = IndexOf(string[0], pi+1);
		}
	}

	return -1;
}
