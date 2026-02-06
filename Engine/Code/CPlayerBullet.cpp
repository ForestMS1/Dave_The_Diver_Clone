#include "CPlayerBullet.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
CPlayerBullet::CPlayerBullet(_vec3 vOrigin, _vec3 vDir, _float speed)
	: m_vDir(vDir)
	, m_vOrigin(vOrigin)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_fSpeed(speed)
	, m_fLifeTime(0.f)
	, m_fFrame(0.f)
{
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet& rhs)
	: CGameObject(rhs)
{
}

CPlayerBullet::~CPlayerBullet()
{
}

HRESULT CPlayerBullet::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	return S_OK;
}

_int CPlayerBullet::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	m_pTransformCom->Move_Pos(&m_vDir, m_fSpeed, fTimeDelta);
	m_fLifeTime += fTimeDelta;
	m_fFrame += 3.f * fTimeDelta;
	if (3.f < m_fFrame)
		m_fFrame = 0.f;
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	return iExit;
}

void CPlayerBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Set_BillBoard();
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	
	if (m_fLifeTime > 10.f)
		m_bDead = true;
}

void CPlayerBullet::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
}

HRESULT CPlayerBullet::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom =
		dynamic_cast<Engine::CRcTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// texture 
	pComponent = m_pTextureCom =
		dynamic_cast<Engine::CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_BulletTexture"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	// Transform
	pComponent = m_pTransformCom =
		dynamic_cast<Engine::CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}
void CPlayerBullet::Set_BillBoard()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	_matrix		matBill, matWorld, matView;

	matWorld = *m_pTransformCom->Get_World();
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	// y축 회전만 제거
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 주의 할 것
	matWorld = matBill * matWorld;

	m_pTransformCom->Set_World(&matWorld);

	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}
CPlayerBullet* CPlayerBullet::Create(_vec3 vOrigin, _vec3 vDir, _float speed)
{
	CPlayerBullet* pBullet = new CPlayerBullet(vOrigin, vDir, speed);
	if (FAILED(pBullet->Ready_GameObject()))
	{
		Safe_Release(pBullet);
		MSG_BOX("Bullet Create Failed");
		return nullptr;
	}
	return pBullet;
}
void CPlayerBullet::Free()
{
	CGameObject::Free();
}
