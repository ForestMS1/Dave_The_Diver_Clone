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
	explicit CTestGlb(const wstring_view tex);
	explicit CTestGlb(const CTestGlb& rhs);
	virtual ~CTestGlb();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();
	HRESULT			Ready_Material();
private:
	Engine::CGlbTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	const wstring_view m_wsName;
	vector<CAABB*> m_pAABB;

public:
	static CTestGlb* Create();
	static CTestGlb* Create(const wstring_view tex);

private:
	virtual void Free();
};

