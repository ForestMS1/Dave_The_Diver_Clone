#pragma once

#include "CBlueTang.h"
#include "CClownfish.h"
#include "CSmallspottedDart.h"
#include "CYellowbackFusilier.h"
#include "CYellowTang.h"
#include "CTitanTriggerfish.h"
#include "CFishAABBCollider.h"

namespace Engine
{
	class CLayer;
}

namespace Fish
{
	HRESULT AddLayer_BlueTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	HRESULT AddLayer_TitanTriggerfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	HRESULT AddLayer_Clownfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	HRESULT AddLayer_SmallspottedDart(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	HRESULT AddLayer_YellowbackFusilier(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	HRESULT AddLayer_YellowTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
}