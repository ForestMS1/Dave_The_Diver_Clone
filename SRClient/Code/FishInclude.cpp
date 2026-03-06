#include "FishInclude.h"
#include "CLayer.h"
#include "CAABB.h"

HRESULT Fish::AddLayer_BlueTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CBlueTang* pFish = Fish::CBlueTang::Create(fPosX, fPosY, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"FishBlueTang", pFish);

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 1.1f, 0.6f, 0.01f };
		vExt *= fScale;

		CFishAABBCollider* pCollider = CFishAABBCollider::Create(
			CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish),
			L"Coll_FishesHitbox");
		pCollider->Set_Parent(pFish);
		pCollider->Set_Tag(L"FishHitBoxCollider");
		pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);
	}

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 2.f, 2.f, 0.01f };
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

HRESULT Fish::AddLayer_TitanTriggerfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CTitanTriggerfish* pFish = Fish::CTitanTriggerfish::Create(fPosX, fPosY, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"FishTitanTriggerfish", pFish);

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 1.1f, 0.6f, 0.01f };
		vExt *= fScale;

		CFishAABBCollider* pCollider = CFishAABBCollider::Create(
			CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish),
			L"Coll_FishesHitbox");
		pCollider->Set_Parent(pFish);
		pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);
	}

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 2.f, 2.f, 0.01f };
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

HRESULT Fish::AddLayer_Clownfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CClownfish* pFish = Fish::CClownfish::Create(fPosX, fPosY, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"Clownfish", pFish);

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 1.1f, 0.6f, 0.01f };
		vExt *= fScale;

		CFishAABBCollider* pCollider = CFishAABBCollider::Create(
			CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish),
			L"Coll_FishesHitbox");
		pCollider->Set_Parent(pFish);
		pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);
	}

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 2.f, 2.f, 0.01f };
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

HRESULT Fish::AddLayer_SmallspottedDart(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CSmallspottedDart* pFish = Fish::CSmallspottedDart::Create(fPosX, fPosY, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"SmallspottedDart", pFish);

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 1.1f, 0.6f, 0.01f };
		vExt *= fScale;

		CFishAABBCollider* pCollider = CFishAABBCollider::Create(
			CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish),
			L"Coll_FishesHitbox");
		pCollider->Set_Parent(pFish);
		pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);
	}

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 2.f, 2.f, 0.01f };
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

HRESULT Fish::AddLayer_YellowbackFusilier(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CYellowbackFusilier* pFish = Fish::CYellowbackFusilier::Create(fPosX, fPosY, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"YellowbackFusilier", pFish);

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 1.1f, 0.6f, 0.01f };
		vExt *= fScale;

		CFishAABBCollider* pCollider = CFishAABBCollider::Create(
			CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish),
			L"Coll_FishesHitbox");
		pCollider->Set_Parent(pFish);
		pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);
	}

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 2.f, 2.f, 0.01f };
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

HRESULT Fish::AddLayer_YellowTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CYellowTang* pFish = Fish::CYellowTang::Create(fPosX, fPosY, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"YellowTang", pFish);

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 1.1f, 0.6f, 0.01f };
		vExt *= fScale;

		CFishAABBCollider* pCollider = CFishAABBCollider::Create(
			CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish),
			L"Coll_FishesHitbox");
		pCollider->Set_Parent(pFish);
		pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);
	}

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 2.f, 2.f, 0.01f };
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

HRESULT Fish::AddLayer_Blobfish(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CBlobfish* pFish = Fish::CBlobfish::Create(fPosX, fPosY, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"Blobfish", pFish);

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 1.1f, 0.6f, 0.01f };
		vExt *= fScale;

		CFishAABBCollider* pCollider = CFishAABBCollider::Create(
			CAABB::Create(&vPos, &vExt, L"AABB_FishHitbox", pFish),
			L"Coll_FishesHitbox");
		pCollider->Set_Parent(pFish);
		pLayer->Add_GameObject(L"FishHitboxCollider", pCollider);
	}

	{
		_vec3 vPos = { 0.0f, 0.f, 0.f };
		_vec3 vExt = { 2.f, 2.f, 0.01f };
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
