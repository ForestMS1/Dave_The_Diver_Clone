#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}


class CCoral : public CGameObject
{
public:
	enum CoralRender {CORAL_ON, CORAL_OFF};
private:
	explicit CCoral();
	explicit CCoral(const wstring_view _TextureName, wstring _objectName);
	explicit CCoral(const CGameObject& rhs);
	virtual ~CCoral();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void						ColliderFrustom();
	void						Set_ObjectName(wstring _name) { m_nameCoralObject = _name; }
	void						Set_CoralTerrian(_bool _coralTerrian) { b_CoralTerrian = _coralTerrian; }
private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	const wstring m_TextureName;
	wstring		  m_nameCoralObject;
	CAABB* m_pAABB;
	CoralRender e_Coral{ CORAL_OFF };
	_bool		b_CoralTerrian{ false };
public:
	static CCoral* Create(const wstring_view _TextureName, wstring _objectName);

private:
	virtual void Free();

	

};


