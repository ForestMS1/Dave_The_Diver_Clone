#include "CGlbTex.h"
#include "CGraphicDev.h"

CGlbTex::CGlbTex()
{
}

CGlbTex::CGlbTex(const CGlbTex& rhs)
	: CVIBuffer(rhs), m_pPos(rhs.m_pPos)

{
}

CGlbTex::~CGlbTex()
{
}

HRESULT CGlbTex::Ready_Buffer(const _ulong& _dwVtxCnt, const _ulong& _dwTriCnt, std::vector<VTXTEX>* vertices, INDEX32* Index)
{

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = _dwVtxCnt;
	m_dwTriCnt = _dwTriCnt;
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_pPos = new _vec3[m_dwVtxCnt];

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* pVertex = NULL;

	// &pVertex : 버텍스버퍼에 보관된 버텍스 중 첫 번째 버텍스의 주소를 얻어옴

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong dwIndex = 0;


	for (int i = 0; i < m_dwVtxCnt; ++i) {
		pVertex[i].vPosition =  (*vertices)[i].vPosition;

		m_pPos[i] = pVertex[i].vPosition;

		pVertex[i].vNormal = (*vertices)[i].vNormal;

		pVertex[i].vTexUV = (*vertices)[i].vTexUV;

	}


	INDEX32* pIndex = nullptr;

	_ulong dwTriCnt = 0;

	_vec3	vDst, vSrc, vNormal;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (unsigned i = 0; i <m_dwTriCnt; ++i)
	{
	

		// 오른쪽 위
		pIndex[i]._0 = Index[i]._0;
		pIndex[i]._1 = Index[i]._1;
		pIndex[i]._2 = Index[i]._2;
		/*	vDst = pVertex[pIndex[dwTriCnt]._1].vPosition - pVertex[pIndex[dwTriCnt]._0].vPosition;
			vSrc = pVertex[pIndex[dwTriCnt]._2].vPosition - pVertex[pIndex[dwTriCnt]._1].vPosition;

			D3DXVec3Cross(&vNormal, &vDst, &vSrc);

			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;*/

	}



	for (_uint i = 0; i < m_dwVtxCnt; ++i)
	{
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
	}

	m_pVB->Unlock();
	m_pIB->Unlock();


	return S_OK;
}



void CGlbTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

void CGlbTex::Render_Buffer(const _ulong& startIndex, const _ulong& TriCount)
{
	CVIBuffer::Render_Buffer(startIndex, TriCount);
}




CGlbTex* CGlbTex::Create(const _ulong& _dwVtxCnt, const _ulong& _dwTriCnt, std::vector<VTXTEX>* vertices, INDEX32* Index)
{
	CGlbTex* pGlbTex = new CGlbTex;

	if (FAILED(pGlbTex->Ready_Buffer( _dwVtxCnt,  _dwTriCnt,vertices, Index)))
	{
		Safe_Release(pGlbTex);
		MSG_BOX("pGlbTex Create Failed");
		return nullptr;
	}

	return pGlbTex;
}

CComponent* CGlbTex::Clone()
{
	return new CGlbTex(*this);
}

void CGlbTex::Free()
{
	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);
	CVIBuffer::Free();
}
