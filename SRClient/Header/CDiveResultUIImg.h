#pragma once
#include "CGameObject.h"
class CDiveResultUIImg : public CGameObject
{
private:
	explicit CDiveResultUIImg(float fPosX, float fPosY);
	virtual ~CDiveResultUIImg();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void Set_ViewZ(float f) { m_fViewZ = f; }
	void Set_Scale(float f) { m_fScale = f; }

public:
	void Update_ImGui() override;

public:
	void Ready_After_Create();

private:
	HRESULT			Ready_Component();


private:
	float m_fDbgX;
	float m_fDbgY;
	float m_fDbgScale;

private:
	const float m_fPosX;
	const float m_fPosY;

	float m_fScale;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	void Set_AssetName(std::wstring_view svAsset) { m_sAssetName = svAsset; }

private:
	std::wstring m_sAssetName;

public:
	static CDiveResultUIImg* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

