#pragma once

#include "../Renderer/Technique.h"

struct ID3D10EffectTechnique;

class D3D10Technique:public ITechnique
{
public:
	D3D10Technique(ID3D10EffectTechnique * pTechnique);
	~D3D10Technique();

	unsigned int getNumberOfPasses();
	void applyPass(unsigned int passNo);
	unsigned char * getPassSignature(unsigned int passNo);
	unsigned int getPassSignatureSize(unsigned int passNo);
private:
	ID3D10EffectTechnique * m_pTechnique;
};