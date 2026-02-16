#include "CAABB.h"
#include "CGraphicDev.h"
CAABB::CAABB(COLLIDERID eColliderID)
	: CCollider(eColliderID)
{

}

CAABB::~CAABB()
{
}

HRESULT CAABB::Ready_AABB(_vec3 const* vCenter, _vec3 const* vExtents)
{
	Set_BoundingBox(vCenter, vExtents);

	m_vRefExtents = *vExtents;

	m_dwOriginalColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	m_dwIntersectColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	
	return S_OK;
}

CAABB* CAABB::Create(_vec3 const* vCenter, _vec3 const* vExtents)
{
	return CAABB::Create(vCenter, vExtents, {});
}

CAABB* CAABB::Create(_vec3 const* vCenter, _vec3 const* vExtents, std::wstring_view svTag, void* pVoid)
{
	CAABB* pAABB = new CAABB(COLL_AABB);
	pAABB->Set_Tag(svTag);
	pAABB->Set_VoidPtr(pVoid);

	if (FAILED(pAABB->Ready_AABB(vCenter, vExtents)))
	{
		Safe_Release(pAABB);
		MSG_BOX("AABB Create Failed");
		return nullptr;
	}

	return pAABB;
}

HRESULT CAABB::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCUBECOL);
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwFVF = FVF_CUBECOL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_dwCurrentColor = m_dwOriginalColor;

	CCollider::Ready_Buffer();


	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	// X+
	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	// X-
	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	// Y+
	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	// Y-
	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// Z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	// Z+
	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// Z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	// Z-
	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CAABB::Render()
{
	if (!m_bRenderInitialized)
	{
		if (FAILED(Ready_Buffer()))
		{
			return;
		}
	}

	XMFLOAT3 corners[8];
	m_BoundingBox.GetCorners(corners);
	
	VTXCUBECOL* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);
	for (int i = 0; i < 8; ++i) {
		_vec3 pos = { corners[i].x, corners[i].y, corners[i].z };
		pVertex[i].vPosition = pos;
		pVertex[i].dwColor = m_dwCurrentColor;
	}
	m_pVB->Unlock();

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	pGraphicDev->SetTexture(0, nullptr);

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 
	pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); 

	pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	pGraphicDev->SetFVF(m_dwFVF);
	pGraphicDev->SetIndices(m_pIB);

	D3DXMATRIX matIden;
	D3DXMatrixIdentity(&matIden);
	pGraphicDev->SetTransform(D3DTS_WORLD, &matIden);

	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	// 상태 복구
	pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CAABB::Free()
{
	CCollider::Free();
}
