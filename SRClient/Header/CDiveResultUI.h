#pragma once
#include "CGameObject.h"
class CDiveResultUI : public CGameObject
{
private:
	explicit CDiveResultUI(float fPosX, float fPosY);
	virtual ~CDiveResultUI();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	HRESULT Ready_AfterCreate();

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

private:
	bool m_bOpen;
	tweeny::tween<float> m_tweenOpen;
	bool m_bOpenTween;

private:
	std::wstring m_sDiveNo;
	std::wstring m_sTemp;
	std::wstring m_sTime;
	std::wstring m_sDepth;
	std::wstring m_sCaught;
	std::wstring m_sObtained;
	std::wstring m_sBiggestFishName;
	std::wstring m_sBiggestFishSize;
	std::wstring m_sBiggestFishImgAsseName;

public:
	static CDiveResultUI* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

