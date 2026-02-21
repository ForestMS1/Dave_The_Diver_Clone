#pragma once
#include "CGameObject.h"
class CShipDiverBoxinventoryDesc : public CGameObject
{
private:
	explicit CShipDiverBoxinventoryDesc(float fPosX, float fPosY, std::wstring_view svAssetName);
	virtual ~CShipDiverBoxinventoryDesc();

public:
	void Set_DescTitle(std::wstring_view svTitle) { m_sTitle = svTitle; };
	void Set_DescContent(std::wstring_view svContent) { m_sContent = svContent; }

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
	const std::wstring m_sAssetName;

	std::wstring m_sTitle;
	std::wstring m_sContent;

public:
	static CShipDiverBoxinventoryDesc* Create(float fPosX, float fPosY, std::wstring_view svAssetName);

private:
	virtual void Free();
};

