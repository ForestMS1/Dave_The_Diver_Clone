#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTextureFrame : public CComponent
{
public:
	typedef struct tagFrameInfo
	{
		std::wstring sFrameName;
		std::wstring sAssetTextureName;
		_int iFrame;
		_int iMaxFrame;
		_float fInterval;
	} FRAME_INFO;
private:
	explicit	CTextureFrame();
	explicit	CTextureFrame(const CTextureFrame& rhs);
	virtual	   ~CTextureFrame();

public:
	_int Update_Component(const _float& fTimeDelta) override;

public:
	bool	RegistTextureFrame(std::wstring_view svFrameName, std::wstring_view svAssetTextureName, _float fInterval, _int iStartFrame = 0);
	bool	RemoveTextureFrame(std::wstring_view svFrameName);
	bool	Set_TextureFrame(std::wstring_view svFrameName);
	void	Set_Play(bool bPlay) { m_bPlay = bPlay; };
	bool	Get_Play() const { return m_bPlay; }
	bool	Get_CurrentInfo(FRAME_INFO& pInfo) const;
	
private:
	unordered_map<std::wstring, FRAME_INFO> m_mapFrameInfo;
	bool m_bPlay;

	wstring m_sCurrFrameName;
	_float m_fAccFrameDelta;

public:
	static CTextureFrame* Create();
	virtual CComponent* Clone();

private:
	virtual void		Free();
};

END