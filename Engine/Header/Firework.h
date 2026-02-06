#pragma once

#include "PSystem.h"
BEGIN(Engine)

class ENGINE_DLL Firework : public PSystem
{
public:
	explicit Firework();
	explicit Firework(_vec3 origin, int numParticles); 
	virtual ~Firework();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static Firework* Create(_vec3 origin, int numParticles);
	static Firework* Create();

	void resetParticle(Attribute* attribute);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	int numOfParticles;
	virtual void	Free();


};

END