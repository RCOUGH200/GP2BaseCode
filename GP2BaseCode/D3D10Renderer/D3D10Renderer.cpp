#include "D3D10Renderer.h"

#include <D3D10.h>
#include <D3DX10.h>

struct Vertex
{
	float x,y,z;
};

//http://www.gamedev.net/topic/506017-quick-question-about-id3d10inputlayout/
// Define the input layout of the vertex, this is so we can bind a vertex to the pipeline - BMD
const D3D10_INPUT_ELEMENT_DESC VerexLayout[] =
{		
    { "POSITION", //Name of the semantic, this helps to bind the vertex inside the Vertex Shader - BMD
	0, //The index of the semantic, see above - BMD
	DXGI_FORMAT_R32G32B32_FLOAT, //The format of the element, in this case 32 bits of each sub element - BMD
	0, //Input slot - BMD
	0, //Offset, this will increase as we add more elements(such texture coords) to the layout - BMD
	D3D10_INPUT_PER_VERTEX_DATA, //Input classification - BMD
	0 }, //Instance Data slot - BMD
};

const char basicEffect[]=\
	"float4 VS( float4 Pos : POSITION ) : SV_POSITION"\
	"{"\
	"	return Pos;"\
	"}"\
	"float4 PS( float4 Pos : SV_POSITION ) : SV_Target"\
	"{"\
	"	return float4( 1.0f, 1.0f, 0.0f, 1.0f );"\
	"}"\
	"technique10 Render"\
	"{"\
	"	pass P0"\
	"	{"\
	"		SetVertexShader( CompileShader( vs_4_0, VS() ) );"\
	"		SetGeometryShader( NULL );"\
	"		SetPixelShader( CompileShader( ps_4_0, PS() ) );"\
	"	}"\
	"}";

D3D10Renderer::D3D10Renderer()
{
	m_pD3D10Device=NULL;
	m_pRenderTargetView=NULL;
	m_pSwapChain=NULL;
	m_pDepthStencelView=NULL;
	m_pDepthStencilTexture=NULL;

	m_pTempEffect=NULL;
	m_pTempBuffer=NULL;
	m_pTempVertexLayout=NULL;
}

D3D10Renderer::~D3D10Renderer()
{
	if (m_pD3D10Device)
		m_pD3D10Device->ClearState();

	if (m_pTempEffect)
		m_pTempEffect->Release();
	if (m_pTempVertexLayout)
		m_pTempVertexLayout->Release();
	if (m_pTempBuffer)
		m_pTempBuffer->Release();

	if (m_pRenderTargetView)
		m_pRenderTargetView->Release();
	if (m_pDepthStencelView)
		m_pDepthStencelView->Release();
	if (m_pDepthStencilTexture)
		m_pDepthStencilTexture->Release();
	if (m_pSwapChain)
		m_pSwapChain->Release();
	if (m_pD3D10Device)
		m_pD3D10Device->Release();
}

bool D3D10Renderer::init(void *pWindowHandle,bool fullScreen)
{
	HWND window=(HWND)pWindowHandle;
	RECT windowRect;
	GetClientRect(window,&windowRect);

	UINT width=windowRect.right-windowRect.left;
	UINT height=windowRect.bottom-windowRect.top;

	if (!createDevice(window,width,height,fullScreen))
		return false;
	if (!createInitialRenderTarget(width,height))
		return false;

	if (!loadEffectFromMemory(basicEffect))
		return false;
	if (!creatVertexLayout())
		return false;
	if (!createBuffer())
		return false;
	return true;
}

bool D3D10Renderer::createDevice(HWND window,int windowWidth, int windowHeight,bool fullScreen)
{
	UINT createDeviceFlags=0;
#ifdef _DEBUG
	createDeviceFlags|=D3D10_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd;
       ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	if (fullScreen)
		sd.BufferCount = 2;
	else 
		sd.BufferCount=1;
	sd.OutputWindow = window;
	sd.Windowed = (BOOL)(!fullScreen);
       sd.SampleDesc.Count = 1;
       sd.SampleDesc.Quality = 0;
       sd.BufferDesc.Width = windowWidth;
       sd.BufferDesc.Height = windowHeight;
       sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
       sd.BufferDesc.RefreshRate.Numerator = 60;
       sd.BufferDesc.RefreshRate.Denominator = 1;

	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, 
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL, 
		createDeviceFlags,
		D3D10_SDK_VERSION,		
              &sd,
		&m_pSwapChain, 
		&m_pD3D10Device)))                       
		return false;

	return true;
}

bool D3D10Renderer::createInitialRenderTarget(int windowWidth, int windowHeight)
{
	ID3D10Texture2D *pBackBuffer;
	
	if (FAILED(m_pSwapChain->GetBuffer(0, 
		__uuidof(ID3D10Texture2D),
		(void**)&pBackBuffer))) 
		return false;

	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width=windowWidth;
	descDepth.Height=windowHeight;
	descDepth.MipLevels=1;
	descDepth.ArraySize=1;
	descDepth.Format=DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count=1;
	descDepth.SampleDesc.Quality=0;
	descDepth.Usage=D3D10_USAGE_DEFAULT;
	descDepth.BindFlags=D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags=0;
	descDepth.MiscFlags=0;

	if (FAILED(m_pD3D10Device->CreateTexture2D(&descDepth,NULL,
			&m_pDepthStencilTexture)))
		return false;

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format=descDepth.Format;
	descDSV.ViewDimension=D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice=0;

	if (FAILED(m_pD3D10Device->CreateDepthStencilView(m_pDepthStencilTexture,
                   &descDSV,&m_pDepthStencelView)))
		return false;

	if (FAILED(m_pD3D10Device->CreateRenderTargetView( pBackBuffer, 
		NULL, 
		&m_pRenderTargetView ))){
             pBackBuffer->Release();
		return  false;
	}
       pBackBuffer->Release();

	m_pD3D10Device->OMSetRenderTargets(1, 
		&m_pRenderTargetView,		
		m_pDepthStencelView);
	
	D3D10_VIEWPORT vp;
   	vp.Width = windowWidth;
    vp.Height = windowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    
	m_pD3D10Device->RSSetViewports( 1 
		, &vp );
	return true;
}

