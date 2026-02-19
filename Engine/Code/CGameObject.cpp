#include "CGameObject.h"
#include "CLog.h"
#include "CGraphicDev.h"
#include "CComponent.h"
#include "CHelper.h"

CGameObject::CGameObject()
    : m_fViewZ(1.f)
    , m_pParentGameObject(nullptr)
    , m_bDead(false)
{
}

CGameObject::CGameObject(const CGameObject& rhs)
    : m_fViewZ(1.f)
    , m_pParentGameObject(nullptr) // 복사생성시 계층구조는 가져오지 않는다.
    , m_bDead(false)
    
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update_ImGui()
{
    if (ImGui::TreeNode("Components"))
    {
        for (int i = 0; i < ID_END; ++i)
        {
            if (i == ID_DYNAMIC)
            {
                if (ImGui::TreeNode("DYNAMIC"))
                {
                    for (auto& pComPair : m_mapComponent[ID_DYNAMIC])
                    {
                        if (ImGui::TreeNode(CHelper::WStringToString(pComPair.first).c_str()))
                        {
                            pComPair.second->Update_ImGui();

                            ImGui::TreePop();
                        }
                    }

                    ImGui::TreePop();
                }
            }
            else if (i == ID_STATIC)
            {
                if (ImGui::TreeNode("STATIC"))
                {
                    for (auto& pComPair : m_mapComponent[ID_STATIC])
                    {
                        if (ImGui::TreeNode(CHelper::WStringToString(pComPair.first).c_str()))
                        {
                            pComPair.second->Update_ImGui();

                            ImGui::TreePop();
                        }
                    }

                    ImGui::TreePop();
                }
            }
        }

        ImGui::TreePop();
    }
}

CComponent* CGameObject::Get_Component(COMPONENTID eID, std::wstring_view svComponentTag)
{
    CComponent* pComponent = Find_Component(eID, svComponentTag);

    if (nullptr == pComponent)
        return nullptr;

    return pComponent;
}

HRESULT CGameObject::Ready_GameObject()
{
    return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fTimeDelta);

    return 0;
}

void CGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
        pComponent.second->LateUpdate_Component();
}

void CGameObject::Compute_ViewZ(const _vec3* pPos)
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    _matrix matCamWorld;
    pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
    D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

    _vec3   vCamPos;
    memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

    _vec3   vDir = vCamPos - *pPos;

    m_fViewZ = D3DXVec3Length(&vDir);
}

CComponent* CGameObject::Find_Component(COMPONENTID eID, std::wstring_view svComponentTag)
{
    auto        iter = find_if(m_mapComponent[eID].begin(),
                                m_mapComponent[eID].end(), 
                                 CTag_FinderSV(svComponentTag));

    if (iter == m_mapComponent[eID].end())
        return nullptr;

    return iter->second;
}

void CGameObject::Set_Parent(CGameObject* const pNewParent)
{
    if (pNewParent != nullptr)
    {
        // 새부모를 넣어야하는데 기존 부모가 존재한다.
        if (m_pParentGameObject != nullptr)
        {
            // 현재 부모의 자식들중 내자신을 제거(이제 더이상 그사람의 부모가 아니니까)
            EraseChildGameObject(m_pParentGameObject, this);

            // 새부모로 세팅
            m_pParentGameObject = pNewParent;

            // 새부모의 자식에 나를 등록해주기
            m_pParentGameObject->m_childGameObjectList.push_back(this);
        }
        // 새부모를 넣을건데 기존에 부모가 없으니 바로 넣어준다.
        else
        {
            m_pParentGameObject = pNewParent;

            m_pParentGameObject->m_childGameObjectList.push_back(this);
        }
    }
    // 새부모가 널로 들어왔고
    else
    {
        // 현재 부모가 존재한다면
        if (m_pParentGameObject != nullptr)
        {
            // 현재 부모의 자식들중 내자신을 제거(이제 더이상 그사람의 부모가 아니니까)
            EraseChildGameObject(m_pParentGameObject, this);

            // 내 부모는 없다.
            m_pParentGameObject = nullptr;
        }
    }
};

// 파라미터1의 자식들중에 파리미터2에 해당하는 주소 제거
void CGameObject::EraseChildGameObject(CGameObject* pParentGameObject, CGameObject* pDelTarget)
{
    if (!pParentGameObject) return;

    list<CGameObject*>* pChildList = &pParentGameObject->m_childGameObjectList;
    for (auto iter = pChildList->begin(); iter != pChildList->end(); )
    {
        if (*iter == pDelTarget)
        {
            iter = pChildList->erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void CGameObject::Free()
{
    // 사망시 계층 처리
    {
        // 부모가 있다면 부모의 자식중 현재주소와 동일한거 지우주기
        if (Get_Parent())
        {
            EraseChildGameObject(m_pParentGameObject, this);
            m_pParentGameObject = nullptr;
        }

        // 내 자식들의 부모는 없애고 내 자식들 리스트는 제거한다.
        for (auto& pChild : m_childGameObjectList)
        {
            pChild->m_pParentGameObject = nullptr;
        }
        m_childGameObjectList.clear();
    }

    // 사망시 컴포넌트 오브젝트 널처리
    // 아래에서 컴포넌트 어차피 지워질거지만 명시적으로 작성
    {
        for (int i = 0; i < ID_END; ++i)
        {
            for (auto& pComponent : m_mapComponent[i])
            {
                pComponent.second->Set_GameObject(nullptr);
            }
        }
    }
    
    for (_uint i = 0; i < ID_END; ++i)
    {
        for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
        m_mapComponent[i].clear();
    }
}
