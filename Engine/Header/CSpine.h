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
	void Update_ImGui() override;

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
	void Set_AniState(std::wstring_view svAniName, bool loop = true);
	void Render_Spine(CDynamicBuffer* pDynamicBuffer);
	bool Get_AniStateProgress(float& fProgress);
	void Set_ColorWhite(bool b) { m_bColorWhite = b; }
	bool Get_ColorWhite() const { return m_bColorWhite; }
	void Set_ColorDarkness(float fDarkness) { m_fDarkness = fDarkness ; }
	float Get_ColorDarkness() const { return m_fDarkness; }

public:
	static CSpine* Create();
	virtual CComponent* Clone();

private:
	spine::Skeleton* m_pSkeleton;
	spine::AnimationState* m_pAniState;
	_ulong m_iNumVertex;
	_ulong m_iNumTri;
	std::wstring m_sSpineAssetName;

	std::wstring m_sAniName;
	float m_fAniProgress;
	float m_fDarkness;
	int m_iSelectAni;
	bool m_bColorWhite;
public:
	virtual		void	Free();
};

END