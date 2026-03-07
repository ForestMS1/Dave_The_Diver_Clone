#include "CLight.h"
#include "CGraphicDev.h"
CLight::CLight()
{
	ZeroMemory(&m_tLight, sizeof(D3DLIGHT9));

}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light()
{


	return S_OK;
}


CLight* CLight::Create( const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight* pLight = new CLight();

	if (FAILED(pLight->Ready_Light()))
	{
		Safe_Release(pLight);
		MSG_BOX("Light Create Failed");
		return nullptr;
	}

	return pLight;
}

void CLight::Free()
{
	LPDIRECT3DDEVICE9 m_pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);

	Safe_Release(m_pGraphicDev);
}
