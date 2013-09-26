#include "D3D10Buffer.h"
#include <D3D10.h>

D3D10Buffer::D3D10Buffer(ID3D10Device* pD3D10Device)
{
	m_pD3D10Device=pD3D10Device;
}

D3D10Buffer::~D3D10Buffer()
{
}

bool D3D10Buffer::createBuffer(unsigned int bufferSize, BUFFER_TYPE type,BUFFER_USAGE usage,void **pData)
{
	return true;
}

bool D3D10Buffer::map(BUFFER_READ_WRITE_FLAGS flags,void **pData)
{
	return true;
}

bool D3D10Buffer::unmap()
{
	return true;
}