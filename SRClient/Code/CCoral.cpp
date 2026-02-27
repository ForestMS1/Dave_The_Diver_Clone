#include "pch.h"
#include "CCoral.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CAssetMgr.h"
CCoral::CCoral()
	: CGameObject()
{
}

CCoral::CCoral(const wstring_view _TextureName) : CGameObject(), m_TextureName(_TextureName)
{
}

CCoral::CCoral(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CCoral::~CCoral()
{
}

HRESULT CCoral::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vScale = { 0.5f, 0.5f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	//D3DXIMAGE_INFO imgInfo;// = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset()->Get_ImgInfo();
	//imgInfo.Width;

	//_float fWidth = imgInfo.Width;;
	//_float fHeight = imgInfo.Height;
	//_float fAspect = fWidth + fHeight;
	//fAspect /= 2.f;

	//vScale = { fWidth / fAspect, fHeight / fAspect, 0.f };
	//m_pTransformCom->Multiply_Scale(&vScale);

	//m_pTransformCom->Set_Pos(0,0,0);




	return S_OK;
}

_int CCoral::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	return iExit;
}

void CCoral::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);



}

void CCoral::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pBufferCom->Render_Buffer();

	D3DXMATRIX matTmp;
	D3DXMatrixIdentity(&matTmp);
	pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

}

HRESULT CCoral::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	// Buffer
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// Transform
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

}


CCoral* CCoral::Create(const wstring_view _TextureName)
{
	CCoral* pCoral = new CCoral(_TextureName);

	if (FAILED(pCoral->Ready_GameObject()))
	{
		Safe_Release(pCoral);
		MSG_BOX("pCoral Create Failed");
		return nullptr;
	}

	return pCoral;
}

void CCoral::Free()
{
	CGameObject::Free();
}
