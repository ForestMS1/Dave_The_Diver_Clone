#pragma once
#include "CGameObject.h"
#include "CAABB.h"

class CTestDancing : public CGameObject
{
private:
	explicit CTestDancing();
	explicit CTestDancing(const CTestDancing& rhs) = delete;
	virtual ~CTestDancing();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	vector<Engine::CDynamicBuffer*> m_vecBufferCom;
	//Engine::CDynamicBuffer* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;


public:
	static CTestDancing* Create();

private:
	virtual void Free();
};

