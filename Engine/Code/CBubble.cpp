#include "CBubble.h"
#include "CParticleMgr.h"
#include "CMapMgr.h"

CBubble::CBubble(CGameObject* _Player)
{
	m_pPlayer = _Player;
	_size = { 0.2f,0.2f,0.2f };
	_origin = { 0,0,0 };
}

CBubble::~CBubble()
{
}

HRESULT CBubble::Ready_Buffer()
{


	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;


	return S_OK;
}


CBubble* CBubble::Create(CGameObject* camera)
{
	CBubble* m_pBubble = new CBubble(camera);

	if (FAILED(m_pBubble->Ready_Buffer()))
	{
		Safe_Release(m_pBubble);
		MSG_BOX("m_pBubble Create Failed");
		return nullptr;
	}
	return m_pBubble;
}

void CBubble::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	_vec3 playerPos{};


	if (CMapMgr::GetInstance()->GetScene() != nullptr) {
		CTransform* pDaveTransform = static_cast<CTransform*>(CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave")->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		
		pDaveTransform->Get_Info(INFO_POS, &playerPos);
		
	}


	_size.x = 0.2f;
	playerPos.x += GetRandomFloat(-0.2f, 0.2f);
	playerPos.y += GetRandomFloat(-0.2f, 0.2f);
	//playerPos.z = 0.f;

	attribute->_position = playerPos;
	attribute->_isAlive = true;

	_vec3 min = { -0.5f, -0.5f,0.f };
	_vec3 max = { 0.5f, 0.5f,0.f };

	GetRandomVector(&attribute->velocity, &min, &max);
	attribute->velocity *= 1.f;
	attribute->_color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	attribute->_age = 0.f;
	attribute->_lifeTime = GetRandomFloat(0.f, 2.f);;
}

void CBubble::render()
{
	Set_Texture(L"Tex_Bubble", 0);
	preRender();
	__super::render();
	postRender();
}
void CBubble::preRender()
{
	__super::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CBubble::postRender()
{
	__super::postRender();
	//_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CBubble::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		i->_position += i->velocity * fTimeDelta;
		i->_age += fTimeDelta;
		//_origin = i->_position;
	
		if (i->_age  > (i->_lifeTime)/5) {
			i->_color -= D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.f);
		}
		
		if (i->_age > (i->_lifeTime)) {
			i->_isAlive = false;
		}
	}
	removeDeadParticles();

}

void CBubble::Free()
{
	PSystem::Free();
}
