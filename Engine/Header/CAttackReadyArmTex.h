#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CAttackReadyArmTex : public CVIBuffer
{
private:
	explicit CAttackReadyArmTex();
	explicit CAttackReadyArmTex(const CAttackReadyArmTex& rhs);
	virtual ~CAttackReadyArmTex();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();

public:
	static CAttackReadyArmTex* Create();
	virtual CComponent* Clone();
private:
	virtual void	Free();

};

END

