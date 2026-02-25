#include "CRcDynamicTex.h"
#include "CGraphicDev.h"

CRcDynamicTex::CRcDynamicTex()
{
}

CRcDynamicTex::CRcDynamicTex(const CRcDynamicTex& rhs)
	: CVIBuffer(rhs)
{
}

CRcDynamicTex::~CRcDynamicTex()
{
}

HRESULT CRcDynamicTex::Ready_Buffer()
{

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (FAILED(pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,	// 버텍스 버퍼의 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// 0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC인 경우 동적 버퍼
		m_dwFVF,	// 버텍스 속성
		D3DPOOL_DEFAULT,	// 정적 버퍼인 경우 MANAGED
		&m_pVB,		// 버텍스 버퍼 객체 생성
		NULL)))		// 공유할 일 없기 때문에 NULL
		return E_FAIL;


	if (FAILED(pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize,	// 인덱스 버퍼의 크기
		m_dwBufferUsage,			// 0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC인 경우 동적 버퍼
		m_IdxFmt,	// 인덱스 속성
		m_dwBufferPool,	// 정적 버퍼인 경우 MANAGED
		&m_pIB,		// 인덱스 버퍼 객체 생성
		NULL)))		// 공유할 일 없기 때문에 NULL
		return E_FAIL;

	//if (FAILED(CVIBuffer::Ready_Buffer()))
	//	return E_FAIL;

	VTXTEX* pVertex = NULL;

	// &pVertex : 버텍스버퍼에 보관된 버텍스 중 첫 번째 버텍스의 주소를 얻어옴

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
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

void CRcDynamicTex::Update_Gauge(const _float& fRatio)
{
	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);

	float bottom = -1.f;
	float newTop = -1.f + (2.f * fRatio);


	pVertex[0].vPosition = { -1.f, newTop, 0.f };
	pVertex[1].vPosition = { 1.f, newTop, 0.f };
	pVertex[2].vPosition = { 1.f, bottom, 0.f };
	pVertex[3].vPosition = { -1.f, bottom, 0.f };

	float vMin = 1.f - fRatio;
	float vMax = 1.f;


	pVertex[0].vTexUV = { 0.f, vMin };
	pVertex[1].vTexUV = { 1.f, vMin };
	pVertex[2].vTexUV = { 1.f, vMax };
	pVertex[3].vTexUV = { 0.f, vMax };

	m_pVB->Unlock();
}

void CRcDynamicTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CRcDynamicTex* CRcDynamicTex::Create()
{
	CRcDynamicTex* pRcTex = new CRcDynamicTex;

	if (FAILED(pRcTex->Ready_Buffer()))
	{
		Safe_Release(pRcTex);
		MSG_BOX("pRcTex Create Failed");
		return nullptr;
	}

	return pRcTex;
}

CComponent* CRcDynamicTex::Clone()
{
	return new CRcDynamicTex(*this);
}

void CRcDynamicTex::Free()
{
	CVIBuffer::Free();
}
