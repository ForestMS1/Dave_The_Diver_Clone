#include "CBullet.h"
#include "CParticleMgr.h"
#include "CLog.h"

CBullet::CBullet( CCamera* camera) :  PSystem()
{
	m_pCamera = camera;
	_size = { 0.5f,0.5f,0.5f };
	_origin = { 0,0,0 };


	//AABB 디버깅용 매트릭스 
	//matView = m_pCamera->Get_ViewMatrix();
	//matProj = m_pCamera->Get_ProjMatrix();
	//matFinal = matWorld * matView * matProj;
}



CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Buffer()
{

	if (FAILED(Ready_Texture(TEX_NORMAL, L"../Bin/Resource/Texture/sprBullet_0.png", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;
	//addParticle();
	

	return S_OK;
}



CBullet* CBullet::Create(CCamera* camera)
{
	CBullet* bullet = new CBullet(camera);

	if (FAILED(bullet->Ready_Buffer()))
	{
		Safe_Release(bullet);
		MSG_BOX("CBullet Create Failed");
		return nullptr;
	}
	return bullet;
}


void CBullet::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	
	attribute->_isAlive = true;
	_vec3 cameraPos = m_pCamera->Get_Pos();
	_vec3 cameraDir =  m_pCamera->Get_At() - cameraPos;
	D3DXVec3Normalize(&cameraDir, &cameraDir);
	attribute->_position = cameraPos + cameraDir* 2.f;

	//attribute->_position.y -= 1.0f;
	attribute->velocity = cameraDir * 20.f;
	attribute->_color = color;
	//attribute->_color = D3DXCOLOR(GetRandomFloat(0.6f, 1.0f), GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.9f, 1.0f), 1.0f);
	attribute->_age = 0.0f;
	attribute->_lifeTime = 0.5f;
}


void CBullet::render()
{

	
	// 실제 총알 렌더링
	Set_Texture(0);
	PSystem::render();

	//ID3DXLine* Line;
	//D3DXCreateLine(_device, &Line);
	//Line->SetWidth(2.0f);
	//Line->SetAntialias(true);

	//// 꼭짓점 배열 (update에서 만든 corners를 render에서도 접근 가능하게 멤버로 두는 게 좋음)
	//D3DXVECTOR3 corners[8] = {
	//	{_boundingBox._min.x, _boundingBox._min.y, _boundingBox._min.z}, // 0
	//	{_boundingBox._max.x, _boundingBox._min.y, _boundingBox._min.z}, // 1
	//	{_boundingBox._max.x, _boundingBox._max.y, _boundingBox._min.z}, // 2
	//	{_boundingBox._min.x, _boundingBox._max.y, _boundingBox._min.z}, // 3
	//	{_boundingBox._min.x, _boundingBox._min.y, _boundingBox._max.z}, // 4
	//	{_boundingBox._max.x, _boundingBox._min.y, _boundingBox._max.z}, // 5
	//	{_boundingBox._max.x, _boundingBox._max.y, _boundingBox._max.z}, // 6
	//	{_boundingBox._min.x, _boundingBox._max.y, _boundingBox._max.z}  // 7
	//};

	//// 박스 Edge 연결 (12개 선분)
	//D3DXVECTOR3 lines[24] = {
	//	corners[0], corners[1], corners[1], corners[2], corners[2], corners[3], corners[3], corners[0], // 앞면
	//	corners[4], corners[5], corners[5], corners[6], corners[6], corners[7], corners[7], corners[4], // 뒷면
	//	corners[0], corners[4], corners[1], corners[5], corners[2], corners[6], corners[3], corners[7]  // 앞-뒤 연결
	//};

	//D3DXMatrixTranslation(&matWorld, _origin.x, _origin.y, _origin.z);
	//matFinal = matWorld * matView * matProj;


	//// 라인 그리기
	//Line->Begin();
	//Line->DrawTransform(lines, 24, &matFinal, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)); // 빨간색 박스
	//Line->End();

	//Line->Release();





}

void CBullet::preRender()
{
	PSystem::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	//_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CBullet::postRender()
{
	//_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	PSystem::postRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);

}
void CBullet::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		i->_position += i->velocity * fTimeDelta;
		i->_age += fTimeDelta;
		//_origin = i->_position;
		
		if (i->_age > i->_lifeTime) {
			CParticleMgr::GetInstance()->spwan_Particle(GUNSHOT, i->_position, 5);
			i->_isAlive = false;
		}
	}
	removeDeadParticles();
	
}

void CBullet::Free()
{
	
	PSystem::Free();
}


