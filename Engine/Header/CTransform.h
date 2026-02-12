#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
	{
		m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
	}

	void		Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eType) += fAngle;
	}

	void		Get_Rotation(_vec3 * pAngle)
	{
		*pAngle = m_vAngle;
	}
	void		Get_RotationRadian(_vec3* pAngle)
	{
		(*pAngle).x = D3DXToRadian(m_vAngle.x);
		(*pAngle).y = D3DXToRadian(m_vAngle.y);
		(*pAngle).z = D3DXToRadian(m_vAngle.z);
		//memcpy(pAngle, m_vAngle, sizeof(_vec3));
	}

	_matrix*			Get_World() { return &m_matWorld;  }
 
	void				Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}

	void			Set_Pos(_float fX, _float fY, _float fZ)
	{
		m_vInfo[INFO_POS] = { fX ,fY, fZ };
	}

	void			Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

	_matrix* Compute_LookAtTarget(const _vec3* pTargetPos);

	void			Set_World(_matrix* pWorld)
	{
		m_matWorld = *pWorld;
	}

	

	void Set_Scale(_vec3 const* vScale)
	{
		m_vScale = *vScale;
	}
	void Get_Scale(_vec3* vScale)
	{
		*vScale = m_vScale;
	}


public:
	HRESULT		Ready_Transform();
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component();

public:
	_vec3		m_vInfo[INFO_END];
	_vec3		m_vScale;
	_vec3		m_vAngle;

	_matrix		m_matWorld;

public:
	static CTransform* Create();
	virtual CComponent* Clone();

private:
	virtual void	Free();
};

END