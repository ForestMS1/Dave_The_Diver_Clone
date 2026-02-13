#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class ENGINE_DLL CPlayerBullet : public CGameObject
{
private:
	explicit CPlayerBullet(_vec3 vOrigin, _vec3 vDir, _float speed);
	explicit CPlayerBullet(const CPlayerBullet& rhs);
	virtual ~CPlayerBullet();
public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;

private:
	HRESULT	Add_Component();
	void	Set_BillBoard();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	_vec3 m_vOrigin; // 처음 생성 위치
	_vec3 m_vDir; // 총알 진행 방향
	_float m_fSpeed;
	_float m_fLifeTime;
	_float m_fFrame;
public:
	static CPlayerBullet* Create(_vec3 vOrigin, _vec3 vDir, _float speed);
private:
	virtual void Free() override;
};