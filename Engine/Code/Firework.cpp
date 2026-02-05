#include "Firework.h"

Firework::Firework()
{
}

Firework::Firework(LPDIRECT3DDEVICE9 pGraphicDev) : PSystem(pGraphicDev), numOfParticles(2000)
{
	
	_origin = { 0,0,0 };
	_size = 0.9f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 64;
	
}

Firework::Firework(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 origin, int numParticles) : PSystem(pGraphicDev)
{
	_device = pGraphicDev;
	_device->AddRef();
	_origin = origin;
	_size = 0.9f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 64;
	numOfParticles = numParticles;
}



Firework::~Firework()
{
	//Free();
}

HRESULT Firework::Ready_Buffer()
{

	if (FAILED(Ready_Texture(TEX_NORMAL, L"../Bin/Resource/Texture/flare.bmp", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;
	for (int i = 0; i < numOfParticles; i++)
		addParticle();

	return S_OK;
}

void Firework::render()
{
	Set_Texture(0);
	preRender();
	PSystem::render();
	postRender();
}

Firework* Firework::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 origin, int numParticles)
{
	Firework* firework = new Firework(pGraphicDev, origin, numParticles);

	if (FAILED(firework->Ready_Buffer()))
	{
		Safe_Release(firework);
		MSG_BOX("firework Create Failed");
		return nullptr;
	}
	return firework;
}


void Firework::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;
	attribute->_position = {0,0,0};
	//attribute->_position = _origin;
	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->velocity, &min, &max);
	//D3DXVec3Normalize(&attribute->velocity, &attribute->velocity);
	attribute->velocity *= 10.f;
	attribute->_color = D3DXCOLOR(GetRandomFloat(0.0f, 1.0f), GetRandomFloat(0.0f, 1.0f), GetRandomFloat(0.0f, 1.0f), 1.0f);
	attribute->_age = 0.0f;
	attribute->_lifeTime = 2.0f;
}

void Firework::preRender()
{
	PSystem::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void Firework::postRender()
{
	PSystem::postRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void Firework::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		if (i->_isAlive) {
			i->_position += i->velocity * fTimeDelta;
			i->_age += fTimeDelta;
			if (i->_age > i->_lifeTime) {
				i->_isAlive = false;
			}
		}
	}
	removeDeadParticles();
	if (_particles.size() == 0) {
		empty = true;
	}
	else {
		empty = false;
	}
}

void Firework::Free()
{
	
	PSystem::Free();
}
