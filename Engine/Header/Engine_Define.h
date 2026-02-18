#ifndef Engine_Define_h__
#define Engine_Define_h__

// ENGINE_EXPORTS 는
// 엔진프로젝트에만 적용되는 매크로이다
// 여기에 정의하는것들은 클라에 안들어간다
#ifdef	ENGINE_EXPORTS

#include <spine/spine.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace spine;

#endif

#include <tweeny/tween.h>

#include <nlohmann/json.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include <DirectXMath.h>
using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <string_view>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <ctime>

#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
//#include "ImSequencer.h"
//#include "ImZoomSlider.h"
//#include "ImCurveEdit.h"
//#include "GraphEditor.h"


#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

#include <cstdarg>

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Engine;

#endif // Engine_Define_h__
