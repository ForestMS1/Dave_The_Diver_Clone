#pragma once
#include "CGameObject.h"
#include "COBB.h"
class CShipPhoneApp : public CGameObject
{
private:
	explicit CShipPhoneApp(_uint iAppIdx, _float fPosX, _float fPosY);
	explicit CShipPhoneApp(const CShipPhoneApp& rhs) = delete;
	virtual ~CShipPhoneApp();

public:
	bool Get_Select() const { return m_bSelect; }
	void Set_Select(bool bSelect) { m_bSelect = bSelect; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	COBB* m_pOBB;

	const _uint m_iAppIdx;
	const _float m_fPosX;
	const _float m_fPosY;

	_matrix m_matNewWorld;

	bool m_bSelect;

public:
	static CShipPhoneApp* Create(_uint iAppIdx, _float fPosX, _float fPosY);

private:
	virtual void Free();
};

