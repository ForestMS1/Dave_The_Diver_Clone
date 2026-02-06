#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "PSystem.h"
#include "CCamera.h"
BEGIN(Engine)

class ENGINE_DLL CParticleMgr : public CBase
{
	DECLARE_SINGLETON(CParticleMgr)


private:
	explicit CParticleMgr();
	virtual ~CParticleMgr();

public:
	HRESULT Ready_Particle(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev);
	void Update_Particle(float fTimeDelta);
	void Render_Particle(LPDIRECT3DDEVICE9 pGraphicDev);
	void spwan_Particle(LPDIRECT3DDEVICE9 pGraphicDev, PARTICLETYPE type, _vec3 origin, int numofPariticles);
	void Set_Camera(CCamera* camera) { m_pCamera = camera; }
private:
	virtual void	Free();
	CCamera* m_pCamera;
	vector <PSystem*> particles;
};

END

