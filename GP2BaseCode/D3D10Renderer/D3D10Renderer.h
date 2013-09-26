#pragma once

//The header file for the renderer interface
#include "../Renderer/Renderer.h"
#include <Windows.h>

//forward declarations of the D3D10 interfaces
struct ID3D10Device;
struct IDXGISwapChain;
struct ID3D10RenderTargetView;
struct ID3D10DepthStencilView;
struct ID3D10Texture2D;
struct ID3D10InputLayout;

//D3D10Renderer implements the Renderer interface
class D3D10Renderer:public IRenderer
{
public:
	D3D10Renderer();
	~D3D10Renderer();
	//notice these have the same signature as the pure methods
	//in the interface
	bool init(void *pWindowHandle,bool fullScreen);
	void clear(float r,float g,float b,float a);
	void present();

	IEffect * loadEffectFromFile(const wstring& name);
	IEffect * loadEffectFromMemory(const char* mem);

	IBuffer * createBuffer(unsigned int bufferSize, BUFFER_TYPE type,BUFFER_USAGE usage,void **pData);
private:
	bool createDevice(HWND pWindowHandle,int windowWidth, int windowHeight,
						bool fullScreen);
	bool createInitialRenderTarget(int windowWidth, int windowHeight);
	bool createDefaultVertexLayout();
private:
	//D3D10 stuff
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;

	IEffect *m_pDefaultEffect;
	//Vertex Layout
	ID3D10InputLayout*      m_pVertexLayout;
};