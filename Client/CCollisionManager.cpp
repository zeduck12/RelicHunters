#include "pch.h"
#include "CObj.h"
#include "CCollisionManager.h"
#include "CGrenade.h"
#include "CCasing.h"

DEFINITION_SINGLETON(CCollisionManager)

// 피해자와 총알
bool CCollisionManager::CollideBullet(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	RECT rc = {};
	RECT rcDst = _pDstObj->GetRect();
	RECT rcSrc = _SrcObj->GetRect();

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
	{
		// 데미지 주고 총알은 없어짐.
		_SrcObj->SetIsValid(false);
		return true;
	}

	return false;
}
bool CCollisionManager::CollidePlasma(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	RECT rc = {};
	RECT rcDst = _pDstObj->GetRect();
	RECT rcSrc = _SrcObj->GetRect();

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
		return true;

	return false;
}
bool CCollisionManager::CollideGrenade(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	RECT rc = {};
	RECT rcDst = _pDstObj->GetRect();
	RECT rcSrc = _SrcObj->GetRect();

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
	{
		////_pDstObj->SetIsValid(false);
		//_SrcObj->SetIsValid(false);
		return true;
	}

	return false;
}
bool CCollisionManager::CollideTileBullet(TILE* _pTile, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	RECT rc = {};
	RECT rcDst = 
	{
		LONG(_pTile->vPos.x - (128.f * 0.5f)),
		LONG(_pTile->vPos.y - (128.f * 0.5f)),
		LONG(_pTile->vPos.x + (128.f * 0.5f)),
		LONG(_pTile->vPos.y + (128.f * 0.5f))
	};

	RECT rcSrc = _SrcObj->GetRect();

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
	{
		// 데미지 주고 총알은 없어짐.
		_SrcObj->SetIsValid(false);
		return true;
	}

	return false;
}
bool CCollisionManager::CollideTileCasing(TILE* _pTile, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	CCasing* pCasing = dynamic_cast<CCasing*>(_SrcObj);

	float fTileX = _pTile->vPos.x;
	float fTileY = _pTile->vPos.y;

	RECT rcDst =
	{
		LONG(_pTile->vPos.x - (128.f * 0.5f)),
		LONG(_pTile->vPos.y - (128.f * 0.5f)),
		LONG(_pTile->vPos.x + (128.f * 0.5f)),
		LONG(_pTile->vPos.y + (128.f * 0.5f))
	};
	RECT rcSrc = _SrcObj->GetRect(); // 몬스터
	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		// 반사각으로 팅겨내기
		float fIncidenceDegree = pCasing->GetIncidenceDegree();
		float fReflectDegree = 180.f - fIncidenceDegree;
		pCasing->SetReflectDegree(fReflectDegree);
		pCasing->SetIsCollide(true);
		pCasing->SetGravity(0.f);
		return true;
	}

	return false;
}
bool CCollisionManager::CollideTileGrenade(TILE* _pTile, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	CGrenade* pCasing = dynamic_cast<CGrenade*>(_SrcObj);

	float fTileX = _pTile->vPos.x;
	float fTileY = _pTile->vPos.y;

	RECT rcDst =
	{
		LONG(_pTile->vPos.x - (128.f * 0.5f)),
		LONG(_pTile->vPos.y - (128.f * 0.5f)),
		LONG(_pTile->vPos.x + (128.f * 0.5f)),
		LONG(_pTile->vPos.y + (128.f * 0.5f))
	};
	RECT rcSrc = _SrcObj->GetRect(); // 몬스터
	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		// 반사각으로 팅겨내기
		float fIncidenceDegree = pCasing->GetIncidenceDegree();
		float fReflectDegree = 180.f - fIncidenceDegree;
		//pCasing->SetGravity(0.f);
		return true;
	}
	return false;
}
// 플레이어와 몬스터
bool CCollisionManager::CollidePlayerMonster(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	RECT rcDst = _pDstObj->GetRect(); // 플레이어
	RECT rcSrc = _SrcObj->GetRect(); // 몬스터
	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		// 충돌했다면 player 밀쳐내기
		int iVertical = rc.bottom - rc.top;
		int iHorizontal = rc.right - rc.left;

		if (iHorizontal > iVertical)
		{
			if (_pDstObj->GetY() < _SrcObj->GetY())
				_pDstObj->SetY(_pDstObj->GetY() - iVertical - 50.f);
			else if (_pDstObj->GetY() > _SrcObj->GetY())
				_pDstObj->SetY(_pDstObj->GetY() + iVertical + 50.f);
		}
		else
		{
			if (_pDstObj->GetX() > _SrcObj->GetX())
				_pDstObj->SetX(_pDstObj->GetX() + iHorizontal * 1.4f);
			else if (_pDstObj->GetX() < _SrcObj->GetX())
				_pDstObj->SetX(_pDstObj->GetX() - iHorizontal * 1.4f);
		}

		return true;
	}

	return false;
}
// 벽과 수류탄
bool CCollisionManager::CollideWallGrenade(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	RECT rc = {};
	RECT rcDst = _pDstObj->GetRect(); // 벽
	RECT rcSrc = _SrcObj->GetRect();  // 수류탄

	float fIncidenceDegree = 0.f;
	float fReflectDegree = 0.f;

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
	{
		// 벽에 부딫힌 수류탄 반사각으로 팅겨내기.
		// 수류탄이 벽의 좌측에 부딫힐 때
		if(_SrcObj->GetX() < _pDstObj->GetX() && _SrcObj->GetY() > _pDstObj->GetTop() && _SrcObj->GetY() < _pDstObj->GetBottom())
		{
			fIncidenceDegree = dynamic_cast<CGrenade*>(_SrcObj)->GetIncidenceDegree();
			fReflectDegree = 180.f - fIncidenceDegree;
		
			dynamic_cast<CGrenade*>(_SrcObj)->SetIsCollide(true);
		}
		// 수류탄이 벽의 우측에 부딫힐 때
		if (_SrcObj->GetX() > _pDstObj->GetX() && _SrcObj->GetY() > _pDstObj->GetTop() && _SrcObj->GetY() < _pDstObj->GetBottom())
		{
			fIncidenceDegree = dynamic_cast<CGrenade*>(_SrcObj)->GetIncidenceDegree();
			fReflectDegree = 180.f - fIncidenceDegree;
			
			dynamic_cast<CGrenade*>(_SrcObj)->SetIsCollide(true);
		}
		// 수류탄이 벽의 상단에 부딫힐 때
		// 수류탄이 벽의 하단에 부딫힐 때
		return true;
	}

	return false;
}

