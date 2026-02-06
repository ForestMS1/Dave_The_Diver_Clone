#pragma once

#include "PSystem.h"
BEGIN(Engine)

class ENGINE_DLL Firework : public PSystem
{
public:
	explicit Firework();
	explicit Firework(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit Firework(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 origin, int numParticles); 
	virtual ~Firework();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static Firework* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 origin, int numParticles);

	void resetParticle(Attribute* attribute);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	int numOfParticles;
	virtual void	Free();


};

END