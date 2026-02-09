#pragma once
#include "CGameObject.h"
#include "CAABB.h"
#include "COBB.h"

class CTestCube : public CGameObject
{
private:
	explicit CTestCube(COLLIDERID eColl);
	explicit CTestCube(const CTestCube& rhs) = delete;
	virtual ~CTestCube();

public:
	virtual			HRESULT		Ready_GameObject(_vec3 const * vPos);
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	CTransform* Get_Transform()
	{
		return m_pTransformCom;
	}

	void Set_Speed(float fSpeed) { m_fSpeed = fSpeed; }

private:
	HRESULT			Add_Component();
private:
	Engine::CCubeCol* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	//CAABB* m_pAABB;
	//COBB* m_pOBB;
	CCollider* m_pCollider;
	float m_fTimer;
	int m_iMoveDir;
	float m_fSpeed;
	COLLIDERID m_eColl;
public:
	static CTestCube* Create(_vec3 const *vPos, COLLIDERID m_eColl);

private:
	virtual void Free();
};

