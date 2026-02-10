#pragma once

#include "PSystem.h"
#include "CCamera.h"
BEGIN(Engine)

class ENGINE_DLL CFlameShot : public PSystem
{
public:
	explicit CFlameShot(CCamera* camera);
	virtual ~CFlameShot();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CFlameShot* Create(CCamera* camera);

	void resetParticle(Attribute* attribute, D3DXCOLOR color);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();

private:
	CCamera* m_pCamera;
};

END