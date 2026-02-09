#include "CFrustrum.h"
#include "CGraphicDev.h"

CFrustrum::CFrustrum()
{
}

CFrustrum::CFrustrum(const CFrustrum& rhs)
	: CVIBuffer(rhs)
{
}

CFrustrum::~CFrustrum()
{
}

HRESULT CFrustrum::Ready_Buffer()
{
    m_dwVtxSize = sizeof(VTXCOL);
    m_dwVtxCnt = 8;      // Near 4개, Far 4개
    m_dwTriCnt = 12;     // LineList 기준으로 선의 개수
    m_dwFVF = FVF_COL;

    m_dwIdxSize = sizeof(INDEX16);
    m_IdxFmt = D3DFMT_INDEX16;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXCOL* pVertex = NULL;
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    
    // Near
    pVertex[0].vPosition = { -1.f,  1.f, 0.f };
    pVertex[1].vPosition = { 1.f,  1.f, 0.f };
    pVertex[2].vPosition = { 1.f, -1.f, 0.f };
    pVertex[3].vPosition = { -1.f, -1.f, 0.f };
    // Far (디버그용이니까 그냥  z값 좀 작게)
    pVertex[4].vPosition = { -1.f,  1.f, 0.9f };
    pVertex[5].vPosition = { 1.f,  1.f, 0.9f };
    pVertex[6].vPosition = { 1.f, -1.f, 0.9f };
    pVertex[7].vPosition = { -1.f, -1.f, 0.9f };

    for (int i = 0; i < 8; ++i) pVertex[i].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

    m_pVB->Unlock();

    
    WORD* pIndex = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    int idx = 0;
    // Near
    pIndex[idx++] = 0; pIndex[idx++] = 1;  pIndex[idx++] = 1; pIndex[idx++] = 2;
    pIndex[idx++] = 2; pIndex[idx++] = 3;  pIndex[idx++] = 3; pIndex[idx++] = 0;
    // Far
    pIndex[idx++] = 4; pIndex[idx++] = 5;  pIndex[idx++] = 5; pIndex[idx++] = 6;
    pIndex[idx++] = 6; pIndex[idx++] = 7;  pIndex[idx++] = 7; pIndex[idx++] = 4;
    // 옆면 연결선
    pIndex[idx++] = 0; pIndex[idx++] = 4;  pIndex[idx++] = 1; pIndex[idx++] = 5;
    pIndex[idx++] = 2; pIndex[idx++] = 6;  pIndex[idx++] = 3; pIndex[idx++] = 7;

    m_pIB->Unlock();

    return S_OK;
}

void CFrustrum::Render_Buffer(const _matrix& matView, const _matrix& matProj)
{
    // View * Proj의 역행렬
    D3DXMATRIX matInv;
    matInv = matView * matProj;
    D3DXMatrixInverse(&matInv, NULL, &matInv);

    // 로컬의 NDC 좌표를 월드로
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    pGraphicDev->SetTransform(D3DTS_WORLD, &matInv);


    pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
    pGraphicDev->SetIndices(m_pIB);
    pGraphicDev->SetFVF(m_dwFVF);

    pGraphicDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    
    pGraphicDev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

}

CFrustrum* CFrustrum::Create()
{
	CFrustrum* pFrustrum = new CFrustrum;

	if (FAILED(pFrustrum->Ready_Buffer()))
	{
		Safe_Release(pFrustrum);
		MSG_BOX("pFrustrum Create Failed");
		return nullptr;
	}

	return pFrustrum;
}

CComponent* CFrustrum::Clone()
{
	return new CFrustrum(*this);
}

void CFrustrum::Free()
{
	CVIBuffer::Free();
}