// 선형 보간법을 이용한 직선의 방정식을 사용해서 선과 선의 충돌체크
bool CCollisionManager::CollideLineToLine(LINEINFO& _DstLine, LINEINFO& _SrcLine)
{
	// 한 선의 시작점
	// 한 선의 끝점
	LINEPOS fAP1 = _DstLine.tLPoint;
	LINEPOS fAP2 = _DstLine.tRPoint;

	LINEPOS fBP1 = _SrcLine.tLPoint;
	LINEPOS fBP2 = _SrcLine.tRPoint;

	float fT;
	float fS;
	float fUnder = (fBP2.fY - fBP1.fY) * (fAP2.fX - fAP1.fX) - (fBP2.fX - fBP1.fX) * (fAP2.fY - fAP1.fY);
	if (fUnder == 0)
		return false;

	float _fT = (fBP2.fX - fBP1.fX) * (fAP1.fY - fBP1.fY) - (fBP2.fY - fBP1.fY) * (fAP1.fX - fBP1.fX);
	float _fS = (fAP2.fX - fAP1.fX) * (fAP1.fY - fBP1.fY) - (fAP2.fY - fAP1.fY) * (fAP1.fX - fBP1.fX);


	fT = _fT / fUnder;
	fS = _fS / fUnder;

	if (fT < 0.f || fT > 1.0f || fS < 0.f || fS > 1.f)
		return false;
	if (_fT == 0.f && _fS == 0.f)
		return false;

	return true;
}

