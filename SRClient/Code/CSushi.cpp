#include "CSushi.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"

CSushi::CSushi()
	: CScene()
{
}
CSushi::~CSushi()
{
}

HRESULT CSushi::Ready_Scene()
{
	return S_OK;
}

_int CSushi::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CSushi");
	if (ImGui::Button("Go Ship Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SUSHI, CTransition::SCENE_SHIP));
	}
	ImGui::End();
	return iExit;
}

void CSushi::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CSushi::Render_Scene()
{
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CSushi", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CSushi* CSushi::Create()
{
	CSushi* pSushi = new CSushi;

	if (FAILED(pSushi->Ready_Scene()))
	{
		MSG_BOX("CSushi Create Failed");
		Safe_Release(pSushi);
		return nullptr;
	}
	return pSushi;
}

void CSushi::Free()
{
	CScene::Free();
}
