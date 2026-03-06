#include "CJohnBullet.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CFishGameObject.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CManagement.h"
#include "CDiveDave.h"
#include "CJohn.h"
CJohnBullet::CJohnBullet(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
	: m_vDir(vDir)
	, m_fZAngle(fZAngle)
	, m_vOrigin(vOrigin)
{
}

CJohnBullet::CJohnBullet(const CJohnBullet& rhs)
	: CGameObject(rhs)
{
}

CJohnBullet::~CJohnBullet()
{
}

HRESULT CJohnBullet::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_pTransformCom->Set_Pos(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z);
	m_pTransformCom->Rotation(ROT_Z, m_fZAngle);

	_float fWidth = 28.f;
	_float fHeight = 17.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.5f;
	m_pTransformCom->Multiply_Scale(&vScale);

	//-------------AABB Collider----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	_vec3 vPos = { 0.0f, 0.0f, 0.0f };

	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_JohnBullet", this);

	m_wsTexName = L"JohnBulletA";
	return S_OK;
}

_int CJohnBullet::Update_GameObject(const _float& fTimeDelta)
{
	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > 6.f)
	{
		m_bDead = true;
		return 0;
	}
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());


	FSM(fTimeDelta);
	_vec3 rot = { 0.f, 0.f, m_fZAngle };
	m_pTransformCom->Set_Rotation(&rot);
	CGameObject::Update_GameObject(fTimeDelta);

	return _int();
}

void CJohnBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiveDaveWithItemBox"))
	{
		for (auto& pCollider : *pColliders)
		{
			if (m_pAABB->Intersect(pCollider))
			{
				if (pCollider->Get_Tag() == L"AABB_DiveDaveWithItemBox")
				{
					//reinterpret_cast<CFishGameObject*>(pCollider->Get_VoidPtr())->Damaged(1);
					if (m_eCurState == CHASE)
					{
						_float fWidth = 28.f;
						_float fHeight = 17.f;
						_float fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
						vScale *= 0.5f;
						m_pTransformCom->Multiply_Scale(&vScale);
						//---------------------------------------------------------------
						m_eCurState = EXLPOSION;
						//---------------------------------------------------------------
					}
					if (m_eCurState == EXLPOSION)
					{
						static_cast<CDiveDave*>(pCollider->Get_VoidPtr())->On_Hit(10.f);
					}
					//m_bDead = true;
				}
			}
		}
	}
}

void CJohnBullet::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());



	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTexName))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at((_uint)m_fFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}


	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CJohnBullet::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CJohnBullet::FSM(const _float& fTimeDelta)
{
	switch (m_eCurState)
	{
	case CHASE:
		m_wsTexName = L"JohnBulletA";
		AddFrame(fTimeDelta, 5.f, 2, true);
		Fire(fTimeDelta);
		break;
	case EXLPOSION:
		m_wsTexName = L"JohnBulletExplosion";
		if (!m_bCompleteSetSize)
		{
			m_bCompleteSetSize = true;
			_float fWidth = 28.f;
			_float fHeight = 17.f;
			_float fAspect = fWidth + fHeight;
			fAspect /= 2.f;
			fWidth = 51.f;
			fHeight = 51.f;
			fAspect = fWidth + fHeight;
			fAspect /= 2.f;

			_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
			vScale *= 5.f;
			m_pTransformCom->Multiply_Scale(&vScale);
		}
		AddFrame(fTimeDelta, 5.f, 9, false);
		Explosion(fTimeDelta);
		break;
	default:
		break;
	}
}
void CJohnBullet::Fire(const _float& fTimeDelta)
{
	m_pTargetTransform = static_cast<CTransform*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
	if (m_pTargetTransform == nullptr)
		return;



	m_pTransformCom->Move_Pos(&m_vDir, 5.f, fTimeDelta);

	_vec3 vAxisX = { 1.f, 0.f, 0.f };

	_float dot = vAxisX.x * m_vDir.x + vAxisX.y * m_vDir.y;
	_float cross = vAxisX.x * m_vDir.y - vAxisX.y * m_vDir.x;
	_float fAngle = D3DXToDegree(atan2(cross, dot));
	m_fZAngle = fAngle;
}

void CJohnBullet::Explosion(const _float& fTimeDelta)
{
	m_fExplosionTime += fTimeDelta;
	if (m_fExplosionTime > 1.f)
	{
		m_bDead = true;
		return;
	}

}


CJohnBullet* CJohnBullet::Create(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
{
	CJohnBullet* pBullet = new CJohnBullet(vOrigin, vDir, fZAngle);

	if (FAILED(pBullet->Ready_GameObject()))
	{
		Safe_Release(pBullet);
		MSG_BOX("DiveDaveBullet Create Failed");
		return nullptr;
	}
	return pBullet;
}

void CJohnBullet::Free()
{
	Safe_Release(m_pAABB);
	CGameObject::Free();
}


void CJohnBullet::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop)
{
	m_fFrame += fSpeed * fTimeDelta;
	if (loop)
	{
		if (m_fFrame > size)
			m_fFrame = 0.f;
	}
	else
	{
		if (m_fFrame > size)
			m_fFrame = size - 1;
	}
}