// 선형 보간법을 이용한 직선의 방정식을 사용해서 선과 선의 충돌체크
bool CCollisionManager::CollideLineToLineReturn(LINEINFO& _DstLine, LINEINFO& _SrcLine, LINEPOS* _pOut)
{
	// 한 선의 시작점
	// 한 선의 끝점
	LINEPOS fAP1 = _DstLine.tLPoint;
	LINEPOS fAP2 = _DstLine.tRPoint;

	LINEPOS fBP1 = _SrcLine.tLPoint;
	LINEPOS fBP2 = _SrcLine.tRPoint;

	float fT;
	float fS;
	float fUnder = (fBP2.fY - fBP1.fY) * (fAP2.fX - fAP1.fX) - (fBP2.fX - fBP1.fX) * (fAP2.fY - fAP1.fY);
	if (fUnder == 0)
		return false;

	float _fT = (fBP2.fX - fBP1.fX) * (fAP1.fY - fBP1.fY) - (fBP2.fY - fBP1.fY) * (fAP1.fX - fBP1.fX);
	float _fS = (fAP2.fX - fAP1.fX) * (fAP1.fY - fBP1.fY) - (fAP2.fY - fAP1.fY) * (fAP1.fX - fBP1.fX);


	fT = _fT / fUnder;
	fS = _fS / fUnder;

	if (fT < 0.f || fT > 1.0f || fS < 0.f || fS > 1.f)
		return false;
	if (_fT == 0.f && _fS == 0.f)
		return false;

	// 요밑에서 교차점 구하기
	_pOut->fX = fAP1.fX + fT * (float)(fAP2.fX - fAP1.fX);
	_pOut->fY = fAP1.fY + fT * (float)(fAP2.fY - fAP1.fY);

	return true;
}

// 플레이어 타일
bool CCollisionManager::CollideCharacterTile(CObj* _pDstObj, TILE* _pTile)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	if (_pTile->iOption == 0) // 0이면 지나갈 수 있게
		return false;

	RECT rcDst = _pDstObj->GetRect(); // 플레이어
	RECT rcSrc =					  // Tile
	{
		LONG(_pTile->vPos.x - (128.f * 0.5f)),
		LONG(_pTile->vPos.y - (128.f * 0.5f)),
		LONG(_pTile->vPos.x + (128.f * 0.5f)),
		LONG(_pTile->vPos.y + (128.f * 0.5f))
	};

	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		// 충돌했다면 player 밀쳐내기
		int iVertical = rc.bottom - rc.top;
		int iHorizontal = rc.right - rc.left;

		if (iHorizontal > iVertical)
		{
			if (_pDstObj->GetY() < _pTile->vPos.y)
				_pDstObj->SetY(_pDstObj->GetY() - iVertical);
			else if (_pDstObj->GetY() > _pTile->vPos.y)
				_pDstObj->SetY(_pDstObj->GetY() + iVertical);
		}
		else
		{
			if (_pDstObj->GetX() > _pTile->vPos.x)
				_pDstObj->SetX(_pDstObj->GetX() + iHorizontal);
			else if (_pDstObj->GetX() < _pTile->vPos.x)
				_pDstObj->SetX(_pDstObj->GetX() - iHorizontal);
		}

		return true;
	}

	return false;
	
}

bool CCollisionManager::CollideCharacterTile(CObj* _pDstObj, TILE* _pTile, RECT* _pOut)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	//if (_pTile->iOption == 0) // 0이면 지나갈 수 있게
	//	return false;

	RECT rcDst = _pDstObj->GetRect(); // 플레이어
	RECT rcSrc =					  // Tile
	{
		LONG(_pTile->vPos.x - (128.f * 0.5f)),
		LONG(_pTile->vPos.y - (128.f * 0.5f)),
		LONG(_pTile->vPos.x + (128.f * 0.5f)),
		LONG(_pTile->vPos.y + (128.f * 0.5f))
	};

	if (IntersectRect(_pOut, &rcSrc, &rcDst) == TRUE)
		return true;

	return false;
}

