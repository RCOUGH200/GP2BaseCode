#include "D3D10Effect.h"
#include "BasicEffect.h"
#include <D3D10.h>
#include <D3DX10.h>

D3D10Effect::D3D10Effect()
{
	m_pEffect=NULL;
	m_pActiveTechnique=NULL;
	m_pCurrentPass=NULL;
}

D3D10Effect::~D3D10Effect()
{
	//effect
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect=NULL;
	}
}

bool D3D10Effect::loadFromFile(const wstring& filename,ID3D10Device *pDevice)
{
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program. - BMD
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob * pErrorBuffer=NULL;
	//Create the effect - BMD
	//http://msdn.microsoft.com/en-us/library/bb172658%28v=vs.85%29.aspx -BMD
	if( FAILED(D3DX10CreateEffectFromFile( filename.c_str(), //The filename of the effect - BMD
		NULL, //An array of shader macros we leave this NULL - BMD
		NULL, //ID3D10Include*, this allows to include other files when we are compiling the effect - BMD
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        pDevice, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&m_pEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		&pErrorBuffer, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		//If the creation of the Effect fails then a message box will be shown
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
		return false;
    }
	return true;
}

bool D3D10Effect::loadFromMemory(const char * mem,ID3D10Device *pDevice)
{
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program. - BMD
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob * pErrorBuffer=NULL;
	if (FAILED(D3DX10CreateEffectFromMemory(mem,
		strlen(mem)+1,
		NULL,
		NULL,
		NULL,
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        pDevice, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&m_pEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		&pErrorBuffer, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		//If the creation of the Effect fails then a message box will be shown
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
		return false;
	}
	return true;
}