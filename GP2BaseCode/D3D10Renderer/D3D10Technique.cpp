#include "D3D10Technique.h"

D3D10Technique::D3D10Technique(ID3D10EffectTechnique * pTechnique)
{
	m_pTechnique=pTechnique;
}

D3D10Technique::~D3D10Technique()
{
}

unsigned int D3D10Technique::getNumberOfPasses()
{
	return 0;
}

void D3D10Technique::applyPass(unsigned int passNo)
{
}

unsigned char * D3D10Technique::getPassSignature(unsigned int passNo)
{
	return 0;
}

unsigned int D3D10Technique::getPassSignatureSize(unsigned int passNo)
{
	return 0;
}
