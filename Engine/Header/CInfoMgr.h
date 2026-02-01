#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
// TODO: Map으로 전환고민?
class ENGINE_DLL CInfoMgr : public CBase
{
	DECLARE_SINGLETON(CInfoMgr)

private:
	explicit CInfoMgr();
	virtual ~CInfoMgr();

public:
	_int Get_Dpi() const { return m_iDpi; }
	void Set_Dpi(_int iDpi) { m_iDpi = iDpi; }

	_float Get_ScaleFactor() const { return m_fScaleFactor; }
	void Set_ScaleFactor(_float fScaleFactor) { m_fScaleFactor = fScaleFactor; }

	_int Get_WINCX() const { return m_iWINCX; }
	void Set_WINCX(_int iWINCX) { m_iWINCX = iWINCX; }

	_int Get_WINCY() const { return m_iWINCY; }
	void Set_WINCY(_int iWINCY) { m_iWINCY = iWINCY; }

private:
	_int m_iDpi;
	_float m_fScaleFactor;

	_int m_iWINCX;
	_int m_iWINCY;

private:
	virtual void	Free();
};

END