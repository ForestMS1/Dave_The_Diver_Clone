#include "CGunShot.h"
#include "CParticleMgr.h"



CGunShot::CGunShot() : PSystem()
{
	_origin = { 0,0,0 };
	_size = { 0.2f,0.2f,0.2f };
	numOfParticles = 5;
}

CGunShot::CGunShot(_vec3 origin) : PSystem()
{
	//_origin = { 0,0,0 };
	_origin = origin;
	_size = { 0.2f,0.2f,0.2f };
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 64;
	numOfParticles = 5;
}



CGunShot::~CGunShot()
{
}

HRESULT CGunShot::Ready_Buffer()
{

	if (FAILED(Ready_Texture(TEX_NORMAL, L"../Bin/Resource/Texture/white.png", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;
	for (int i = 0; i < numOfParticles; i++)
		//addParticle();

	return S_OK;
}

void CGunShot::render()
{
	Set_Texture(0);
	preRender();
	PSystem::render();
	postRender();
}

CGunShot* CGunShot::Create()
{
	CGunShot* gunshot = new CGunShot();

	if (FAILED(gunshot->Ready_Buffer()))
	{
		Safe_Release(gunshot);
		MSG_BOX("CGunShot Create Failed");
		return nullptr;
	}
	return gunshot;
}


void CGunShot::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	attribute->_isAlive = true;
	//attribute->_position = _origin;
	attribute->_position = { 0,0,0 };

	_vec3 min = _vec3(-1.0f, -1.0f, 1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->velocity, &min, &max);
	D3DXVec3Normalize(&attribute->velocity, &attribute->velocity);
	attribute->velocity *= 10.f;
	int random = rand() % 4;
	switch (random) {
	case 0:
		attribute->_color = D3DXCOLOR(GetRandomFloat(0.0f, 0.1f), GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.9f, 1.0f), 1.0f);
		break;
	case 1:
		attribute->_color = D3DXCOLOR(GetRandomFloat(0.0f, 0.1f), GetRandomFloat(0.7f, 0.9f), GetRandomFloat(0.9f, 1.0f), 1.0f);
		break;
	default:
		attribute->_color = D3DXCOLOR(GetRandomFloat(1.0f, 1.0f), GetRandomFloat(1.0f, 1.0f), GetRandomFloat(1.0f, 1.0f), 1.0f);
		break;
	}
	attribute->_age = 0.0f;
	attribute->_lifeTime = 0.5f;
}

void CGunShot::preRender()
{
	PSystem::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void CGunShot::postRender()
{
	PSystem::postRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CGunShot::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		if (i->_isAlive) {
			i->_position;
			i->_position += i->velocity * fTimeDelta;
			i->_age += fTimeDelta;
			if (i->_age > i->_lifeTime) {
				i->_isAlive = false;
			}
		}
	}
	removeDeadParticles();

}

void CGunShot::Free()
{
	PSystem::Free();
}


