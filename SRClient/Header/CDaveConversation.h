#pragma once
#include "CGameObject.h"
class CDaveConversation : public CGameObject
{
private:
	enum CONVERSATION
	{
		CONV_1,
		CONV_END
	};
private:
	explicit CDaveConversation(float fPosX, float fPosY);
	virtual ~CDaveConversation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void Update_ImGui() override;

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	float m_fDbgX;
	float m_fDbgY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	float m_fTimer;

	CONVERSATION m_eCurrentConversation;
	_uint m_iCurrentConversationIdx;
	std::vector<std::wstring> m_vecConversation[CONV_END];

	_uint m_iCurrentTxtIdx;
	std::wstring m_sCurrentTxt;


public:
	static CDaveConversation* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

