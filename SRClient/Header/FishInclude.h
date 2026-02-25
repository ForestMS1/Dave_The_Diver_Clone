#pragma once

#include "CBlueTang.h"
#include "CClownfish.h"
#include "CSmallspottedDart.h"
#include "CYellowbackFusilier.h"
#include "CYellowTang.h"
#include "CFishAABBCollider.h"

namespace Engine
{
	class CLayer;
}

namespace Fish
{
	HRESULT AddLayer_BlueTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
}