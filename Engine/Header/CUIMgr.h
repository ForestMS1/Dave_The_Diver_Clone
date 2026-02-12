#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CUIMgr : public CBase
{
	DECLARE_SINGLETON(CUIMgr)

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	HRESULT Ready();
	void Test();

	void Render();

private:
	LPD3DXSPRITE				m_pSprite;

private:
	virtual void		Free();
};

END