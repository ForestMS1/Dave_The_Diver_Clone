#include "CMapTerrainTex.h"

CMapTerrainTex::CMapTerrainTex() : m_pPos(nullptr)
{
}

CMapTerrainTex::CMapTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_hFile(nullptr), m_pPos(nullptr)
{
}

CMapTerrainTex::CMapTerrainTex(const CMapTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_pPos(rhs.m_pPos)
{
}

CMapTerrainTex::~CMapTerrainTex()
{
}

HRESULT CMapTerrainTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
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

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = { _float(j * dwVtxItv),
											0.f,
											_float(i * dwVtxItv) };

			m_pPos[dwIndex] = pVertex[dwIndex].vPosition;

			pVertex[dwIndex].vTexUV = { (_float(j) / (dwCntX - 1)) * 20.f ,
										   (_float(i) / (dwCntZ - 1)) * 20.f };
		}
	}

	m_pVB->Unlock();


	CloseHandle(m_hFile);

	INDEX32* pIndex = nullptr;

	_ulong dwTriCnt = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			dwTriCnt++;

			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			dwTriCnt++;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CMapTerrainTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CMapTerrainTex* CMapTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CMapTerrainTex* pMapTerrainTex = new CMapTerrainTex(pGraphicDev);

	if (FAILED(pMapTerrainTex->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pMapTerrainTex);
		MSG_BOX("pTerrainTex Create Failed");
		return nullptr;
	}

	return pMapTerrainTex;
}

CComponent* CMapTerrainTex::Clone()
{
	return new CMapTerrainTex(*this);
}

void CMapTerrainTex::Free()
{
	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);

	CVIBuffer::Free();
}
