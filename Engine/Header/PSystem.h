#pragma once
#include "CComponent.h"
//#include "Engine_Define.h"
BEGIN(Engine)

class ENGINE_DLL PSystem :
	public CBase
{
public:


public:
	explicit PSystem();
	explicit PSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~PSystem();

	virtual	HRESULT	 Ready_Buffer();
	virtual void reset();
	virtual void resetParticle(Attribute* attribute) = 0;
	virtual void addParticle();

	virtual void update(float timeDelta) = 0;
	virtual void preRender();
	virtual void render();
	virtual void postRender();
	bool isEmpty();
	bool isDead();
	DWORD FtoDw(float f);
	float GetRandomFloat(float lowBound, float highBound);
	void GetRandomVector(
		D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);

public:
	virtual HRESULT	Ready_Texture(TEXTUREID eID, const _tchar* pPath, const _uint& iCnt);
	virtual void		Set_Texture(const _uint& iIndex = 0);
	virtual void Free();

protected:
	virtual void removeDeadParticles();
public:

	IDirect3DDevice9*					_device;
	D3DXVECTOR3							_origin;
	BoundingBox							_boundingBox;
	IDirect3DVertexBuffer9*				 _vb;
	list<Attribute>						_particles;
	DWORD								_vbSize;
	DWORD								_vbOffset;
	DWORD								_vbBatchSize;
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	float								_emitRate;
	float								_size;
	int									_matParticles;
	int									numOfParticles;
	bool								empty;
};

END