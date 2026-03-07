
#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}


class CDeepSeaEffect : public CGameObject
{

private:
	explicit CDeepSeaEffect();
	explicit CDeepSeaEffect(const CGameObject& rhs);
	virtual ~CDeepSeaEffect();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;


public:
	static CDeepSeaEffect* Create(wstring _objectName);

private:
	virtual void Free();



};

