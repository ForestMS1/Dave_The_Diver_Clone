#pragma once

#include "CBase.h"
#include "CLayer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene();
	virtual ~CScene();

public:

	map<const std::wstring, CLayer*>* Get_Layer() { return &m_mapLayer; };
	CLayer* Get_Layer(std::wstring_view svLayertag);

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene() = 0;

	virtual pair<std::wstring, map<const std::wstring, CLayer*>> Before_SceneChange() { return {}; };
	virtual void After_SceneChange(const pair<std::wstring, map<const std::wstring, CLayer*>>& transfer) {};

protected:
	map<const std::wstring, CLayer*>			m_mapLayer;

protected:
	virtual void			Free();

};

END