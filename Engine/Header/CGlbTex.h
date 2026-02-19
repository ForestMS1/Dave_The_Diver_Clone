
#pragma once
#include "CVIBuffer.h"




BEGIN(Engine)

class ENGINE_DLL CGlbTex : public CVIBuffer
{
private:
	explicit CGlbTex();
	explicit CGlbTex(const CGlbTex& rhs);
	virtual ~CGlbTex();


public:

	virtual		HRESULT		Ready_Buffer(const _ulong& m_dwVtxCnt, const _ulong& m_dwTriCnt,std::vector<VTXTEX>* vertices, INDEX32* Index);
	virtual		void		Render_Buffer(); 
	virtual		void		Render_Buffer(const _ulong& startIndex, const _ulong& TriCount);

public:
	static CGlbTex* Create(const _ulong& _dwVtxCnt, const _ulong& _dwTriCnt, std::vector<VTXTEX>* vertices, INDEX32* Index);
	virtual CComponent* Clone();

private:
	_vec3* m_pPos;

private:
	virtual void	Free();

};

END