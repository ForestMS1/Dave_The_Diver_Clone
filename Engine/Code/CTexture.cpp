#include "CTexture.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetCubeTexture.h"

CTexture::CTexture()
{
}

CTexture::CTexture(const CTexture& rhs)
    : CComponent(rhs)
	, m_sAssetLayer(rhs.m_sAssetLayer)
{
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(std::wstring_view svAssetLayerTag)
{
	m_sAssetLayer = svAssetLayerTag;
	return S_OK;
}

void CTexture::Set_Texture(const _uint& iIndex)
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto pAssTex = dynamic_cast<CAssetTexture*>((*CAssetMgr::GetInstance()->Get_Asset(m_sAssetLayer))[iIndex]))
	{
		pGraphicDev->SetTexture(0, pAssTex->Get_Texture());
	}
	else if (auto pAssTex = dynamic_cast<CAssetCubeTexture*>((*CAssetMgr::GetInstance()->Get_Asset(m_sAssetLayer))[iIndex]))
	{
		pGraphicDev->SetTexture(0, pAssTex->Get_CubeTexture());
	}
}

CTexture* CTexture::Create(std::wstring_view svAssetLayerTag)
{
	CTexture* pTexture = new CTexture;

	if (FAILED(pTexture->Ready_Texture(svAssetLayerTag)))
	{
		Safe_Release(pTexture);
		MSG_BOX("Texture Create Failed");
		return nullptr;
	}

	return pTexture;
}

CComponent* CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	CComponent::Free();

}
