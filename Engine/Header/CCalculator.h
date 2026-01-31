#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator :   public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator();

	_float		Compute_HeightOnTerrain(const _vec3* pPos,
										const _vec3* pTerrainVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ);

public:
	static CCalculator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void Free();


};

END