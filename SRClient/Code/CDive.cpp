#include "CDive.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"

CDive::CDive()
	: CScene()
{
}
CDive::~CDive()
{
}

HRESULT CDive::Ready_Scene()
{
	return S_OK;
}

_int CDive::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CDive");
	if (ImGui::Button("Go Ship Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_DIVE, CTransition::SCENE_SHIP));
	}
	ImGui::End();
	return iExit;
}

void CDive::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CDive::Render_Scene()
{
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CDive", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CDive* CDive::Create()
{
	CDive* pDive = new CDive;

	if (FAILED(pDive->Ready_Scene()))
	{
		MSG_BOX("CDive Create Failed");
		Safe_Release(pDive);
		return nullptr;
	}
	return pDive;
}

void CDive::Free()
{
	CScene::Free();
}
