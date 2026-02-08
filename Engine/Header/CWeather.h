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
	virtual void reset(_vec3 position, _vec3 center, _vec3 extents, D3DXCOLOR color);
	virtual void	Free();
	void Set_Spawning(bool flag) { Spawning = flag; }
	bool Get_Spawning() {return  Spawning; }
	//_vec3 Get_Center() {return  m_vCenter; }
	//_vec3 Get_Extents() {return  m_vExtents; }
	//void Set_Center (_vec3 position) { m_vCenter = position; }
	//void Set_Extents(_vec3 position) { m_vExtents = position; }
private:
	bool Intersect_AABB(_vec3 min, _vec3 max, _vec3 point);
private:
	//_vec3 m_vCenter;
	//_vec3 m_vExtents;
	_vec3 m_vMax;
	_vec3 m_vMin;
	bool Spawning;
};

END