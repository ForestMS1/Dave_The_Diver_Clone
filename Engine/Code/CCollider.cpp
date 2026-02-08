#include "CCollider.h"
#include "CAABB.h"
#include "CGraphicDev.h"
#include "COBB.h"
#include "CBoundingSphere.h"
#include "CBoundingFrustum.h"

CCollider::CCollider(COLLIDERID eColliderID)
	: m_eColliderID(eColliderID)
	, m_pVoid(nullptr)
	, m_sTag({})
	, m_bRenderInitialized(false)
	, m_dwOriginalColor(D3DXCOLOR(0.f, 0.f, 0.f, 1.f))
	, m_dwIntersectColor(D3DXCOLOR(0.f, 0.f, 0.f, 1.f))
	, m_dwCurrentColor(D3DXCOLOR(0.f, 0.f, 0.f, 1.f))
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
			if (static_cast<CAABB*>(this)->Get_BoundingBox().Intersects(static_cast<CBoundingSphere*>(pCollider)->Get_BoundingSphere()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
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
			if (static_cast<CAABB*>(this)->Get_BoundingBox().Intersects(static_cast<CBoundingFrustum*>(pCollider)->Get_BoundingFrustum()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
	}
	else if (m_eColliderID == COLL_SPHERE)
	{
		if (pCollider->m_eColliderID == COLL_AABB)
		{
			if (static_cast<CBoundingSphere*>(this)->Get_BoundingSphere().Intersects(static_cast<CAABB*>(pCollider)->Get_BoundingBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_SPHERE)
		{
			if (static_cast<CBoundingSphere*>(this)->Get_BoundingSphere().Intersects(static_cast<CBoundingSphere*>(pCollider)->Get_BoundingSphere()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_OBB)
		{
			if (static_cast<CBoundingSphere*>(this)->Get_BoundingSphere().Intersects(static_cast<COBB*>(pCollider)->Get_BoundingOrientedBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_FRUSTUM)
		{
			if (static_cast<CBoundingSphere*>(this)->Get_BoundingSphere().Intersects(static_cast<CBoundingFrustum*>(pCollider)->Get_BoundingFrustum()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
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
			if (static_cast<COBB*>(this)->Get_BoundingOrientedBox().Intersects(static_cast<CBoundingSphere*>(pCollider)->Get_BoundingSphere()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
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
			if (static_cast<COBB*>(this)->Get_BoundingOrientedBox().Intersects(static_cast<CBoundingFrustum*>(pCollider)->Get_BoundingFrustum()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
	}
	else if (m_eColliderID == COLL_FRUSTUM)
	{
		if (pCollider->m_eColliderID == COLL_AABB)
		{
			if (static_cast<CBoundingFrustum*>(this)->Get_BoundingFrustum().Intersects(static_cast<CAABB*>(pCollider)->Get_BoundingBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_SPHERE)
		{
			if (static_cast<CBoundingFrustum*>(this)->Get_BoundingFrustum().Intersects(static_cast<CBoundingSphere*>(pCollider)->Get_BoundingSphere()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_OBB)
		{
			if (static_cast<CBoundingFrustum*>(this)->Get_BoundingFrustum().Intersects(static_cast<COBB*>(pCollider)->Get_BoundingOrientedBox()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
		else if (pCollider->m_eColliderID == COLL_FRUSTUM)
		{
			if (static_cast<CBoundingFrustum*>(this)->Get_BoundingFrustum().Intersects(static_cast<CBoundingFrustum*>(pCollider)->Get_BoundingFrustum()))
			{
				Set_CurrentColorIntersect();
				pCollider->Set_CurrentColorIntersect();
				return true;
			}
		}
	}

	return false;
}

bool CCollider::Intersect(_vec3* vPos, _vec3* vDir, float& pOutDist)
{
	XMVECTOR vTrans = XMVectorSet(vPos->x, vPos->y, vPos->z, 1.0f);
	D3DXVec3Normalize(vDir, vDir);
	FXMVECTOR vRot = XMVectorSet(vDir->x, vDir->y, vDir->z, 0.0f);
	if (m_eColliderID == COLL_AABB)
	{
		if (static_cast<CAABB*>(this)->Get_BoundingBox().Intersects(vTrans, vRot, pOutDist))
		{
			Set_CurrentColorIntersect();
			return true;
		}
	}
	else if (m_eColliderID == COLL_OBB)
	{
		if (static_cast<COBB*>(this)->Get_BoundingOrientedBox().Intersects(vTrans, vRot, pOutDist))
		{
			Set_CurrentColorIntersect();
			return true;
		}
	}
	else if (m_eColliderID == COLL_SPHERE)
	{
		if (static_cast<CBoundingSphere*>(this)->Get_BoundingSphere().Intersects(vTrans, vRot, pOutDist))
		{
			Set_CurrentColorIntersect();
			return true;
		}
	}
	else if (m_eColliderID == COLL_FRUSTUM)
	{
		if (static_cast<CBoundingFrustum*>(this)->Get_BoundingFrustum().Intersects(vTrans, vRot, pOutDist))
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
	XMVECTOR qRot = XMQuaternionRotationRollPitchYawFromVector(vRot);
	XMVECTOR vTrans = XMVectorSet(pVecTranslate->x, pVecTranslate->y, pVecTranslate->z, 1.0f);

	if (m_eColliderID == COLL_AABB)
	{
		_vec3 vRefExtent =static_cast<CAABB*>(this)->Get_RefExtents();
		BoundingBox bb;
		bb.Extents.x = vRefExtent.x;
		bb.Extents.y = vRefExtent.y;
		bb.Extents.z = vRefExtent.z;
		bb.Transform(bb, fScale, qRot, vTrans);
		static_cast<CAABB*>(this)->Set_BoundingBox(bb);
	}
	else if (m_eColliderID == COLL_OBB)
	{
		_vec3 vRefExtent = static_cast<COBB*>(this)->Get_RefExtents();
		_vec3 vRefOri = static_cast<COBB*>(this)->Get_RefOri();
		BoundingOrientedBox ob;
		FXMVECTOR refOri = XMVectorSet(vRefOri.x, vRefOri.y, vRefOri.z, 0.0f);
		XMVECTOR refQuat = XMQuaternionRotationRollPitchYawFromVector(refOri);
		XMFLOAT4 quat;
		XMStoreFloat4(&quat, refQuat);
		ob.Extents.x = vRefExtent.x;
		ob.Extents.y = vRefExtent.y;
		ob.Extents.z = vRefExtent.z;
		ob.Orientation.x = quat.x;
		ob.Orientation.y = quat.y;
		ob.Orientation.z = quat.z;
		ob.Orientation.w = quat.w;
		ob.Transform(ob, fScale, qRot, vTrans);
		static_cast<COBB*>(this)->Set_BoundingOrientedBox(ob);
	}
	else if (m_eColliderID == COLL_SPHERE)
	{
		BoundingSphere bs;
		bs.Radius = static_cast<CBoundingSphere*>(this)->Get_RefRadius();
		bs.Transform(bs, fScale, qRot, vTrans);
		static_cast<CBoundingSphere*>(this)->Set_BoundingSphere(bs);
	}
	else if (m_eColliderID == COLL_FRUSTUM)
	{
		_matrix mat = static_cast<CBoundingFrustum*>(this)->Get_RefProj();
		BoundingFrustum bf{ XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&mat)) };
		bf.Transform(bf, fScale, qRot, vTrans);
		static_cast<CBoundingFrustum*>(this)->Set_BoundingFrustum(bf);
	}
}

void CCollider::Transform(_matrix const* mat)
{
	if (m_eColliderID == COLL_AABB)
	{
		XMMATRIX matXM = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(mat));
		_vec3 vRefExtent = static_cast<CAABB*>(this)->Get_RefExtents();
		BoundingBox bb;
		bb.Extents.x = vRefExtent.x;
		bb.Extents.y = vRefExtent.y;
		bb.Extents.z = vRefExtent.z;
		bb.Transform(bb, matXM);
		//static_cast<CAABB*>(this)->Get_BoundingBox().Transform(bb, matXM);
		static_cast<CAABB*>(this)->Set_BoundingBox(bb);
	}
	else if (m_eColliderID == COLL_OBB)
	{
		XMMATRIX matXM = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(mat));
		_vec3 vRefExtent = static_cast<COBB*>(this)->Get_RefExtents();
		_vec3 vRefOri = static_cast<COBB*>(this)->Get_RefOri();
		BoundingOrientedBox ob;
		FXMVECTOR refOri = XMVectorSet(vRefOri.x, vRefOri.y, vRefOri.z, 0.0f);
		XMVECTOR refQuat = XMQuaternionRotationRollPitchYawFromVector(refOri);
		XMFLOAT4 quat;
		XMStoreFloat4(&quat, refQuat);
		ob.Extents.x = vRefExtent.x;
		ob.Extents.y = vRefExtent.y;
		ob.Extents.z = vRefExtent.z;
		ob.Orientation.x = quat.x;
		ob.Orientation.y = quat.y;
		ob.Orientation.z = quat.z;
		ob.Orientation.w = quat.w;
		ob.Transform(ob, matXM);
		static_cast<COBB*>(this)->Set_BoundingOrientedBox(ob);
	}
	else if (m_eColliderID == COLL_SPHERE)
	{
		XMMATRIX matXM = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(mat));
		BoundingSphere bs;
		bs.Radius = static_cast<CBoundingSphere*>(this)->Get_RefRadius();
		bs.Transform(bs, matXM);
		static_cast<CBoundingSphere*>(this)->Set_BoundingSphere(bs);
	}
	else if (m_eColliderID == COLL_FRUSTUM)
	{
		XMMATRIX matXM = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(mat));
		_matrix mat = static_cast<CBoundingFrustum*>(this)->Get_RefProj();
		BoundingFrustum bf{ XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&mat)) };
		bf.Transform(bf, matXM);
		static_cast<CBoundingFrustum*>(this)->Set_BoundingFrustum(bf);
	}
}

void CCollider::Render()
{

}

HRESULT CCollider::Ready_Buffer()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	if (FAILED(pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,	// 버텍스 버퍼의 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// 0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC인 경우 동적 버퍼
		m_dwFVF,	// 버텍스 속성
		D3DPOOL_DEFAULT,	// 정적 버퍼인 경우 MANAGED
		&m_pVB,		// 버텍스 버퍼 객체 생성
		NULL)))		// 공유할 일 없기 때문에 NULL
		return E_FAIL;


	if (FAILED(pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize,	// 인덱스 버퍼의 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// 0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC인 경우 동적 버퍼
		m_IdxFmt,	// 인덱스 속성
		D3DPOOL_DEFAULT,	// 정적 버퍼인 경우 MANAGED
		&m_pIB,		// 인덱스 버퍼 객체 생성
		NULL)))		// 공유할 일 없기 때문에 NULL
		return E_FAIL;

	m_bRenderInitialized = true;

	return S_OK;
}

void CCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
