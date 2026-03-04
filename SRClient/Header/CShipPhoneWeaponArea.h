#pragma once
#include "CGameObject.h"
class CShipPhoneWeaponArea : public CGameObject
{
private:
	explicit CShipPhoneWeaponArea(_uint iIdx, float fPosX, float fPosY);
	virtual ~CShipPhoneWeaponArea();

public:
	_uint Get_Idx()const { return m_iIdx; }
	void Set_AssetName(std::wstring_view sv) { m_sAssetName = sv; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void	Ready_AfterCreate();


private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	//CShipPhoneIDiverSelectEdge* m_pEdge;
	const float m_fPosX;
	const float m_fPosY;
	_uint m_iIdx;

	std::wstring m_sAssetName;
	
public:
	static CShipPhoneWeaponArea* Create(_uint iIdx, float fPosX, float fPosY);

private:
	virtual void Free();
};

