#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicBuffer : public CVIBuffer
{
private:
	explicit CDynamicBuffer(
		_ulong			dwBufferUsage,
		D3DPOOL			dwBufferPool,
		_ulong			dwFVF,
		_ulong			dwVtxSize,
		_ulong          dwIdxSize,
		D3DFORMAT		IdxFmt);
	explicit CDynamicBuffer(const CDynamicBuffer& rhs);
	virtual ~CDynamicBuffer();

public:
	_ulong Get_VertexCnt() const { return m_dwVtxCnt; };
	_ulong Get_TriCnt() const { return m_dwTriCnt ; };
	void Set_VertexCnt(_ulong dwVtxCnt) { m_dwVtxCnt = dwVtxCnt; };
	void Set_TriCnt(_ulong triCnt) { m_dwTriCnt = triCnt; };


public:
	LPDIRECT3DVERTEXBUFFER9 Get_VertexBuffer() const { return m_pVB; }
	LPDIRECT3DINDEXBUFFER9 Get_IndexBuffer() const { return m_pIB; }

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();

public:
	static CDynamicBuffer* Create(
		_ulong			dwBufferUsage,
		D3DPOOL			dwBufferPool,
		_ulong			dwFVF,
		_ulong			dwVtxSize,
		_ulong          dwIdxSize,
		D3DFORMAT		IdxFmt
	);
	virtual CComponent* Clone();
private:
	virtual void	Free();

};

END