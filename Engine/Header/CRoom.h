#pragma once
#include "CBase.h"
#include "CScene.h"

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRoom : public CBase
{
protected:
	explicit CRoom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRoom(LPDIRECT3DDEVICE9 pGraphicDev, const CScene*  pScene);
	explicit CRoom(const CRoom& rhs);
	virtual ~CRoom();


public:
	virtual			HRESULT		Ready_GameObject() PURE;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)PURE;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)PURE;
	virtual			void		Render_GameObject()PURE;

protected:

	LPDIRECT3DDEVICE9						m_pGraphicDev;
	const CScene* m_pScene;
protected:
	virtual		void		Free();
};

END