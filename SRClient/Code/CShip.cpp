#include "CShip.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"

CShip::CShip()
	: CScene()
{
}
CShip::~CShip()
{
}

HRESULT CShip::Ready_Scene()
{
	return S_OK;
}

_int CShip::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CShip");
	if (ImGui::Button("Go Dive Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_DIVE));
	}
	if (ImGui::Button("Go Sushi Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_SUSHI));
	}
	if (ImGui::Button("Go Logo Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_LOGO));
	}
	ImGui::End();
	return iExit;
}

void CShip::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CShip::Render_Scene()
{
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CShip", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CShip* CShip::Create()
{
	CShip* pShip = new CShip;

	if (FAILED(pShip->Ready_Scene()))
	{
		MSG_BOX("CShip Create Failed");
		Safe_Release(pShip);
		return nullptr;
	}
	return pShip;
}

void CShip::Free()
{
	CScene::Free();
}
