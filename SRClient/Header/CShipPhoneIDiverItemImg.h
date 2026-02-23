#pragma once
#include "CGameObject.h"
class CShipPhoneIDiverItemImg : public CGameObject
{
private:
	explicit CShipPhoneIDiverItemImg(float fPosX, float fPosY);
	virtual ~CShipPhoneIDiverItemImg();

public:
	void Set_AssetName(std::wstring_view sAssetName) { m_sAssetName = sAssetName; }
	void Set_ViewZ(float fViewZ) { m_fViewZ = fViewZ; }
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

	std::wstring m_sAssetName;

public:
	static CShipPhoneIDiverItemImg* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

