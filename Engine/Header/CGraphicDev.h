#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit CGraphicDev();
	virtual ~CGraphicDev();

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev() { return m_pGraphicDev; }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd, 
								WINMODE eMode, 
								const _uint& iSizeX,
								const _uint& iSizeY,
								CGraphicDev** ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End();


private:
	LPDIRECT3D9			m_pSDK;			// 그래픽 카드 성능 조사 객체
	LPDIRECT3DDEVICE9	m_pGraphicDev;	// 조사된 정보를 토대로 생성한 그리기 담당 객체

private:
	virtual void Free();
};

END



