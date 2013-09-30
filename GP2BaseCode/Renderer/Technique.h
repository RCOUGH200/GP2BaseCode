#pragma once

class ITechnique
{
public:
	virtual ~ITechnique(){};

	virtual unsigned int getNumberOfPasses()=0;
	virtual void applyPass(unsigned int passNo)=0;
	virtual unsigned char * getPassSignature(unsigned int passNo)=0;
	virtual unsigned int getPassSignatureSize(unsigned int passNo)=0;

};