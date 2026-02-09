#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CVIBuffer.h"

class ENGINE_DLL CCollider : public CBase
{
protected:
	explicit CCollider(COLLIDERID m_eColliderID);
	virtual ~CCollider();

public:
	void Set_OriginalColor(_ulong color) { m_dwOriginalColor = color; }
	void Set_IntersectColor(_ulong color) { m_dwIntersectColor = color; }
	void Set_CurrentColorIntersect() { m_dwCurrentColor = m_dwIntersectColor; };
	void Set_CurrentColorOriginal() { m_dwCurrentColor = m_dwOriginalColor; };

public:
	void* Get_VoidPtr() const { return m_pVoid; }
	void Set_VoidPtr(void* pVoid) { m_pVoid = pVoid; }
	std::wstring_view Get_Tag() const { return m_sTag; }
	void Set_Tag(std::wstring_view svTag) { m_sTag = std::wstring(svTag); }

public:
	bool Intersect(CCollider* pCollider);
	bool Intersect(_vec3* vPos, _vec3* vDir, float&  pOutDist);
	void Transform(const float& fScale, _vec3* pVecRotate, _vec3* pVecTranslate);
	void Transform(_matrix const * mat);

public:
	virtual void Render();

protected:
	COLLIDERID m_eColliderID;
	std::wstring m_sTag;
	void* m_pVoid;

protected:
	virtual HRESULT Ready_Buffer();

protected:
	bool m_bRenderInitialized;

	_ulong m_dwOriginalColor;
	_ulong m_dwIntersectColor;
	_ulong m_dwCurrentColor;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	_ulong			m_dwVtxSize;
	_ulong			m_dwVtxCnt;
	_ulong			m_dwTriCnt;
	_ulong			m_dwFVF;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	_ulong			m_dwIdxSize;
	D3DFORMAT		m_IdxFmt;

public:
	void Free() override;
};

