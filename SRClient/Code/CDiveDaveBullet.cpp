#include "CDiveDaveBullet.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
CDiveDaveBullet::CDiveDaveBullet(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
    : m_vDir(vDir)
	, m_fZAngle(fZAngle)
	, m_vOrigin(vOrigin)
{
}

CDiveDaveBullet::CDiveDaveBullet(const CDiveDaveBullet& rhs)
    : CGameObject(rhs)
{
}

CDiveDaveBullet::~CDiveDaveBullet()
{
}

HRESULT CDiveDaveBullet::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z);
	m_pTransformCom->Rotation(ROT_Z, m_fZAngle);

	_float fWidth = 15.f;
	_float fHeight = 7.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.1f;
	m_pTransformCom->Multiply_Scale(&vScale);

	return S_OK;
}

_int CDiveDaveBullet::Update_GameObject(const _float& fTimeDelta)
{
	if (m_fLifeTime > 3.f)
		m_bDead = true;
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	m_fLifeTime += fTimeDelta;
	m_pTransformCom->Move_Pos(&m_vDir, 10.f, fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);

    return _int();
}

void CDiveDaveBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDiveDaveBullet::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDiveDaveBullet::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 텍스쳐
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BulletTexture", L"Com_Texture", &m_pTextureCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CDiveDaveBullet* CDiveDaveBullet::Create(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
{
	CDiveDaveBullet* pBullet = new CDiveDaveBullet(vOrigin, vDir, fZAngle);

	if (FAILED(pBullet->Ready_GameObject()))
	{
		Safe_Release(pBullet);
		MSG_BOX("DiveDaveBullet Create Failed");
		return nullptr;
	}
	return pBullet;
}

void CDiveDaveBullet::Free()
{
	CGameObject::Free();
}
