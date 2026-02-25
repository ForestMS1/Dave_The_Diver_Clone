#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}


class CCoral : public CGameObject
{
private:
	explicit CCoral();
	explicit CCoral(const wstring_view _TextureName);
	explicit CCoral(const CGameObject& rhs);
	virtual ~CCoral();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	const wstring_view m_TextureName;

public:
	static CCoral* Create(const wstring_view _TextureName);

private:
	virtual void Free();

};


