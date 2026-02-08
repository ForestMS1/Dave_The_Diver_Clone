#include "CWeather.h"



CWeather::CWeather(int numParticles) : PSystem()
{
	Spawning = false;
	_size = { 0.1f,0.1f,0.1f };
	numOfParticles = numParticles;
	roomSize = { 10.f,10.f,10.f };
	_boundingBox.Extents = roomSize;
}



CWeather::~CWeather()
{
	//Free();
}

HRESULT CWeather::Ready_Buffer()
{

	if (FAILED(Ready_Texture(TEX_NORMAL, L"../Bin/Resource/Texture/white.png", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;
	for (int i = 0; i < numOfParticles; i++)
		addParticle(_origin,{1,1,1,1});

	return S_OK;
}

void CWeather::render()
{
	if (Spawning) {
		Set_Texture(0);
		preRender();
		PSystem::render();
		postRender();
	}
}

CWeather* CWeather::Create(int numParticles)
{
	CWeather* weather = new CWeather(numParticles);

	if (FAILED(weather->Ready_Buffer()))
	{
		Safe_Release(weather);
		MSG_BOX("CWeather Create Failed");
		return nullptr;
	}
	return weather;
}

//CWeather* CWeather::Create()
//{
//	CWeather* CWeather = new CWeather();
//
//	if (FAILED(CWeather->Ready_Buffer()))
//	{
//		Safe_Release(CWeather);
//		MSG_BOX("CWeather Create Failed");
//		return nullptr;
//	}
//	return CWeather;
//}


void CWeather::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	attribute->_isAlive = true;
	_vec3 min, max;
	min = { _boundingBox.Center.x - roomSize.x,_boundingBox.Center.y - roomSize.y,_boundingBox.Center.z - roomSize.z };
	max = { _boundingBox.Center.x + roomSize.x,_boundingBox.Center.y + roomSize.y,_boundingBox.Center.z + roomSize.z };
	//attribute->_position = position;
	GetRandomVector(&attribute->_position, &min ,&max);
	attribute->_position.y = max.y;
	attribute->velocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->velocity.y = GetRandomFloat(0.0f, 1.0f) * -10.0f;
	attribute->velocity.z = 0.0f;
	attribute->_color = color;
}

void CWeather::preRender()
{
	PSystem::preRender();
	//_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void CWeather::postRender()
{
	PSystem::postRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CWeather::update(float fTimeDelta)
{
	if (Spawning) {
		list<Attribute>::iterator i;
		for (i = _particles.begin(); i != _particles.end(); i++) {
			i->_position += i->velocity * fTimeDelta;
			FXMVECTOR curpos = { i->_position.x, i->_position.y, i->_position.z };
			if (_boundingBox.Contains(curpos) == 0) {
				resetParticle(&(*i),i->_color);
			}
		}
		removeDeadParticles();
	}

}

void CWeather::Free()
{

	PSystem::Free();
}
