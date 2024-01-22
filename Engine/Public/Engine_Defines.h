#pragma once

#pragma warning (disable : 4251)

#include <d3d9.h>
#include <d3dx9.h>

#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <list>
#include <map>

#include <filesystem>
#include <iostream>
#include <cstdlib>

using namespace std;

namespace Engine
{
	const wstring g_strTransformTag = TEXT("Com_Transform");

	enum EKeyState
	{
		NONE, DOWN, PRESSING, UP
	};

#define		VK_MAX			0xff
}

#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Enum.h"

using namespace Engine;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

