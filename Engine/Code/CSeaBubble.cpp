#include "CSeaBubble.h"
#include "CParticleMgr.h"

CSeaBubble::CSeaBubble(CGameObject* _Player)
{
	m_pPlayer = _Player;
	_size = { 0.05f,0.05f,0.05f };
	_origin = { 0,0,0 };
	m_bRender = false;
}

CSeaBubble::~CSeaBubble()
{
}

HRESULT CSeaBubble::Ready_Buffer()
{
	if (FAILED(Ready_Texture(L"Tex_SeaBubble", L"../Bin/Resource/Texture/Particle/SeaBubble", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;

	for (int i = 0; i < 70; i++)
		addParticle(_origin, { 1,1,1,1 });

	return S_OK;
}


CSeaBubble* CSeaBubble::Create(CGameObject* camera)
{
	CSeaBubble* m_pSeaBubble = new CSeaBubble(camera);

	if (FAILED(m_pSeaBubble->Ready_Buffer()))
	{
		Safe_Release(m_pSeaBubble);
		MSG_BOX("m_pSeaBubble Create Failed");
		return nullptr;
	}
	return m_pSeaBubble;
}

void CSeaBubble::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	_vec3 playerPos{};
	if (m_pPlayer != nullptr) {
		dynamic_cast<CTransform*>(m_pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &playerPos);
	}

	_vec3 max = { playerPos.x + 15.f, playerPos.y + 10.f, 0.f };
	_vec3 min = { playerPos.x - 15.f, playerPos.y - 10.f, 0.f };
	attribute->_isAlive = true;
	//attribute->_position = position;
	GetRandomVector(&attribute->_position, &min, &max);
	attribute->_position.z = GetRandomFloat(-2.f, 0.f);

	if (attribute->_position.x <= playerPos.x + 7.f && attribute->_position.x <= playerPos.x - 7.f &&
		attribute->_position.y <= playerPos.y + 5.f && attribute->_position.y <= playerPos.y - 5.f) {
		if (attribute->_position.x - playerPos.x > 0.f && attribute->_position.y - playerPos.y > 0.f) {
			attribute->_position.x += 7.f - attribute->_position.x - playerPos.x;
			attribute->_position.y += 5.f - attribute->_position.y - playerPos.y;
		}
		if(attribute->_position.x - playerPos.x < 0.f && attribute->_position.y - playerPos.y > 0.f) {
			attribute->_position.x += -7.f - attribute->_position.x - playerPos.x;
			attribute->_position.y += 5.f - attribute->_position.y - playerPos.y;
		}

		if (attribute->_position.x - playerPos.x > 0.f && attribute->_position.y - playerPos.y < 0.f) {
			attribute->_position.x += 7.f - attribute->_position.x - playerPos.x;
			attribute->_position.y += -5.f - attribute->_position.y - playerPos.y;
		}
		if (attribute->_position.x - playerPos.x < 0.f && attribute->_position.y - playerPos.y < 0.f) {
			attribute->_position.x += -7.f - attribute->_position.x - playerPos.x;
			attribute->_position.y += -5.f - attribute->_position.y - playerPos.y;
		}
	}
	
	attribute->velocity.x = GetRandomFloat(0.0f, 0.5f) ;
	attribute->velocity.y = GetRandomFloat(0.0f, 0.5f) ;
	attribute->velocity.z = 0.0f;

	attribute->_age = 0.f;
	attribute->_lifeTime = GetRandomFloat(0.f, 10.f);

	attribute->_type = 0;
	attribute->_color = color;

}

void CSeaBubble::render()
{

		Set_Texture(L"Tex_SeaBubble", 0);
		preRender();
		__super::render();
		postRender();
	

}
void CSeaBubble::preRender()
{
	__super::preRender();
/*	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE)*/;
	
}

void CSeaBubble::postRender()
{
	__super::postRender();

}

void CSeaBubble::update(float fTimeDelta)
{
	if (true) {
		list<Attribute>::iterator i;
		for (i = _particles.begin(); i != _particles.end(); i++) {
			i->_position += i->velocity * fTimeDelta;

			if (i->_color.r <= 0.9f) {
				i->_color += D3DXCOLOR{ 0.1f,0.1f,0.1f,0.f };
			}
		

			_vec3 playerPos{};
			if (m_pPlayer != nullptr) {
				dynamic_cast<CTransform*>(m_pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &playerPos);
			}
			if (!CheckPlayer(playerPos, i->_position)) {
				resetParticle(&(*i), D3DXCOLOR{0.f,0.f,0.f,1.f});
			}
		}
		removeDeadParticles();
	}
}

void CSeaBubble::Free()
{
	PSystem::Free();
}

HRESULT CSeaBubble::CheckPlayer(_vec3 PlayerPos, _vec3 ParticlePos) {
	if ((ParticlePos.x > PlayerPos.x + 20.f || ParticlePos.x < PlayerPos.x - 20.f) ||
		(ParticlePos.y > PlayerPos.y + 15.f || ParticlePos.y < PlayerPos.y - 15.f)) {
		return 0;
	}
	else {
		return 1;
	}
}
