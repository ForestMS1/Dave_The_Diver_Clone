#pragma once
#include "CAsset.h"
#include "Engine_Struct.h"

BEGIN(Engine)
class ENGINE_DLL CAssetGlb : public CAsset
{
private:
	explicit CAssetGlb(std::wstring_view m_sAssetPath);
	virtual ~CAssetGlb();



public:
	static CAssetGlb* Create(std::wstring_view m_sAssetPath);

private:
	// 클라에 assimp를 노출안시키기위한 pimpl 패턴
	struct Impl;
	Impl* m_pImpl;


public:
	_ulong	Get_VtxCnt() { return m_dwVtxCnt; }
	_ulong	Get_TriCnt() { return m_dwTriCnt; }
	vector<VTXTEX>*	Get_vertices() { return &vertices; }
	INDEX32*	Get_Index() { return pIndex; }
	LPDIRECT3DTEXTURE9 Get_Texture() { return tex; }
private:
	_ulong m_dwVtxCnt;
	_ulong m_dwTriCnt;

	std::vector<VTXTEX> vertices;
	INDEX32* pIndex;

	LPDIRECT3DTEXTURE9 tex;
public:

	HRESULT Load() override;
	HRESULT Unload() override;

private:
	void Free() override;
};

END