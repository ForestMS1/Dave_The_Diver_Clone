#pragma once

#include "PSystem.h"
BEGIN(Engine)

class ENGINE_DLL FireworkTail : public PSystem
{
public:
	explicit FireworkTail();
	explicit FireworkTail(_vec3 origin);
	virtual ~FireworkTail();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	//static FireworkTail* Create(_vec3 origin, int numParticles);
	static FireworkTail* Create(_vec3 origin);

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	int numOfParticles;
	virtual void	Free();


};

END