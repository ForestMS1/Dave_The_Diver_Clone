
#pragma once

#include "PSystem.h"
#include "CGameObject.h"
#include "CTransform.h"
BEGIN(Engine)

class ENGINE_DLL CBlood : public PSystem
{
public:
	explicit CBlood();
	virtual ~CBlood();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CBlood* Create();

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();


};

END