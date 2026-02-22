#include "CLineBuffer.h"
#include "CGraphicDev.h"

CLineBuffer::CLineBuffer()
{
}

CLineBuffer::CLineBuffer(const CLineBuffer& rhs)
	: CComponent(rhs)
	, m_pLine(rhs.m_pLine)
{
	m_pLine->AddRef();
}

CLineBuffer::~CLineBuffer()
{
}

HRESULT CLineBuffer::Ready_Buffer()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if(FAILED(D3DXCreateLine(pGraphicDev, &m_pLine)))
		return E_FAIL;

	m_pLine->SetWidth(2.f);
	
	return S_OK;
}

void CLineBuffer::Render_Buffer(vector<_vec3>& points, _matrix& matVP)
{
	m_pLine->Begin();

	m_pLine->DrawTransform(points.data(), points.size(), &matVP, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	m_pLine->End();
}

CLineBuffer* CLineBuffer::Create()
{
	CLineBuffer* pLineBuffer = new CLineBuffer;

	if (FAILED(pLineBuffer->Ready_Buffer()))
	{
		Safe_Release(pLineBuffer);
		MSG_BOX("pLineBuffer Create Failed");
		return nullptr;
	}

	return pLineBuffer;
}

CComponent* CLineBuffer::Clone()
{
	return new CLineBuffer(*this);
}

void CLineBuffer::Free()
{
	Safe_Release(m_pLine);
}
