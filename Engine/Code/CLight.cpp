#include "CLight.h"
#include "CGraphicDev.h"
CLight::CLight()
{
	ZeroMemory(&m_tLight, sizeof(D3DLIGHT9));

}

CLight::~CLight()
{
}




void CLight::Free()
{
	LPDIRECT3DDEVICE9 m_pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);

	Safe_Release(m_pGraphicDev);
}
