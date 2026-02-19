#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CDiveDaveBullet :
    public CGameObject
{
private:
	explicit CDiveDaveBullet(_vec3 vOrigin, _vec3 vDir, _float fZAngle);
	explicit CDiveDaveBullet(const CDiveDaveBullet& rhs);
	virtual ~CDiveDaveBullet();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	_vec3 m_vOrigin;
	_vec3 m_vDir;
	_float m_fZAngle = 0.f;
	_float m_fLifeTime = 0.f;
	CAABB* m_pAABB;

public:
	static CDiveDaveBullet* Create(_vec3 vOrigin, _vec3 vDir, _float fZAngle);

private:
	virtual void Free() override;
};

