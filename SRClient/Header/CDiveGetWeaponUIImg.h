#pragma once
#include "CGameObject.h"
class CDiveGetWeaponUIImg : public CGameObject
{
private:
	explicit CDiveGetWeaponUIImg(float fPosX, float fPosY);
	virtual ~CDiveGetWeaponUIImg();

public:
	void Update_ImGui() override;

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
	float m_fDbgX;
	float m_fDbgY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	void Set_AssetName(std::wstring_view svAsset) { m_sAssetName = svAsset; }

private:
	std::wstring m_sAssetName;

public:
	static CDiveGetWeaponUIImg* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

