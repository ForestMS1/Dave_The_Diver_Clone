#pragma once
#include "CGameObject.h"
class CGetItemUIImg : public CGameObject
{
private:
	explicit CGetItemUIImg(float fPosX, float fPosY);
	virtual ~CGetItemUIImg();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void Ready_After_Create();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	void Set_AssetName(std::wstring_view svAsset) { m_sAssetName = svAsset ; }

private:
	std::wstring m_sAssetName;

public:
	static CGetItemUIImg* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

