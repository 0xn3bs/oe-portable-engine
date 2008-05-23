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

#include "string.h"
#include <cstdlib>
#include <iostream>

Odorless::Engine::Types::String::String()
{
	this->_szData = (char*)malloc(sizeof(char));
	this->_szData[0] = 0;
	this->_uiLength = Length(this->_szData);
}

Odorless::Engine::Types::String::String(const char* s)
{
	int sz = (Length(s)+1)*sizeof(char);
	this->_szData = (char*)malloc(sz);
	memcpy(this->_szData, s, sz);
	this->_szData[sz-1] = 0;
	this->_uiLength = Length(this->_szData);
}

Odorless::Engine::Types::String::~String()
{

}

std::ostream& Odorless::Engine::Types::operator <<(std::ostream& os, const Odorless::Engine::Types::String& rightHand)
{
	os << rightHand.c_str();
	return os;
}

char* Odorless::Engine::Types::String::c_str() const
{
	int size = (_uiLength+1)*sizeof(char);
	char* temp = (char*)malloc(size);
	memcpy(temp, this->_szData, size);
	temp[size-1] = 0;
	return temp;
}

void Odorless::Engine::Types::String::Add(const char* s)
{
	int slen = Length(s);
	_szData = (char*)realloc(_szData,(this->_uiLength+slen+1)*sizeof(char));
	memcpy(_szData+this->_uiLength,s,(slen+1)*sizeof(char));
	this->_uiLength = Length(_szData);
}

//	Does not count the null terminator
unsigned int Odorless::Engine::Types::String::Length(const char* s)
{
	int i = 0;
	while(s[i] != 0)
		i++;
	return i;
}

bool Odorless::Engine::Types::String::IsUpper(const char& c)
{
	return (c > 64 && c < 91);
}

bool Odorless::Engine::Types::String::IsLower(const char& c)
{
	return (c > 96 && c < 123);
}

void Odorless::Engine::Types::String::ToLower()
{
	for(unsigned int i = 0; i < this->_uiLength; i++)
		if(IsUpper(this->_szData[i]))
			this->_szData[i] += 32;
}

void Odorless::Engine::Types::String::ToUpper()
{
	for(unsigned int i = 0; i < this->_uiLength; i++)
		if(IsLower(this->_szData[i]))
			this->_szData[i] -= 32;
}

Odorless::Engine::Types::String Odorless::Engine::Types::String::Remove(const char* string) const
{
	const unsigned int len = Length(string);
	int index = IndexOf(string);
	const unsigned int sz = this->_uiLength - len;
	char* temp = (char*)malloc(sz+1);
	memcpy(temp,this->_szData,index);
	memcpy(temp+index,this->_szData+index+len,this->_uiLength-index);
	temp[sz] = 0;
	Odorless::Engine::Types::String* t = new Odorless::Engine::Types::String(temp);
	free(temp);
	return *t;
}

Odorless::Engine::Types::String Odorless::Engine::Types::String::Replace(const char* t, const char* n) const
{
	int index = IndexOf(t);

	int oldSz = Length(t);
	int newSz = Length(n);
	int fnlSz = (this->_uiLength - oldSz) + newSz;

	char* temp = (char*)malloc(fnlSz+1);
	Odorless::Engine::Types::String rmd = this->Remove(t);


	//	FINISH THIS.

	return rmd;
}

int Odorless::Engine::Types::String::IndexOf(const char c) const
{
	for(unsigned int i = 0; i < this->_uiLength; i++)
		if(c == this->_szData[i])
			return i;
	return -1;
}

int Odorless::Engine::Types::String::IndexOf(const char c, unsigned int i) const
{
	for(; i < this->_uiLength; i++)
		if(c == this->_szData[i])
			return i;
	return -1;
}

int Odorless::Engine::Types::String::IndexOf(const char* string) const
{
	const unsigned int len = Length(string);

	if(len == 0)
		return -1;

	int pi = IndexOf(string[0]);

	for(unsigned int i = 0; i < len; i++)
	{
		if(pi == -1)
			break;
		if(this->_szData[pi+i]==string[i])
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
