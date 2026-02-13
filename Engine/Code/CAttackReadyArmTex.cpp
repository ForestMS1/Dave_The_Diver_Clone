#include "CAttackReadyArmTex.h"
#include "CGraphicDev.h"

CAttackReadyArmTex::CAttackReadyArmTex()
{
}

CAttackReadyArmTex::CAttackReadyArmTex(const CAttackReadyArmTex& rhs)
	: CVIBuffer(rhs)
{
}

CAttackReadyArmTex::~CAttackReadyArmTex()
{
}

HRESULT CAttackReadyArmTex::Ready_Buffer()
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


	pVertex[0].vPosition = { -0.2f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 1.8f, 1.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 1.8f, -1.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -0.2f, -1.f, 0.f };
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

void CAttackReadyArmTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CAttackReadyArmTex* CAttackReadyArmTex::Create()
{
	CAttackReadyArmTex* pCAttackReadyArmTex = new CAttackReadyArmTex;

	if (FAILED(pCAttackReadyArmTex->Ready_Buffer()))
	{
		Safe_Release(pCAttackReadyArmTex);
		MSG_BOX("pCAttackReadyArmTex Create Failed");
		return nullptr;
	}

	return pCAttackReadyArmTex;
}

CComponent* CAttackReadyArmTex::Clone()
{
	return new CAttackReadyArmTex(*this);
}

void CAttackReadyArmTex::Free()
{
	CVIBuffer::Free();
}
