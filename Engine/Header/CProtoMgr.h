#pragma once
#include "CBase.h"
#include "CTriCol.h"
#include "CRcCol.h"
#include "CRcTex.h"
#include "CTerrainTex.h"
#include "CCubeCol.h"
#include "CCubeTex.h"

#include "CTransform.h"
#include "CTexture.h"
#include "CCalculator.h"
#include "CDynamicBuffer.h"
#include "CTextureFrame.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	CProtoMgr();
	virtual ~CProtoMgr();

public:
	HRESULT		Ready_Prototype(std::wstring_view svComponentTag, CComponent* pComponent);
	CComponent* Clone_Prototype(std::wstring_view svComponentTag);

private:
	CComponent* Find_Prototype(std::wstring_view svComponentTag);

private:
	map<const std::wstring, CComponent*>		m_mapPrototype;

private:
	virtual void	Free();

};

END