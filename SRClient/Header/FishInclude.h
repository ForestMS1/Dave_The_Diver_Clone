#pragma once

#include "CBlueTang.h"
#include "CClownfish.h"
#include "CSmallspottedDart.h"
#include "CYellowbackFusilier.h"
#include "CYellowTang.h"
#include "CTitanTriggerfish.h"
#include "CBlobfish.h"
#include "CGreatSpiderCrab.h"
#include "CHumboldtSquid.h"
#include "CRedLionfish.h"
#include "CGreenHumpheadParrotfish.h"
#include "CGiantTrevally.h"
#include "CJuvenileCircularBatFish.h"
#include "CDevilScorpionFish.h"
#include "CAsianSheepshead.h"
#include "CGreyTriggerfish.h"
#include "CGreatBarracuda.h"
#include "CCowPatternSnapper.h"
#include "CCuttleFish.h"
#include "CAtlanticBonito.h"
#include "CAtlanticAnglerfish.h"
#include "CChamberedNautilus.h"
#include "CBigeyeTrevally.h"
#include "CLayer.h"

#include "CFishAABBCollider.h"
#include "CLayer.h"


namespace Fish
{
	//HRESULT AddLayer_BlueTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	//HRESULT AddLayer_TitanTriggerfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	//HRESULT AddLayer_Clownfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	//HRESULT AddLayer_SmallspottedDart(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	//HRESULT AddLayer_YellowbackFusilier(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	//HRESULT AddLayer_YellowTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);
	//HRESULT AddLayer_Blobfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent = nullptr);


	template<typename T>
	HRESULT AddLayer_Fish(
		std::wstring_view svFishObjName,
		Engine::CLayer* pLayer,
		float fPosX, float fPosY, float fScale,
		_vec3* pHitboxExtents,
		_vec3* pDetecboxExtents,
		CGameObject* pParent = nullptr)
	{
		T* pFish = T::Create(fPosX, fPosY, 0.05f * fScale);
		if (pParent)
		{
			pFish->Set_Parent(pParent);
		}
		pLayer->Add_GameObject(svFishObjName, pFish);

		{
			_vec3 vPos = { 0.0f, 0.f, 0.f };
			_vec3 vExt = *pHitboxExtents;
			vExt *= fScale;

			auto pAABB = CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish);
			CFishAABBCollider* pCollider = CFishAABBCollider::Create(
				pAABB,
				L"Coll_FishesHitbox");
			pCollider->Set_Parent(pFish);
			pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);

			pFish->Set_HitboxAABB(pAABB);
		}

		{
			_vec3 vPos = { 0.0f, 0.f, 0.f };
			_vec3 vExt = *pDetecboxExtents;
			//vExt *= fScale;

			CFishAABBCollider* pCollider = CFishAABBCollider::Create(
				CAABB::Create(&vPos, &vExt, L"AABB_FishDetectbox", pFish),
				L"Coll_FishesDetectBox");
			pCollider->Set_Parent(pFish);
			pCollider->Set_ApplyParentAngle(false);
			pLayer->Add_GameObject(L"FishDetetCollider", pCollider);
		}
		return S_OK;
	}

	template<typename T>
	HRESULT AddLayer_Fish(
		std::wstring_view svFishObjName,
		Engine::CLayer* pLayer,
		float fPosX, float fPosY, float fScale,
		CGameObject* pParent = nullptr)
	{
		_vec3 vDefaultHitboxExt = { 1.1f, 0.6f, 0.01f };
		_vec3 vDefaultDetecboxExt = { 2.f, 2.f, 0.01f };

		return AddLayer_Fish<T>(
			svFishObjName,
			pLayer,
			fPosX, fPosY, fScale,
			&vDefaultHitboxExt,
			&vDefaultDetecboxExt,
			pParent
			);
	}
}