bool D3D10Renderer::loadEffectFromMemory(const char *pMem)
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
	if (FAILED(D3DX10CreateEffectFromMemory(pMem,
		strlen(pMem),
		NULL,
		NULL,
		NULL,
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        m_pD3D10Device, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&m_pTempEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		&pErrorBuffer, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
		return false;
	}

	m_pTempTechnique=m_pTempEffect->GetTechniqueByName("Render");
	return true;
}

bool D3D10Renderer::createBuffer()
{
	Vertex verts[]={{-1.0f,-1.0f,0.0f},{0.0f,1.0f,0.0f},{1.0f,-1.0f,0.0f}};
	//Buffer desc
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;//Usuage flag,this describes how the buffer is read/written to. Default is the most common - BMD
	bd.ByteWidth = sizeof( Vertex ) * 3;//The size of the buffer, this is the size of one vertex * by the num of vertices -BMD
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;//BindFlags, says how the buffer is going to be used. In this case as a Vertex Buffer - BMD
	bd.CPUAccessFlags = 0;//CPUAccessFlag, sepcfies if the CPU can access the resource. 0 means no CPU access - BMD
	bd.MiscFlags = 0;//MiscCreation flags, this will be zero most of the time - BMD

	//This is used to supply the initial data for the buffer - BMD
	//http://msdn.microsoft.com/en-us/library/bb172456%28VS.85%29.aspx - BMD
	D3D10_SUBRESOURCE_DATA InitData;
	//A pointer to the initial data
	InitData.pSysMem = &verts;
    
	//Create the Buffer using the buffer description and initial data - BMD
	//http://msdn.microsoft.com/en-us/library/bb173544%28v=VS.85%29.aspx - BMD
	if (FAILED(m_pD3D10Device->CreateBuffer( 
		&bd, //Memory address of a buffer description - BMD
		&InitData, //Memory address of the initial data - BMD
		&m_pTempBuffer )))//A pointer to a memory address of a buffer, this will be initialise after - BMD
	{
		OutputDebugStringA("Can't create vertex buffer");
	}
	return true;
}

bool D3D10Renderer::creatVertexLayout()
{
	//Number of elements in the layout - BMD
    UINT numElements = sizeof( VerexLayout ) / sizeof(D3D10_INPUT_ELEMENT_DESC);
	//Get the Pass description, we need this to bind the vertex to the pipeline - BMD
    D3D10_PASS_DESC PassDesc;
    m_pTempTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	//Create Input layout to describe the incoming buffer to the input assembler - BMD
    if (FAILED(m_pD3D10Device->CreateInputLayout( VerexLayout, //The layout describing our vertices - BMD
		numElements, //The number of elements in the layout
		PassDesc.pIAInputSignature,//Input signature of the description of the pass - BMD
        PassDesc.IAInputSignatureSize, //The size of this Signature size of the pass - BMD
		&m_pTempVertexLayout ))) //The pointer to an address of Vertex Layout - BMD
	{
		OutputDebugStringA("Can't create layout");
	}
	return true;
}

void D3D10Renderer::clear(float r,float g,float b,float a)
{
    // Just clear the backbuffer, colours start at 0.0 to 1.0
	// Red, Green , Blue, Alpha - BMD
    const float ClearColor[4] = { r, g, b, a}; 
	//Clear the Render Target
	//http://msdn.microsoft.com/en-us/library/bb173539%28v=vs.85%29.aspx - BMD
    m_pD3D10Device->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	m_pD3D10Device->ClearDepthStencilView(m_pDepthStencelView,D3D10_CLEAR_DEPTH,1.0f,0);
}

void D3D10Renderer::render()
{
	//Set the type of primitive
	m_pD3D10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//Set Vertex Laout
	m_pD3D10Device->IASetInputLayout(m_pTempVertexLayout);

    //Get the stride(size) of the a vertex, we need this to tell the pipeline the size of one vertex - BMD
    UINT stride = sizeof( Vertex );
    //The offset from start of the buffer to where our vertices are located - BMD
    UINT offset = 0;
    //Bind the vertex buffer to input assembler stage - BMD
    //http://msdn.microsoft.com/en-us/library/bb173591%28v=VS.85%29.aspx - BMD
    m_pD3D10Device->IASetVertexBuffers( 
		0, //The input slot to bind, zero indicates the first slot - BMD
		1, //The number of buffers - BMD
		&m_pTempBuffer, //A pointer to an array of vertex buffers - BMD
		&stride, //Pointer to an array of strides of vertices in the buffer - BMD
		&offset );//Pointer to an array of offsets to the vertices in the vertex buffers - BMD

	D3D10_TECHNIQUE_DESC techniqueDesc;
	m_pTempTechnique->GetDesc(&techniqueDesc);

	for (unsigned int i=0;i<techniqueDesc.Passes;++i)
	{
		ID3D10EffectPass *pCurrentPass=m_pTempTechnique->GetPassByIndex(0);
		pCurrentPass->Apply(0);
		m_pD3D10Device->Draw(3,0);
	}

	

}

void D3D10Renderer::present()
{
	//Swaps the buffers in the chain, the back buffer to the front(screen)
	//http://msdn.microsoft.com/en-us/library/bb174576%28v=vs.85%29.aspx - BMD
    m_pSwapChain->Present( 0, 0 );
}