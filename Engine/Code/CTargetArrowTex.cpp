#include "CTargetArrowTex.h"
#include "CGraphicDev.h"

CTargetArrowTex::CTargetArrowTex()
{
}

CTargetArrowTex::CTargetArrowTex(const CTargetArrowTex& rhs)
	: CVIBuffer(rhs)
{
}

CTargetArrowTex::~CTargetArrowTex()
{
}

HRESULT CTargetArrowTex::Ready_Buffer()
{

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* pVertex = NULL;

	// &pVertex : 버텍스버퍼에 보관된 버텍스 중 첫 번째 버텍스의 주소를 얻어옴

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	pVertex[0].vPosition = { 6.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 8.f, 1.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 8.f, -1.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { 6.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CTargetArrowTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CTargetArrowTex* CTargetArrowTex::Create()
{
	CTargetArrowTex* pCTargetArrowTex = new CTargetArrowTex;

	if (FAILED(pCTargetArrowTex->Ready_Buffer()))
	{
		Safe_Release(pCTargetArrowTex);
		MSG_BOX("pCTargetArrowTex Create Failed");
		return nullptr;
	}

	return pCTargetArrowTex;
}

CComponent* CTargetArrowTex::Clone()
{
	return new CTargetArrowTex(*this);
}

void CTargetArrowTex::Free()
{
	CVIBuffer::Free();
}
