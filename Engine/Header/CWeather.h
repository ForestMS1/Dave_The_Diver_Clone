#pragma once

#include "PSystem.h"
#include "CCamera.h"
BEGIN(Engine)

class ENGINE_DLL CWeather : public PSystem
{
public:
	explicit CWeather(int numParticles);
	//explicit CWeather(LPDIRECT3DDEVICE9	pGraphicDev);
	//explicit CWeather(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 origin);
	virtual ~CWeather();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CWeather* Create(int numParticles);

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();
	void Set_Spawning(bool flag) { Spawning = flag; }
	bool Get_Spawning() {return  Spawning; }
private:
	XMFLOAT3 roomSize;
	bool Spawning;
};

END