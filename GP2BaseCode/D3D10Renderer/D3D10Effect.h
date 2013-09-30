#pragma once

#include "../Renderer/Effect.h"

struct ID3D10Effect;
struct ID3D10Device;

class D3D10Effect:public IEffect
{
public:
	D3D10Effect(ID3D10Device *pDevice);
	~D3D10Effect();

	bool loadFromFile(const wstring& filename);
	bool loadFromMemory(const char * mem);
	ITechnique * getTechnique(const string& passName);

	ID3D10Effect * getD3D10Effect()
	{
		return m_pEffect;
	};
private:
	ID3D10Effect* m_pEffect;
	ID3D10Device* m_pD3D10Device;
};