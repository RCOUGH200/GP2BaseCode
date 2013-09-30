#pragma once

#include <string>

using namespace std;

class ITechnique;

class IEffect
{
public:
	virtual ~IEffect(){};

	virtual bool loadFromFile(const wstring& filename)=0;
	virtual bool loadFromMemory(const char * mem)=0;
	virtual ITechnique * getTechnique(const string& passName)=0;
};