bool CCollisionManager::CollideCharacterStructure(CObj* _pDstObj, CObj* _pSrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	DO_IF_IS_NOT_VALID_OBJ(_pSrcObj)
		return false;

	RECT rcDst = _pDstObj->GetRect(); // 플레이어
	RECT rcSrc = _pSrcObj->GetRect(); // Structure

	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		// 충돌했다면 player 밀쳐내기
		int iVertical = rc.bottom - rc.top;
		int iHorizontal = rc.right - rc.left;

		if (iHorizontal > iVertical)
		{
			if (_pDstObj->GetY() < _pSrcObj->GetY())
				_pDstObj->SetY(_pDstObj->GetY() - iVertical);
			else if (_pDstObj->GetY() > _pSrcObj->GetY())
				_pDstObj->SetY(_pDstObj->GetY() + iVertical);
		}
		else
		{
			if (_pDstObj->GetX() > _pSrcObj->GetX())
				_pDstObj->SetX(_pDstObj->GetX() + iHorizontal);
			else if (_pDstObj->GetX() < _pSrcObj->GetX())
				_pDstObj->SetX(_pDstObj->GetX() - iHorizontal);
		}

		return true;
	}

	return false;

}

bool CCollisionManager::CollideReflectStructure(CObj* _pDstObj, CObj* _pSrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	DO_IF_IS_NOT_VALID_OBJ(_pSrcObj) 
		return false;

	RECT rcDst = _pDstObj->GetRect(); // 플레이어
	RECT rcSrc = _pSrcObj->GetRect(); // 총알

	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		int iVertical = rc.bottom - rc.top;
		int iHorizontal = rc.right - rc.left;

		if (iHorizontal > iVertical)
		{
			if (_pSrcObj->GetY() < _pDstObj->GetY())
				_pSrcObj->SetY(_pSrcObj->GetY() - iVertical * 10.f);
			else if (_pSrcObj->GetY() > _pDstObj->GetY())
				_pSrcObj->SetY(_pSrcObj->GetY() + iVertical * 10.f);

			float fIncidenceDegree = _pSrcObj->GetDegree();
			float fReflectDegree = 360.f - fIncidenceDegree;
			_pSrcObj->SetDegree((fReflectDegree));
		}
		else
		{
			if (_pSrcObj->GetX() > _pDstObj->GetX())
				_pSrcObj->SetX(_pSrcObj->GetX() + iHorizontal * 10.f);
			else if (_pSrcObj->GetX() < _pDstObj->GetX())
				_pSrcObj->SetX(_pSrcObj->GetX() - iHorizontal * 10.f);

			float fIncidenceDegree = _pSrcObj->GetDegree();
			float fReflectDegree = 180.f - fIncidenceDegree;
			_pSrcObj->SetDegree((fReflectDegree));
		}
		return true;
	}

	return false;
}

bool CCollisionManager::CollideReflectWall(TILE* _pTile, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	RECT rc = {};
	RECT rcDst =
	{
		LONG(_pTile->vPos.x - (128.f * 0.5f)),
		LONG(_pTile->vPos.y - (128.f * 0.5f)),
		LONG(_pTile->vPos.x + (128.f * 0.5f)),
		LONG(_pTile->vPos.y + (128.f * 0.5f))
	};
	RECT rcSrc = _SrcObj->GetRect();

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
	{
		int iVertical = rc.bottom - rc.top;
		int iHorizontal = rc.right - rc.left;

		if (iHorizontal > iVertical)
		{
			if (_SrcObj->GetY() < _pTile->vPos.y)
				_SrcObj->SetY(_SrcObj->GetY() - iVertical * 10.f);
			else if (_SrcObj->GetY() > _pTile->vPos.y)
				_SrcObj->SetY(_SrcObj->GetY() + iVertical * 10.f);

			float fIncidenceDegree = _SrcObj->GetDegree();
			float fReflectDegree = 360.f - fIncidenceDegree;
			_SrcObj->SetDegree((fReflectDegree));
		}
		else
		{
			if (_SrcObj->GetX() > _pTile->vPos.x)
				_SrcObj->SetX(_SrcObj->GetX() + iHorizontal * 10.f);
			else if (_SrcObj->GetX() < _pTile->vPos.x)
				_SrcObj->SetX(_SrcObj->GetX() - iHorizontal * 10.f);

			float fIncidenceDegree = _SrcObj->GetDegree();
			float fReflectDegree = 180.f - fIncidenceDegree;
			_SrcObj->SetDegree((fReflectDegree));
		}

		return true;
	}

	return false;
}

CCollisionManager::~CCollisionManager()
{
}
