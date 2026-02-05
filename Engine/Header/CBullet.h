#pragma once

#include "PSystem.h"
#include "CCamera.h"
BEGIN(Engine)

class ENGINE_DLL CBullet : public PSystem
{
public:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev, CCamera* camera);
	//explicit CBullet(LPDIRECT3DDEVICE9	pGraphicDev);
	//explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 origin);
	virtual ~CBullet();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		render();

public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev,CCamera* camera);

	void resetParticle(Attribute* attribute);
	void preRender();
	void postRender();
	void update(float fTimeDelta) override;
	virtual void	Free();
	D3DXVECTOR3 corners[8];
	_matrix matWorld, matView, matProj, matFinal;

private:
	CCamera * m_pCamera;
};

END