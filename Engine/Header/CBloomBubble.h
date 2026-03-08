#pragma once

#include "PSystem.h"
#include "CGameObject.h"
#include "CTransform.h"

BEGIN(Engine)

class ENGINE_DLL CBloomBubble : public PSystem
{
public:
	explicit CBloomBubble();
	virtual ~CBloomBubble();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CBloomBubble* Create();

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();

	

};

END