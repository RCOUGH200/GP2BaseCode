#pragma once

#include <string>

using namespace std;

class IEffect;


class IRenderer
{
public:
	virtual ~IRenderer(){};
	virtual bool init(void *pWindowHandle,bool fullScreen)=0;
	virtual void clear(float r,float g,float b,float a)=0;
	virtual void present()=0;

	virtual IEffect * loadEffectFromFile(const wstring& name)=0;
	virtual IEffect * loadEffectFromMemory(const char* mem)=0;
};