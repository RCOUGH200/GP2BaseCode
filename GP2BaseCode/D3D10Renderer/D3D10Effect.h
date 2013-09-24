#pragma once

#include "../Renderer/Effect.h"

struct ID3D10Effect;
struct ID3D10EffectTechnique;
struct ID3D10EffectPass;
struct ID3D10Device;

class D3D10Effect:public IEffect
{
public:
	D3D10Effect();
	~D3D10Effect();

	bool loadFromFile(const wstring& filename,ID3D10Device *pDevice);
	bool loadFromMemory(const char * mem,ID3D10Device *pDevice);
private:
	ID3D10Effect*           m_pEffect;
	ID3D10EffectTechnique*  m_pActiveTechnique;
	ID3D10EffectPass*		m_pCurrentPass;
};