#pragma once
#include "CGameObject.h"
class CTargetCurveStart : public CGameObject
{
private:
	explicit CTargetCurveStart();
	explicit CTargetCurveStart(const CTargetCurveStart& rhs);
	virtual ~CTargetCurveStart();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();
	void	Set_ParentTransform();
	void	Mouse_Check();


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	_vec3 m_vOffset;

public:
	static CTargetCurveStart* Create();

private:
	virtual void Free() override;
};

