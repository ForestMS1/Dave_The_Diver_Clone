#include "pch.h"
#include "CEnding.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CFontMgr.h"
#include "CDInputMgr.h"
#include "CLog.h"
#include "CSoundMgr.h"
#include "CHelper.h"
#include "CAssetTexture.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CColliderMgr.h"
#include "CGraphicDev.h"
#include "CParticleMgr.h"
#include "CTransitionFade.h"

CEnding::CEnding()
	: CScene()
{
}

CEnding::~CEnding()
{
}

HRESULT CEnding::Ready_Scene()
{

	Update_Camera();
	CColliderMgr::GetInstance()->Set_Render(false);
	if (FAILED(Ready_Environment_Layer(L"0_Environment_Layer")))
		return E_FAIL;

	CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_Logo_BGM", CSoundMgr::BGM, 1.f);

	return S_OK;
}

_int CEnding::Update_Scene(const _float& fTimeDelta)
{
	Update_Camera();
	_int		iExit = CScene::Update_Scene(fTimeDelta);


	return iExit;
}

void CEnding::LateUpdate_Scene(const _float& fTimeDelta)
{
	Update_Camera();
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CEnding::Render_Scene()
{
#ifdef _DEBUG
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CEnding", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif // !_DEBUG
}

HRESULT CEnding::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

void CEnding::Update_Camera()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	D3DXMATRIX matView, matProj;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -2.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)WINCX / (float)WINCY, 0.1f, 1000.0f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEnding* CEnding::Create()
{
	CEnding* pLogo = new CEnding;

	if (FAILED(pLogo->Ready_Scene()))
	{
		Safe_Release(pLogo);
		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pLogo;
}

void CEnding::Free()
{
	CScene::Free();
	CColliderMgr::GetInstance()->Clear_ColliderGroup();
	CSoundMgr::GetInstance()->StopAll();
}
