#include "CDiveDaveMeeleAttack.h"
#include "CDiveDave.h"
#include "CHelper.h"
CDiveDaveMeeleAttack::CDiveDaveMeeleAttack(CGameObject* pOwner)
    : CPlayerState(pOwner)
{
}

CDiveDaveMeeleAttack::~CDiveDaveMeeleAttack()
{
}

void CDiveDaveMeeleAttack::Enter()
{
    static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
    _float fWidth = 56;
    _float fHeight = 56.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
    static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_MeleeDaggerAttackTexture");
}

void CDiveDaveMeeleAttack::Input(const _float& fTimeDelta)
{

}

_int CDiveDaveMeeleAttack::Update_State(const _float& fTimeDelta)
{
    Mouse_Check();
    static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 4);

    return 0;
}

void CDiveDaveMeeleAttack::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveMeeleAttack::Render_State()
{
    CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

    _float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

    pPlayerTextureCom->Set_Texture((_uint)fFrame);

    if (fFrame > 3.f)
        static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
}

void CDiveDaveMeeleAttack::Exit()
{
    _float fWidth = 56;
    _float fHeight = 56.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
}

void CDiveDaveMeeleAttack::Clear()
{
}

CDiveDaveMeeleAttack* CDiveDaveMeeleAttack::Create(CGameObject* pOwner)
{
    CDiveDaveMeeleAttack* pState = new CDiveDaveMeeleAttack(pOwner);

    return pState;
}

void CDiveDaveMeeleAttack::Free()
{
}

void CDiveDaveMeeleAttack::Mouse_Check()
{
    _vec3 vMousePos, vPlayerPos;
    CHelper::GetMousePointInWorld(&vMousePos);
    static_cast<CDiveDave*>(m_pPlayer)->Get_Pos(&vPlayerPos);

    if (vMousePos.x <= vPlayerPos.x)
    {
        _vec3 vRotateDir = { 0.f, -180.f, 0.f };
        static_cast<CDiveDave*>(m_pPlayer)->Set_RotateDir(&vRotateDir);
    }
    else
    {
        _vec3 vRotateDir = { 0.f, 0.f, 0.f };
        static_cast<CDiveDave*>(m_pPlayer)->Set_RotateDir(&vRotateDir);
    }
}