#pragma once
#include "CComponent.h"

namespace spine
{
	class Atlas;
	class SkeletonData;
	class Skeleton;
	class AnimationStateData;
	class AnimationState;
}


BEGIN(Engine)

class ENGINE_DLL CSpine : public CComponent
{
protected:
	explicit CSpine();
	explicit CSpine(const CSpine& rhs);
	virtual ~CSpine();

public:
	_int Update_Component(const _float& fTimeDelta) override;

public:
	void Vertex_Buffer_Lock(LPDIRECT3DVERTEXBUFFER9 pVB);
	void Index_Buffer_Lock(LPDIRECT3DINDEXBUFFER9 pIB);

public:
	HRESULT Ready_Spine();
	HRESULT Ready_AfterCreate(
		spine::SkeletonData * pSkeletonData,
		spine::AnimationStateData * pAnimationStateData,
		std::wstring_view svSpineAssetName
	);
	void Set_AniState(std::wstring_view svAniName);
	void Render(CDynamicBuffer* pDynamicBuffer);

public:
	static CSpine* Create();
	virtual CComponent* Clone();

private:
	spine::Skeleton* m_pSkeleton;
	spine::AnimationState* m_pAniState;
	_ulong m_iNumVertex;
	_ulong m_iNumTri;
	std::wstring m_sSpineAssetName;
public:
	virtual		void	Free();
};

END