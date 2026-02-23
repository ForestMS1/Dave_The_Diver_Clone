#include "CTextureFrame.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetCubeTexture.h"


CTextureFrame::CTextureFrame()
{
}

CTextureFrame::CTextureFrame(const CTextureFrame& rhs)
	: CComponent(rhs)
{
}

CTextureFrame::~CTextureFrame()
{
}

_int CTextureFrame::Update_Component(const _float& fTimeDelta)
{
	if (m_bPlay)
	{
		auto    iter = find_if(m_mapFrameInfo.begin(), m_mapFrameInfo.end(), CTag_FinderSV(m_sCurrFrameName));

		if (iter != m_mapFrameInfo.end())
		{
			FRAME_INFO& info = iter->second;

			m_fAccFrameDelta += fTimeDelta;

			if (m_fAccFrameDelta > info.fInterval)
			{
				info.iFrame = (info.iFrame + 1) % info.iMaxFrame;
				m_fAccFrameDelta = 0;
			}
		}
	}
	return NOEVENT;
}

bool CTextureFrame::RegistTextureFrame(std::wstring_view svFrameName, std::wstring_view svAssetTextureName, _float fInterval, _int iStartFrame)
{
	FRAME_INFO fInfo;
	fInfo.sFrameName = svFrameName;
	fInfo.sAssetTextureName = svAssetTextureName;
	
	fInfo.fInterval = fInterval;

	if (!CAssetMgr::GetInstance()->Get_AssetFirst<CAssetTexture>(svAssetTextureName))
	{
		return false;
	}

	if (CAssetMgr::GetInstance()->Get_Asset(svAssetTextureName)->empty())
	{
		return false;
	}

	fInfo.iMaxFrame = CAssetMgr::GetInstance()->Get_Asset(svAssetTextureName)->size();

	if (iStartFrame > fInfo.iMaxFrame)
	{
		fInfo.iFrame = 0;
	}
	else
	{
		fInfo.iFrame = iStartFrame;
	}
	

	m_mapFrameInfo.insert({ std::wstring(svFrameName) , fInfo});

	return true;
	
}

bool CTextureFrame::RemoveTextureFrame(std::wstring_view svFrameName)
{
	auto    iter = find_if(m_mapFrameInfo.begin(), m_mapFrameInfo.end(), CTag_FinderSV(svFrameName));

	if (iter == m_mapFrameInfo.end())
		return false;

	m_mapFrameInfo.erase(std::wstring(svFrameName));

	return true;
}

bool CTextureFrame::Set_TextureFrame(std::wstring_view svFrameName)
{
	auto    iter = find_if(m_mapFrameInfo.begin(), m_mapFrameInfo.end(), CTag_FinderSV(svFrameName));

	if (iter == m_mapFrameInfo.end())
		return false;

	m_fAccFrameDelta = 0.f;
	m_sCurrFrameName = svFrameName;
	return true;
}

bool CTextureFrame::Get_CurrentInfo(FRAME_INFO& pInfo) const
{
	auto    iter = find_if(m_mapFrameInfo.begin(), m_mapFrameInfo.end(), CTag_FinderSV(m_sCurrFrameName));

	if (iter == m_mapFrameInfo.end())
	{
		return false;
	}

	pInfo = iter->second;
	return true;
}


CTextureFrame* CTextureFrame::Create()
{
	CTextureFrame* pTexture = new CTextureFrame;


	return pTexture;
}

CComponent* CTextureFrame::Clone()
{
	return new CTextureFrame(*this);
}

void CTextureFrame::Free()
{
	CComponent::Free();
}
