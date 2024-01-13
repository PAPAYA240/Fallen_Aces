#pragma once

namespace Engine
{
	typedef struct
	{
		HWND			hWnd;
		unsigned int	iWinSizeX, iWinSizeY;

	}ENGINE_DESC;

	typedef struct
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord;	
		
	}VTXPOSTEX;

	typedef struct
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexcoord; /* 중심으로부터의 코너 방향성을 저장한다. */

	}VTXCUBE;

	/* 형준 추가*/
	typedef struct
	{
		D3DXVECTOR3			vPosition;
	}VTXCOLLIDER;
	
	// D3DDECLUSAGE


}