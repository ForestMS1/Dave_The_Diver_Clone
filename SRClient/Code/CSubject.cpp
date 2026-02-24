#include "CSubject.h"

CSubject::CSubject()
{
}

CSubject::CSubject(const CSubject& rhs)
	: CGameObject(rhs)
{
}

CSubject::~CSubject()
{
}
