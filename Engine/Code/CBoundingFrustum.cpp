#include "CBoundingFrustum.h"
#include "CGraphicDev.h"
CBoundingFrustum::CBoundingFrustum(COLLIDERID eColliderID)
	: CCollider(eColliderID)
{

}

CBoundingFrustum::~CBoundingFrustum()
{
}

HRESULT CBoundingFrustum::Ready_Frustum(_matrix const* matProj)
{
	Set_BoundingFrustum(matProj);
	m_RefProjMat = *matProj;
	m_dwOriginalColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	m_dwIntersectColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	return S_OK;
}

CBoundingFrustum* CBoundingFrustum::Create(_matrix const* matProj)
{
	return CBoundingFrustum::Create(matProj, {});
}

CBoundingFrustum* CBoundingFrustum::Create(_matrix const* matProj, std::wstring_view svTag, void* pVoid)
{
	CBoundingFrustum* pFrustum = new CBoundingFrustum(COLL_FRUSTUM);
	pFrustum->Set_Tag(svTag);
	pFrustum->Set_VoidPtr(pVoid);

	if (FAILED(pFrustum->Ready_Frustum(matProj)))
	{
		Safe_Release(pFrustum);
		MSG_BOX("pFrustum Create Failed");
		return nullptr;
	}

	return pFrustum;
}

HRESULT CBoundingFrustum::Ready_Buffer()
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

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

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

void CBoundingFrustum::Render()
{
	if (!m_bRenderInitialized)
	{
		if (FAILED(Ready_Buffer()))
		{
			return;
		}
	}

	XMFLOAT3 corners[8];
	m_BoundingFrustum.GetCorners(corners);

	VTXCUBECOL* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
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

	pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CBoundingFrustum::Free()
{
	CCollider::Free();
}
