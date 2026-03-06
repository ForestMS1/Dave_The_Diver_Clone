#include "FireworkTail.h"
#include "CGraphicDev.h"


FireworkTail::FireworkTail() : PSystem()
{

	_origin = { 0,0,0 };
	_size = { 0.1f,0.1f,0.1f };


}

FireworkTail::FireworkTail(_vec3 origin) : PSystem()
{
	_origin = origin;
	_size = { 0.1f,0.1f,0.1f };

}



FireworkTail::~FireworkTail()
{
	//Free();
}

HRESULT FireworkTail::Ready_Buffer()
{

	if (FAILED(Ready_Texture(L"Tex_FireworkTail", L"../Bin/Resource/Texture/Particle/flare", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;


	return S_OK;
}

void FireworkTail::render()
{
	Set_Texture(L"Tex_FireworkTail", 0);
	preRender();
	PSystem::render();
	postRender();
}

//FireworkTail* FireworkTail::Create(_vec3 origin, int numParticles)
//{
//	FireworkTail* FireworkTail = new FireworkTail(origin, numParticles);
//
//	if (FAILED(FireworkTail->Ready_Buffer()))
//	{
//		Safe_Release(FireworkTail);
//		MSG_BOX("FireworkTail Create Failed");
//		return nullptr;
//	}
//	return FireworkTail;
//}

FireworkTail* FireworkTail::Create(_vec3 origin)
{
	FireworkTail* firwork = new FireworkTail(origin);

	if (FAILED(firwork->Ready_Buffer()))
	{
		Safe_Release(firwork);
		MSG_BOX("FireworkTail Create Failed");
		return nullptr;
	}
	return firwork;
}


void FireworkTail::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	attribute->_isAlive = true;
	attribute->_position = _hitPosition;
	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->velocity, &min, &max);
	D3DXVec3Normalize(&attribute->velocity, &attribute->velocity);
	attribute->velocity *= 2.f;
	attribute->_color = D3DXCOLOR(GetRandomFloat(0.0f, 1.0f), GetRandomFloat(0.0f, 1.0f), GetRandomFloat(0.0f, 1.0f), 1.0f);
	attribute->_age = 0.0f;

	attribute->_lifeTime = 5.0f;
}

void FireworkTail::preRender()
{
	PSystem::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void FireworkTail::postRender()
{
	PSystem::postRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void FireworkTail::update(float fTimeDelta)
{
	removeDeadParticles();

	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		if (i->_isAlive) {
			i->_age += fTimeDelta;
			if (i->_age < 2.f) {
				i->_position.y += fabsf(/*i->velocity.y **/ fTimeDelta);

			}
			//if (i->_position.y <= (_origin.y + 2.f)) {
			//	i->_position.y += fabsf(/*i->velocity.y **/ 2* fTimeDelta);
			//}
			else {
				if (i->_age < i->_lifeTime) {
					i->_position += i->velocity * fTimeDelta * 0.5f;
				}
				else {
					i->_isAlive = false;
				}
			}
		}
	}

	/*if (flag == true) {
		for (i = _particles.begin(); i != _particles.end(); i++) {
			if (i->_isAlive) {
				
			}
		}
	}*/
}

void FireworkTail::Free()
{

	PSystem::Free();
}
