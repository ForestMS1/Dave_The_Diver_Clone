#include "CCollider.h"
#include "CAABB.h"
#include "CGraphicDev.h"
#include "COBB.h"

CCollider::CCollider(COLLIDERID eColliderID)
	: m_eColliderID(eColliderID)
	, m_pVoid(nullptr)
	, m_sTag({})
	, m_dwOriginalColor(0)
	, m_dwIntersectColor(0)
	, m_dwCurrentColor(0)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwTriCnt(0)
	, m_dwFVF(0)
	, m_dwIdxSize(0)
	, m_IdxFmt(D3DFMT_INDEX32)
{
}

CCollider::~CCollider()
{

}

bool CCollider::Intersect(CCollider* pCollider)
{
	if (m_eColliderID == COLL_AABB)
	{
		if (pCollider->m_eColliderID == COLL_AABB)
		{
			if (static_cast<CAABB*>(this)->Get_BoundingBox().Intersects(static_cast<CAABB*>(pCollider)->Get_BoundingBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_SPHERE)
		{
			
		}
		else if (pCollider->m_eColliderID == COLL_OBB)
		{
			if (static_cast<CAABB*>(this)->Get_BoundingBox().Intersects(static_cast<COBB*>(pCollider)->Get_BoundingOrientedBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_FRUSTUM)
		{
			

		}
	}
	else if (m_eColliderID == COLL_SPHERE)
	{

	}
	else if (m_eColliderID == COLL_OBB)
	{
		if (pCollider->m_eColliderID == COLL_AABB)
		{
			if (static_cast<COBB*>(this)->Get_BoundingOrientedBox().Intersects(static_cast<CAABB*>(pCollider)->Get_BoundingBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_SPHERE)
		{
		}
		else if (pCollider->m_eColliderID == COLL_OBB)
		{
			if (static_cast<COBB*>(this)->Get_BoundingOrientedBox().Intersects(static_cast<COBB*>(pCollider)->Get_BoundingOrientedBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_FRUSTUM)
		{
		}
	}

	return false;
}

bool CCollider::Intersect(_vec3* vPos, _vec3* vDir, float& pOutDist)
{
	FXMVECTOR vRot = XMVectorSet(vDir->x, vDir->y, vDir->z, 0.0f);
	XMVECTOR vTrans = XMVectorSet(vPos->x, vPos->y, vPos->z, 1.0f);
	FXMVECTOR qRot = XMQuaternionRotationRollPitchYawFromVector(vRot);
	if (m_eColliderID == COLL_AABB)
	{
		if (static_cast<CAABB*>(this)->Get_BoundingBox().Intersects(vRot, vTrans, pOutDist))
		{
			Set_CurrentColorIntersect();
			return true;
		}
	}
	else if (m_eColliderID == COLL_OBB)
	{
		if (static_cast<COBB*>(this)->Get_BoundingOrientedBox().Intersects(vRot, vTrans, pOutDist))
		{
			Set_CurrentColorIntersect();
			return true;
		}
	}

	return false;
}

void CCollider::Transform(const float& fScale, _vec3* pVecRotate, _vec3* pVecTranslate)
{
	
	XMVECTOR vRot = XMVectorSet(pVecRotate->x, pVecRotate->y, pVecRotate->z, 0.0f);
	XMVECTOR vTrans = XMVectorSet(pVecTranslate->x, pVecTranslate->y, pVecTranslate->z, 1.0f);
	XMVECTOR qRot = XMQuaternionRotationRollPitchYawFromVector(vRot);
	if (m_eColliderID == COLL_AABB)
	{
		BoundingBox bb;
		static_cast<CAABB*>(this)->Get_BoundingBox().Transform(bb, fScale, qRot, vTrans);
		static_cast<CAABB*>(this)->Set_BoundingBox(bb);
	}
	else if (m_eColliderID == COLL_OBB)
	{
		BoundingOrientedBox ob;
		static_cast<COBB*>(this)->Get_BoundingOrientedBox().Transform(ob, fScale, qRot, vTrans);
		static_cast<COBB*>(this)->Set_BoundingOrientedBox(ob);
	}
}

void CCollider::Render()
{

}

HRESULT CCollider::Ready_Buffer()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	if (FAILED(pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,	// 버텍스 버퍼의 크기
		0,			// 0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC인 경우 동적 버퍼
		m_dwFVF,	// 버텍스 속성
		D3DPOOL_MANAGED,	// 정적 버퍼인 경우 MANAGED
		&m_pVB,		// 버텍스 버퍼 객체 생성
		NULL)))		// 공유할 일 없기 때문에 NULL
		return E_FAIL;


	if (FAILED(pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize,	// 인덱스 버퍼의 크기
		0,			// 0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC인 경우 동적 버퍼
		m_IdxFmt,	// 인덱스 속성
		D3DPOOL_MANAGED,	// 정적 버퍼인 경우 MANAGED
		&m_pIB,		// 인덱스 버퍼 객체 생성
		NULL)))		// 공유할 일 없기 때문에 NULL
		return E_FAIL;
}

void CCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
