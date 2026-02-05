#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3		vPosition;			
		_ulong		dwColor;
	
	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;	

	struct Particle {
		D3DXVECTOR3 _poisition;
		D3DCOLOR _color;
	};
	const DWORD	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	struct BoundingBox {
		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;

		bool isPointInside(D3DXVECTOR3& p) {
			//지정된 포인트가 경계 상자 내에 있는가?
			if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
				p.x <= _max.x && p.y <= _max.y && p.z <= _max.z) {
				return true;
			}
			else {
				return false;
			}
		}
	
	};


	struct Attribute {
		D3DXVECTOR3 _position;
		D3DXVECTOR3 velocity;
		D3DXVECTOR3 acceleration;
		float _lifeTime;
		float _age;
		D3DXCOLOR _color;
		D3DXCOLOR _colorFade;
		bool _isAlive;
	};

}


#endif // Engine_Struct_h__
