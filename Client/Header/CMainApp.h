#pragma once 

#include "CBase.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CManagement.h"

#include "CLogo.h"


//namespace Engine
//{
//	class CGraphicDev;
//}

class CMainApp : public CBase
{
	// 생성자, 소멸자
private:
	explicit	CMainApp();
	virtual ~CMainApp();

	// 일반적인 멤버함수
	// public
	// protected
	// private
public:
	HRESULT			Ready_MainApp();
	int				Update_MainApp(const float& fTimeDelta);
	void			LateUpdate_MainApp(const float& fTimeDelta);
	void			Render_MainApp();

private:
	HRESULT			Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	Engine::CGraphicDev* m_pDeviceClass;
	Engine::CManagement* m_pManagement;
	LPDIRECT3DDEVICE9	m_pGraphicDev;

public:
	// 생성 함수
	static CMainApp* Create();

	// 소멸 함수
	virtual void			Free();
};

// 1. 마우스를 이용하여 플레이어를 이동시켜라
// 광선 충돌, ray picking
// D3DXIntersectTri();
