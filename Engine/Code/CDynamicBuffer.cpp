#include "CDynamicBuffer.h"
#include "CGraphicDev.h"

CDynamicBuffer::CDynamicBuffer(
	_ulong			dwBufferUsage,
	D3DPOOL			dwBufferPool,
	_ulong			dwFVF,
	_ulong			dwVtxSize,
	_ulong			dwIdxSize,
	D3DFORMAT		IdxFmt)
{
	m_dwBufferUsage = dwBufferUsage;
	m_dwBufferPool = dwBufferPool;
	m_dwFVF = dwFVF;
	m_dwVtxSize = dwVtxSize;
	m_dwIdxSize = dwIdxSize;
	m_IdxFmt = IdxFmt;
}

CDynamicBuffer::CDynamicBuffer(const CDynamicBuffer& rhs)
{
	// 다이나믹 버퍼는 버퍼 공유가아니라
	// 매번 새로만들도록 한다 클론해도
	// m_bClone true 일단 주는데 모르겠다...
	m_bClone = true;
	m_pGameObject = nullptr;

	m_dwBufferUsage = rhs.m_dwBufferUsage;
	m_dwBufferPool = rhs.m_dwBufferPool;
	m_dwFVF = rhs.m_dwFVF;
	m_dwVtxSize = rhs.m_dwVtxSize;
	m_dwVtxCnt = rhs.m_dwVtxCnt;
	m_dwTriCnt = rhs.m_dwTriCnt;
	m_dwIdxSize = rhs.m_dwIdxSize;
	m_IdxFmt = rhs.m_IdxFmt;

	Ready_Buffer();
}

CDynamicBuffer::~CDynamicBuffer()
{
}

HRESULT CDynamicBuffer::Ready_Buffer()
{
	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	return S_OK;
}

void CDynamicBuffer::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CDynamicBuffer* CDynamicBuffer::Create(
	_ulong dwBufferUsage,
	D3DPOOL dwBufferPool,
	_ulong dwFVF,
	_ulong dwVtxSize,
	_ulong  dwIdxSize,
	D3DFORMAT IdxFmt)
{
	CDynamicBuffer* pDyBuf = new CDynamicBuffer{
		dwBufferUsage,
		dwBufferPool,
		dwFVF,
		dwVtxSize,
		dwIdxSize,
		IdxFmt
	};

	//if (FAILED(pDyBuf->Ready_Buffer()))
	//{
	//	Safe_Release(pDyBuf);
	//	MSG_BOX("pDyBuf Create Failed");
	//	return nullptr;
	//}

	return pDyBuf;
}

CComponent* CDynamicBuffer::Clone()
{
	return new CDynamicBuffer(*this);
}

void CDynamicBuffer::Free()
{
	CVIBuffer::Free();
}
