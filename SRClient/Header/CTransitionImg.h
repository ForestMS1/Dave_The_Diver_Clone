#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CTransitionImg : public CGameObject
{
private:
	explicit CTransitionImg(float fPosX, float fPosY);
	virtual ~CTransitionImg();

public:
	void Set_AssetName(std::wstring_view sAssetName) { m_sAssetName = sAssetName; }
	void Set_ViewZ(float fViewZ) { m_fViewZ = fViewZ; }
	void Set_CustomScaleX(float f) { m_fCustomScaleX = f; }
	void Set_CustomScaleY(float f) { m_fCustomScaleY = f; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void Ready_AfterCreate();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;
	float m_fCustomScaleX;
	float m_fCustomScaleY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	std::wstring m_sAssetName;



public:
	void Set_Aabb(CAABB* pAABB) { m_pAABB = pAABB; };
	void Set_ColliderGroup(std::wstring_view svGruop) {
		m_sColliderGroup = svGruop;
	}

private:

	CAABB* m_pAABB;
	std::wstring m_sColliderGroup;


public:
	static CTransitionImg* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

