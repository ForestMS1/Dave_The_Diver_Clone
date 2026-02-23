#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class COkayButton : public CGameObject
{
private:
	explicit COkayButton();
	explicit COkayButton(const CGameObject& rhs);
	virtual ~COkayButton();


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	wstring Get_Okay() { return whichOkay; }
	void Set_Okay(wstring purpose) { whichOkay = purpose; }
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pOkayTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB; 
	wstring whichOkay;

public:
	static COkayButton* Create();
private:
	virtual void Free();

};

