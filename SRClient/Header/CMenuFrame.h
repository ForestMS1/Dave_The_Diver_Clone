#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CMenuFrame : public CGameObject
{
private:
	explicit CMenuFrame();
	explicit CMenuFrame(const CGameObject& rhs);
	virtual ~CMenuFrame();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void						Hide();
	void						Show();
private:
	HRESULT			Ready_Component();
	void			Key_Input();
	void			Move_Frame();
public:

	void			Reset_Frame();
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pMenuTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CMenuFrame* Create();
	vector<CGameObject*> &Get_Buttons() { return addButtons; }
	int curButton;
	bool frameMoved;

private:
	virtual void Free();
	bool buttonCreated;
	vector<CGameObject*> addButtons;

};

