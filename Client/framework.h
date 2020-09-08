#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <memory>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <ctime>
#include <string>
//#include <algorithm>
using namespace std;

#include "d3d9.h"
#pragma comment(lib, "d3d9.lib")
#include "d3dx9.h"
#pragma comment(lib, "d3dx9.lib")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "Total.h"
#include "Define.h"
#include "CAbstractFactory.h"
#include "Struct.h"
#include "Function.h"
#include "Const.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <malloc.h>
#include <crtdbg.h>
#define _VC_DEBUG
#endif

#include <io.h>
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")
#include "CSoundManager.h"

