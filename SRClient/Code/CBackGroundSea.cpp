#include "pch.h"
#include "CBackGroundSea.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CMapMgr.h"
#include "CGameMemMgr.h"
#include "CJohn.h"

CBackGroundSea::CBackGroundSea()
	: CGameObject()
{
}

CBackGroundSea::CBackGroundSea(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CBackGroundSea::~CBackGroundSea()
{
}

HRESULT CBackGroundSea::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;



	return S_OK;
}

_int CBackGroundSea::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_TERRIAN, this);
	_vec3 vPos{};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
	return iExit;
}

void CBackGroundSea::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	LightDown();
}

void CBackGroundSea::Render_GameObject()
{
	//LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



	//pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());



	//m_pTextureCom->Set_Texture(0);

	//m_pBufferCom->Render_Buffer();

	//D3DXMATRIX matTmp;
	//D3DXMatrixIdentity(&matTmp);
	//pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);


	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	IDirect3DStateBlock9* pState = nullptr;
	pGraphicDev->CreateStateBlock(D3DSBT_ALL, &pState);
	pState->Capture();
	_vec3 floor{};
	if (CMapMgr::GetInstance()->GetScene() != nullptr) {
		CGameObject* pDiveDave = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave");
		CTransform* pDaveTransform = static_cast<CTransform*>(pDiveDave->Get_Component(ID_DYNAMIC, L"Com_Transform"));

		pDaveTransform->Get_Info(INFO_POS, &floor);
	}
	
	_float Dark = CGameMemMgr::GetInstance()->Get_Dark();

	if (floor.y <= -130.f ) {
		if (!CGameMemMgr::GetInstance()->Get_BossAppear()) {
			CGameMemMgr::GetInstance()->Set_BossAppear(true);

			// 보스
			CGameObject* pGameObject = CJohn::Create(13.f, -87.f, 0.f);
			CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"John", pGameObject);


		}
	}

	float t = 1.f - Dark;

	// 시작색 (밝은)
	float r0 = 255.f;
	float g0 = 255.f;
	float b0 = 255.f;


	// 목표색 (어두운)
	float r1 = 100.f;
	float g1 = 100.f;
	float b1 = 100.f;


	// 선형보간
	BYTE r = (BYTE)(r0 + (r1 - r0) * t);
	BYTE g = (BYTE)(g0 + (g1 - g0) * t);
	BYTE b = (BYTE)(b0 + (b1 - b0) * t);

	DWORD tfactor = D3DCOLOR_ARGB(255, r, g, b);

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);

	pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	// 알파는 원본 텍스쳐 알파 그대로
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);


	m_pBufferCom->Render_Buffer();
	// -------------------------------------------------------------

	pState->Apply();
	pState->Release();




	D3DXMATRIX matTmp;
	D3DXMatrixIdentity(&matTmp);
	pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

}

HRESULT CBackGroundSea::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	// Buffer
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;
	// texture
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BackGroundSea", L"Com_Texture", &m_pTextureCom))))
		return E_FAIL;

	// Transform
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;



	return S_OK;
}


CBackGroundSea* CBackGroundSea::Create()
{
	CBackGroundSea* pBackGround = new CBackGroundSea;

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("CBackGroundSea Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CBackGroundSea::Free()
{
	CGameObject::Free();
}

void CBackGroundSea::LightDown() {





}