#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CAsset.h"

BEGIN(Engine)

class ENGINE_DLL CAssetMgr : public CBase
{
	DECLARE_SINGLETON(CAssetMgr)
	/*
	로드 에셋
	릴리스 에셋

	에셋 키, 에셋 경로

	GetAsset<Type>(에셋키)
	*/
private:
	explicit CAssetMgr();
	virtual ~CAssetMgr();

public:
	HRESULT AddAsset(std::wstring_view svLayerTag, CAsset* pAsset);
	void LoadAsset();
	void LoadAsset(std::wstring_view svLayerTag);
	void DelAsset(std::wstring_view svLayerTag);
	map<const std::wstring, vector<CAsset*>>* const Get_Asset() { return &m_AssetMap; };
	vector<CAsset*>* const Get_Asset(std::wstring_view svLayerTag) { return Find_AssetLayer(svLayerTag); };

	template<typename T>
	T* const Get_AssetFirst(std::wstring_view svLayerTag) { return dynamic_cast<T*>(Find_AssetLayer(svLayerTag)->at(0)); };

public:
	void Update_ImGui();

private:
	vector<CAsset*>* Find_AssetLayer(std::wstring_view svLayerTag);

private:
	map<const std::wstring, vector<CAsset*>> m_AssetMap;

private:
	virtual void		Free();
};

END
