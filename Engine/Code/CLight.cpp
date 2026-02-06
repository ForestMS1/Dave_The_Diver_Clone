#include "CLight.h"
#include "CGraphicDev.h"
CLight::CLight() : m_iIndex(0)
{
	ZeroMemory(&m_tLight, sizeof(D3DLIGHT9));
}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_tLight, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	CGraphicDev::GetInstance()->Get_GraphicDev()->SetLight(iIndex, pLightInfo);

	CGraphicDev::GetInstance()->Get_GraphicDev()->LightEnable(iIndex, TRUE);


	return S_OK;
}


CLight* CLight::Create(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight* pLight = new CLight();

	if (FAILED(pLight->Ready_Light(pLightInfo, iIndex)))
	{
		Safe_Release(pLight);
		MSG_BOX("Light Create Failed");
		return nullptr;
	}

	return pLight;
}

void CLight::Free()
{

}
