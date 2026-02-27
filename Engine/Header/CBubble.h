
#pragma once

#include "PSystem.h"
#include "CGameObject.h"
#include "CTransform.h"
BEGIN(Engine)

class ENGINE_DLL CBubble : public PSystem
{
public:
	explicit CBubble(CGameObject* _Player);
	virtual ~CBubble();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CBubble* Create(CGameObject* _Player);

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();

private:
	CGameObject* m_pPlayer;
};

END