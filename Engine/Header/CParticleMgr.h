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
	HRESULT Ready_Particle(HWND hWnd);
	void Update_Particle(float fTimeDelta);
	void Render_Particle( );
	void PreRender_Particle( );
	void PostRender_Particle( );
	void spwan_Particle(PARTICLETYPE type, _vec3 origin, int numofPariticles);
	void spwan_Weather(WEATHERTYPE type, _vec3 origin, int numofPariticles, D3DXCOLOR color);
	void Set_Camera(CCamera* camera) { m_pCamera = camera; }
	void Set_Player(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
	void Set_ParicleOn(_bool _particle) { m_bRenderOn = _particle; }
	void Clear_Particle();

private:
	virtual void	Free();
	CCamera* m_pCamera;
	CGameObject* m_pPlayer;
	vector <PSystem*> particles;
	vector <PSystem*> preparticles;
	vector <PSystem*> postparticles;
	bool m_bRenderOn{false};
	bool cameraOn;
};

END

