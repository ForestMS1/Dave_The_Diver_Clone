#pragma once

#include "PSystem.h"
BEGIN(Engine)

class ENGINE_DLL CGunShot : public PSystem
{
public:
	explicit CGunShot();
	explicit CGunShot(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit CGunShot(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 origin);
	virtual ~CGunShot();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CGunShot* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 origin);

	void resetParticle(Attribute* attribute);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();



};

END