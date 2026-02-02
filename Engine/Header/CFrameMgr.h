#pragma once

#include "CBase.h"
#include "CFrame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameMgr : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit		CFrameMgr();
	virtual			~CFrameMgr();

public:
	HRESULT			Ready_Frame(std::wstring_view svFrameTag, const _float& fCallLimit);
	_bool			IsPermit_Call(std::wstring_view svFrameTag, const _float& fTimeDelta);

private:
	CFrame*			Find_Frame(std::wstring_view svFrameTag);

private:
	map<const std::wstring, CFrame*>		m_mapFrame;

public:
	virtual void	Free();
};

END