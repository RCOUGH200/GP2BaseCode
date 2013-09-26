#pragma once

#include "..\Renderer\Buffers.h"

struct ID3D10Device;

class D3D10Buffer:public IBuffer
{
public:
	D3D10Buffer(ID3D10Device* pD3D10Device);
	~D3D10Buffer();

	bool createBuffer(unsigned int bufferSize, BUFFER_TYPE type,BUFFER_USAGE usage,void **pData);
	bool map(BUFFER_READ_WRITE_FLAGS flags,void **pData);
	bool unmap();
private:
	ID3D10Device *m_pD3D10Device;
	BUFFER_TYPE m_BufferType;
	BUFFER_USAGE m_Usage;
};