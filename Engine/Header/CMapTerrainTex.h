#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMapTerrainTex : public CVIBuffer
{
private:
	explicit CMapTerrainTex();
	explicit CMapTerrainTex(const CMapTerrainTex& rhs);
	virtual ~CMapTerrainTex();

public:
	const	_vec3* Get_VtxPos() { return m_pPos; }

public:
	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual		void		Render_Buffer();

private:
	HANDLE					m_hFile;
	BITMAPFILEHEADER		m_fH;
	BITMAPINFOHEADER		m_iH;

	_vec3* m_pPos;


public:
	static CMapTerrainTex* Create(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual CComponent* Clone();

private:
	virtual void	Free();

};

END