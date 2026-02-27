#pragma once

#include "PSystem.h"
#include "CGameObject.h"
#include "CTransform.h"
BEGIN(Engine)

class CSeaBubble : public PSystem
{
public:
	explicit CSeaBubble(CGameObject* _Player);
	virtual ~CSeaBubble();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CSeaBubble* Create(CGameObject* _Player);

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();

	HRESULT CheckPlayer(_vec3 PlayerPos, _vec3 ParticlePos);
	
	void Set_bRender(bool _bRender) { m_bRender = _bRender; }

private:
	CGameObject* m_pPlayer;
	_bool		 m_bRender;
};

END