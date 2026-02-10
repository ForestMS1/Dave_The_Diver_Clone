#include "CWeather.h"



CWeather::CWeather(int numParticles) : PSystem()
{
	Spawning = false;
	_size = { 0.1f,0.1f,0.1f };
	numOfParticles = numParticles;
	m_vMin = { 0,0,0 };
	m_vMax = { 0,0,0 };
}



CWeather::~CWeather()
{
	//Free();
}

HRESULT CWeather::Ready_Buffer()
{

	if (FAILED(Ready_Texture(L"Tex_Weather", L"../Bin/Resource/Texture/white", 1))) {
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
		Set_Texture(L"Tex_Weather",0);
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
	//attribute->_position = position;
	GetRandomVector(&attribute->_position, &m_vMin ,&m_vMax);
	attribute->_position.y = m_vMax.y;
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
			//max와 min 은 
			//max 와 min 을 정하고  파티클의 포지션이 범위를 벗어나면 리셋
			i->_position += i->velocity * fTimeDelta;
			//FXMVECTOR curpos = { i->_position.x, i->_position.y, i->_position.z };
			if (!Intersect_AABB(m_vMin,m_vMax,i->_position)) {
				resetParticle(&(*i),i->_color);
			}
		}
		removeDeadParticles();
	}

}

void CWeather::reset(_vec3 position, _vec3 center, _vec3 extents, D3DXCOLOR color)
{
	m_vMin = { center.x - extents.x, center.y - extents.y  ,center.z - extents.z };
	m_vMax = { center.x + extents.x, center.y + extents.y  ,center.z + extents.z };
	//m_vCenter = center;

	//m_vExtents = extents;
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		i->_position = position;
		resetParticle(&(*i), color);
	}
}


void CWeather::Free()
{

	PSystem::Free();
}

bool CWeather::Intersect_AABB(_vec3 min, _vec3 max, _vec3 point)
{
	if(min.x < point.x && min.y < point.y && min.z < point.z 
		&& max.x > point.x  && max.y > point.y && max.z > point.z)
		return true;
}
