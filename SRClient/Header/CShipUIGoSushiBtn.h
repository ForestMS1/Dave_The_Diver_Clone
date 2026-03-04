#pragma once
#include "CGameObject.h"
class CShipUIGoSushiBtn : public CGameObject
{
private:
	explicit CShipUIGoSushiBtn(float fPosX, float fPosY);
	virtual ~CShipUIGoSushiBtn();

public:
	void SetActive(bool bActive) { m_bActive = bActive; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	bool m_bActive;
	wstring m_sImgName;
	float m_fScaleX;
	float m_fScaleY;
	float m_fActiveScaleX;
	float m_fActiveScaleY;

public:
	static CShipUIGoSushiBtn* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

