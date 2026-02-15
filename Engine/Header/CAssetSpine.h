#pragma once
#include "CAsset.h"
#include "CAssetTexture.h"

namespace spine
{
	class Atlas;
	class SkeletonData;
	class Skeleton;
	class AnimationStateData;
	class AnimationState;
}


BEGIN(Engine)
class ENGINE_DLL CAssetSpine : public CAsset
{
private:
	explicit CAssetSpine(std::wstring_view m_sAssetPath);
	virtual ~CAssetSpine();

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	void TempUpdate(const float& fTimeDelta);
	void TempRender();
	HRESULT LoadFromFile(std::wstring_view atlasPath, std::wstring_view skeletonPath);

public:
	static CAssetSpine* Create(std::wstring_view m_sAssetPath);

private:
	class CMySpineTextureLoader;
	unique_ptr<CMySpineTextureLoader> m_pTexLoader;
	unique_ptr<spine::Atlas> m_pAtlas;
	unique_ptr<spine::SkeletonData> m_pSkeletonData;
	unique_ptr<spine::Skeleton> m_pSkeleton;
	unique_ptr<spine::AnimationStateData> m_pAniStateData;
	unique_ptr<spine::AnimationState> m_pAniState;

private:
	void Free() override;
};

END