#include "CBlood.h"
#include "CParticleMgr.h"

CBlood::CBlood()
{
	_size = { 0.5,0.5,0.5 };
	_origin = { 0,0,0 };
}

CBlood::~CBlood()
{
}

HRESULT CBlood::Ready_Buffer()
{
	if (FAILED(Ready_Texture(L"Tex_Blood", L"../Bin/Resource/Texture/Particle/Blood", 2))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;


	return S_OK;
}


CBlood* CBlood::Create()
{
	CBlood* pBlood = new CBlood();

	if (FAILED(pBlood->Ready_Buffer()))
	{
		Safe_Release(pBlood);
		MSG_BOX("pBlood Create Failed");
		return nullptr;
	}
	return pBlood;
}

void CBlood::resetParticle(Attribute* attribute, D3DXCOLOR color)
{

	_size.x = 0.5f;

	attribute->_position = _hitPosition;
	attribute->_position.z = -1.f;
	attribute->_isAlive = true;

	_vec3 min = { -0.2f, 0.f,0.f };
	_vec3 max = { 0.2f, 0.3f,0.f };

	GetRandomVector(&attribute->velocity, &min, &max);

	attribute->velocity *= 0.3f;
	attribute->_color = {1.f, 1.f, 1.f, 1.f};
	attribute->_age = 0.f;
	attribute->_lifeTime = GetRandomFloat(1.f, 4.f);
}

void CBlood::render()
{
	Set_Texture(L"Tex_Blood", 0);
	preRender();

	__super::render();

	postRender();
}
void CBlood::preRender()
{

	__super::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

void CBlood::postRender()
{
	__super::postRender();
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CBlood::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
	
		i->_position += i->velocity * fTimeDelta;
		i->_age += fTimeDelta;
		//_origin = i->_position;

		if (i->_age < (i->_lifeTime) / 3 * 2) {
			i->_color += D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.f);
			//_size.x += 0.001f;
		}

		if (i->_age > (i->_lifeTime)) {
			i->_isAlive = false;
		}
	}
	removeDeadParticles();

}

void CBlood::Free()
{
	PSystem::Free();
}
