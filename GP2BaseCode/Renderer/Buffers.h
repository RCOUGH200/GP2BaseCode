#pragma once

#include "Vertex.h"

enum BUFFER_TYPE
{
	VERTEX_BUFFER    = 0x1L,
	INDEX_BUFFER     = 0x2L,
	CONSTANT_BUFFER  = 0x4L,
};

enum BUFFER_USAGE
{
  DEFAULT    = 0,
  IMMUTABLE  = 1,
  DYNAMIC    = 2,
  STAGING    = 3
};

enum BUFFER_READ_WRITE_FLAGS
{
	READ                = 1,
	WRITE               = 2,
	READ_WRITE          = 3,
	WRITE_DISCARD       = 4,
	WRITE_NO_OVERWRITE  = 5
};

class IBuffer
{
public:
	virtual ~IBuffer(){};

	virtual bool createBuffer(unsigned int bufferSize, BUFFER_TYPE type,BUFFER_USAGE usage,void **pData)=0;
	virtual bool map(BUFFER_READ_WRITE_FLAGS flags,void **pData)=0;
	virtual bool unmap()=0;
};