#include "CLay.h"

#include "CGraphicDev.h"

CLay::CLay() : CGameObject()
{
}

CLay::CLay(const CLay& rhs)
    : CGameObject(rhs)
{
}

CLay::~CLay()
{
}

HRESULT CLay::Ready_GameObject()
{


    return S_OK;
}

_int CLay::Update_GameObject(const _float& fTimeDelta)
{



    return 0;
}

void CLay::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CLay::Free()
{
    CGameObject::Free();
}

_vec3 CLay::Map_Picking(HWND hWnd) {
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 -> 뷰 스페이스

	D3DXMATRIX		matProj;
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드

	D3DXMATRIX		matView;
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3	vRayPos{ 0.f, 0.f, 0.f };
	_vec3	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);


	return _vec3{};

}