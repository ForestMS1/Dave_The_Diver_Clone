#include "FishInclude.h"
#include "CLayer.h"


HRESULT Fish::AddLayer_BlueTang(Engine::CLayer* pLayer, float fPosX, float fPosY, float fScale, CGameObject* pParent)
{
	Fish::CBlueTang* pFish = Fish::CBlueTang::Create(fPosX, fPosX, 0.05f * fScale);
	if (pParent)
	{
		pFish->Set_Parent(pParent);
	}
	pLayer->Add_GameObject(L"FishBlueTang", pFish);



	_vec3 vPos = { 0.0f, 0.f, 0.f };
	_vec3 vExt = { 1.1f, 0.6f, 0.01f };
	vExt *= fScale;

	CFishHitCollider* pCollider = CFishHitCollider::Create(&vPos, &vExt);
	pCollider->Set_Parent(pFish);
	pLayer->Add_GameObject(L"FishColl", pCollider);

	return S_OK;
}
