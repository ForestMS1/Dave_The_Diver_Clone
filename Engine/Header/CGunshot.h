#pragma once

#include "PSystem.h"
BEGIN(Engine)

class ENGINE_DLL CGunShot : public PSystem
{
public:
	explicit CGunShot();
	explicit CGunShot(_vec3 origin);
	virtual ~CGunShot();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CGunShot* Create();

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();



};

END