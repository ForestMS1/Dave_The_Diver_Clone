#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTexture :  public CComponent
{
private:
	explicit	CTexture();
	explicit	CTexture(const CTexture& rhs);
	virtual	   ~CTexture();

public:
	HRESULT		Ready_Texture(std::wstring_view svAssetLayerTag);
	void		Set_Texture(const _uint& iIndex = 0);

private:
	std::wstring m_sAssetLayer;

public:

	static CTexture* Create(std::wstring_view svAssetLayerTag);
	virtual CComponent* Clone();

private:
	virtual void		Free();

};

END

