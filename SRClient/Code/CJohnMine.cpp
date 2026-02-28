#include "CJohnMine.h"
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
CJohnMine::CJohnMine(_vec3 vOrigin, _vec3 vDir)
	: m_vDir(vDir)
	, m_vOrigin(vOrigin)
{
}

CJohnMine::CJohnMine(const CJohnMine& rhs)
	: CGameObject(rhs)
{
}

CJohnMine::~CJohnMine()
{
}

HRESULT CJohnMine::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_pTransformCom->Set_Pos(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z);

	_float fWidth = 13.;
	_float fHeight = 13.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.5f;
	m_pTransformCom->Multiply_Scale(&vScale);

	//-------------AABB Collider----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	_vec3 vPos = { 0.0f, 0.0f, 0.0f };

	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_JohnMine", this);

	m_wsTexName = L"JohnMine";
	return S_OK;
}

_int CJohnMine::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());


	FSM(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);

	return _int();
}

void CJohnMine::LateUpdate_GameObject(const _float& fTimeDelta)
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
					if (m_eCurState == MOVE)
					{
						_float fWidth = 13.f;
						_float fHeight = 13.f;
						_float fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
						vScale *= 0.5f;
						m_pTransformCom->Multiply_Scale(&vScale);
						//---------------------------------------------------------------
						m_eCurState = EXPLOSION;
						//---------------------------------------------------------------
						fWidth = 51.f;
						fHeight = 51.f;
						fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
						vScale *= 5.f;
						m_pTransformCom->Multiply_Scale(&vScale);
					}
					if (m_eCurState == EXPLOSION_READY)
					{
						_float fWidth = 13.f;
						_float fHeight = 13.f;
						_float fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
						vScale *= 0.5f;
						m_pTransformCom->Multiply_Scale(&vScale);
						//---------------------------------------------------------------
						m_eCurState = EXPLOSION;
						//---------------------------------------------------------------
						fWidth = 51.f;
						fHeight = 51.f;
						fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
						vScale *= 5.f;
						m_pTransformCom->Multiply_Scale(&vScale);
					}
					if (m_eCurState == EXPLOSION)
					{
						static_cast<CDiveDave*>(pCollider->Get_VoidPtr())->On_Hit(10.f);
					}
					//m_bDead = true;
				}
			}
		}
	}
}

void CJohnMine::Render_GameObject()
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

HRESULT CJohnMine::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CJohnMine::FSM(const _float& fTimeDelta)
{
	switch (m_eCurState)
	{
	case MOVE:
		AddFrame(fTimeDelta, 5.f, 2, true);
		Move(fTimeDelta);
		break;
	case EXPLOSION_READY:
		AddFrame(fTimeDelta, 5.f, 2, true);
		StopReady(fTimeDelta);
		break;
	case EXPLOSION:
		m_wsTexName = L"JohnBulletExplosion";
		AddFrame(fTimeDelta, 5.f, 9, false);
		Explosion(fTimeDelta);
		break;
	default:
		break;
	}
}

void CJohnMine::Move(const _float& fTimeDelta)
{
	m_fChaseTime += fTimeDelta;

	_uint random = rand() % 50;
	if (m_fChaseTime > random)
	{
		m_eCurState = EXPLOSION_READY;
		m_fChaseTime = 0.f;
		return;
	}
	m_pTransformCom->Move_Pos(&m_vDir, 5.f, fTimeDelta);
}

void CJohnMine::StopReady(const _float& fTimeDelta)
{
	m_fExplosionReadyTime += fTimeDelta;
	if (m_fExplosionReadyTime > 3.f)
	{
		//m_eCurState = CJohnMine::STATE::RETURN;
		m_fExplosionReadyTime = 0.f;
		_float fWidth = 28.f;
		_float fHeight = 17.f;
		_float fAspect = fWidth + fHeight;
		fAspect /= 2.f;

		_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
		vScale *= 0.5f;
		m_pTransformCom->Multiply_Scale(&vScale);
		//---------------------------------------------------------------
		m_eCurState = EXPLOSION;
		//---------------------------------------------------------------
		fWidth = 51.f;
		fHeight = 51.f;
		fAspect = fWidth + fHeight;
		fAspect /= 2.f;

		vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
		vScale *= 5.f;
		m_pTransformCom->Multiply_Scale(&vScale);
		return;
	}

}


void CJohnMine::Explosion(const _float& fTimeDelta)
{
	m_fExplosionTime += fTimeDelta;
	if (m_fExplosionTime > 1.f)
	{
		m_bDead = true;
		return;
	}

}


CJohnMine* CJohnMine::Create(_vec3 vOrigin, _vec3 vDir)
{
	CJohnMine* pBullet = new CJohnMine(vOrigin, vDir);

	if (FAILED(pBullet->Ready_GameObject()))
	{
		Safe_Release(pBullet);
		MSG_BOX("DiveDaveBullet Create Failed");
		return nullptr;
	}
	return pBullet;
}

void CJohnMine::Free()
{
	Safe_Release(m_pAABB);
	CGameObject::Free();
}


void CJohnMine::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop)
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
