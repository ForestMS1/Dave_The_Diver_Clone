#include "CBulletSpark.h"
#include "CParticleMgr.h"



CBulletSpark::CBulletSpark() : PSystem()
{
	_origin = { 0,0,0 };
	_size = { 0.2f,0.2f,0.2f };
	numOfParticles = 5;
}

CBulletSpark::CBulletSpark(_vec3 origin) : PSystem()
{
	//_origin = { 0,0,0 };
	_origin = origin;
	_size = { 0.2f,0.2f,0.2f };
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 64;
	numOfParticles = 5;
}



CBulletSpark::~CBulletSpark()
{
}

HRESULT CBulletSpark::Ready_Buffer()
{

	if (FAILED(Ready_Texture(L"Tex_Spark", L"../Bin/Resource/Texture/white", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;
	for (int i = 0; i < numOfParticles; i++)
		//addParticle();

	return S_OK;
}

void CBulletSpark::render()
{
	Set_Texture(L"Tex_Spark",0);
	preRender();
	PSystem::render();
	postRender();
}

CBulletSpark* CBulletSpark::Create()
{
	CBulletSpark* gunshot = new CBulletSpark();

	if (FAILED(gunshot->Ready_Buffer()))
	{
		Safe_Release(gunshot);
		MSG_BOX("CBulletSpark Create Failed");
		return nullptr;
	}
	return gunshot;
}


void CBulletSpark::resetParticle(Attribute* attribute, D3DXCOLOR color)
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

void CBulletSpark::preRender()
{
	PSystem::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void CBulletSpark::postRender()
{
	PSystem::postRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CBulletSpark::update(float fTimeDelta)
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

void CBulletSpark::Free()
{
	PSystem::Free();
}


