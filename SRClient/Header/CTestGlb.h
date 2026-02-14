#pragma once
#include "CGameObject.h"
#include "CAABB.h"
namespace Engine {
	class CGlbTex;
}
class CTestGlb : public CGameObject
{
private:
	explicit CTestGlb();
	explicit CTestGlb(const CTestGlb& rhs);
	virtual ~CTestGlb();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CGlbTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CTestGlb* Create();

private:
	virtual void Free();
};

