#pragma once

#include "CBase.h"
#include "CLayer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent* Get_Component(COMPONENTID eID,
		std::wstring_view svLayerTag,
		std::wstring_view svObjTag,
		std::wstring_view svComponentTag);

	map<const std::wstring, CLayer*>* Get_Layer() { return &m_mapLayer; };

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene() = 0;

protected:
	map<const std::wstring, CLayer*>			m_mapLayer;
	LPDIRECT3DDEVICE9					m_pGraphicDev;

protected:
	virtual void			Free();

};

END