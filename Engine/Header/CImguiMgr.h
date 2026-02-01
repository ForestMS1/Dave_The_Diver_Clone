#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CImguiMgr : public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

private:
	explicit CImguiMgr();
	virtual ~CImguiMgr();

public:
	HRESULT Ready_Imgui(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev);
	void Update_Imgui();
	void Render_Imgui(LPDIRECT3DDEVICE9 pGraphicDev);
	bool WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Before_DeviceReset();
	void After_DeviceReset();

private:
	virtual void	Free();
};

END

