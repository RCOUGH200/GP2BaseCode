#pragma once

#include <D3D10.h>

//http://www.gamedev.net/topic/506017-quick-question-about-id3d10inputlayout/
// Define the input layout of the vertex, this is so we can bind a vertex to the pipeline - BMD
const D3D10_INPUT_ELEMENT_DESC SimpleVerexLayout[] =
{		
    { "POSITION", //Name of the semantic, this helps to bind the vertex inside the Vertex Shader - BMD
	0, //The index of the semantic, see above - BMD
	DXGI_FORMAT_R32G32B32_FLOAT, //The format of the element, in this case 32 bits of each sub element - BMD
	0, //Input slot - BMD
	0, //Offset, this will increase as we add more elements(such texture coords) to the layout - BMD
	D3D10_INPUT_PER_VERTEX_DATA, //Input classification - BMD
	0 }, //Instance Data slot - BMD
};