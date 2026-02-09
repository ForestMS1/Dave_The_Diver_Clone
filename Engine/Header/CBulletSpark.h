#pragma once

#include "PSystem.h"
BEGIN(Engine)

class ENGINE_DLL CBulletSpark : public PSystem
{
public:
	explicit CBulletSpark();
	explicit CBulletSpark(_vec3 origin);
	virtual ~CBulletSpark();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CBulletSpark* Create();

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();



};

END