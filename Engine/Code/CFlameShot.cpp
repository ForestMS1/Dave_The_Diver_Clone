#include "CFlameShot.h"
#include "CParticleMgr.h"
#include "CLog.h"

CFlameShot::CFlameShot(CCamera* camera) : PSystem()
{
	m_pCamera = camera;
	_size = { 0.1f,0.1f,0.1f };
	_origin = { 0,0,0 };

}



CFlameShot::~CFlameShot()
{
}

HRESULT CFlameShot::Ready_Buffer()
{

	if (FAILED(Ready_Texture(TEX_NORMAL, L"../Bin/Resource/Texture/white.png", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;
	//addParticle();


	return S_OK;
}



CFlameShot* CFlameShot::Create(CCamera* camera)
{
	CFlameShot* bullet = new CFlameShot(camera);

	if (FAILED(bullet->Ready_Buffer()))
	{
		Safe_Release(bullet);
		MSG_BOX("CFlameShot Create Failed");
		return nullptr;
	}
	return bullet;
}


void CFlameShot::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	_vec3 cameraPos = m_pCamera->Get_Pos();

	// 카메라의 월드 좌표계 기준 방향 벡터들
	_vec3 look = m_pCamera->Get_At() - cameraPos;
	D3DXVec3Normalize(&look, &look);

	_vec3 up = m_pCamera->Get_Up();
	_vec3 right;
	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&right, &right);

	//// 총구 오프셋 (카메라 로컬 기준: 앞쪽 1.0, 오른쪽 0.2, 위쪽 -0.1)
	_vec3 gunOffset = look * 1.0f + right * 0.2f + up * -0.1f;

	//// 최종 총구 위치
	attribute->_position = cameraPos + gunOffset;
	attribute->_isAlive = true;
	//_vec3 cameraPos = m_pCamera->Get_Pos();
	/*_vec3 cameraDir = m_pCamera->Get_At() - cameraPos;
	D3DXVec3Normalize(&cameraDir, &cameraDir);
	attribute->_position = cameraPos + cameraDir;*/
	_vec3 min = { -0.2f, -0.2f,0.2f};
	_vec3 max = { 0.2f,0.2f,0.2f };
	max.x = look.x + max.x;
	max.y = look.y + max.y;
	max.z = look.z;
	min.x = look.x + min.x;
	min.y = look.y + min.y;
	min.z = max.z;
	GetRandomVector(&attribute->velocity, &min, &max);
	attribute->velocity *= 10.f;
	attribute->_color = D3DXCOLOR(1.f,1.f,1.f,1.f);
	attribute->_age = 0.0f;
	attribute->_lifeTime = 1.0f;
}


void CFlameShot::render()
{

	Set_Texture(0);
	preRender();
	PSystem::render();
	postRender();

}

void CFlameShot::preRender()
{
	PSystem::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void CFlameShot::postRender()
{
	PSystem::postRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);

}
void CFlameShot::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		i->_position += i->velocity * fTimeDelta;
		i->_age += fTimeDelta;
		//_origin = i->_position;

		if (i->_age > (i->_lifeTime/5.f)) {
			i->_color = D3DXCOLOR((19 / 255.f), (243 / 255.f), (255 / 255.f), 1.f);
		}
		if (i->_age > (i->_lifeTime)) {
			i->_isAlive = false;
		}
	}
	removeDeadParticles();

}

void CFlameShot::Free()
{

	PSystem::Free();
}


