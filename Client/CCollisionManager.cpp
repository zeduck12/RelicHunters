#include "pch.h"
#include "CObj.h"
#include "CCollisionManager.h"
#include "CGrenade.h"

DEFINITION_SINGLETON(CCollisionManager)

// �����ڿ� �Ѿ�
bool CCollisionManager::CollideBullet(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	RECT rc = {};
	RECT rcDst = _pDstObj->GetRect();
	RECT rcSrc = _SrcObj->GetRect();

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
	{
		// ������ �ְ� �Ѿ��� ������.
		_SrcObj->SetIsValid(false);
		return true;
	}

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
		// ������ �ְ� �Ѿ��� ������.
		_SrcObj->SetIsValid(false);
		return true;
	}

	return false;
}
// �÷��̾�� ����
bool CCollisionManager::CollidePlayerMonster(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	RECT rcDst = _pDstObj->GetRect(); // �÷��̾�
	RECT rcSrc = _SrcObj->GetRect(); // ����
	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		// �浹�ߴٸ� player ���ĳ���
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
// ���� ����ź
bool CCollisionManager::CollideWallGrenade(CObj* _pDstObj, CObj* _SrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;
	DO_IF_IS_NOT_VALID_OBJ(_SrcObj)
		return false;

	RECT rc = {};
	RECT rcDst = _pDstObj->GetRect(); // ��
	RECT rcSrc = _SrcObj->GetRect();  // ����ź

	float fIncidenceDegree = 0.f;
	float fReflectDegree = 0.f;

	if (IntersectRect(&rc, &rcDst, &rcSrc) == TRUE)
	{
		// ���� �΋H�� ����ź �ݻ簢���� �ðܳ���.
		// ����ź�� ���� ������ �΋H�� ��
		if(_SrcObj->GetX() < _pDstObj->GetX() && _SrcObj->GetY() > _pDstObj->GetTop() && _SrcObj->GetY() < _pDstObj->GetBottom())
		{
			fIncidenceDegree = dynamic_cast<CGrenade*>(_SrcObj)->GetIncidenceDegree();
			fReflectDegree = 180.f - fIncidenceDegree;
			dynamic_cast<CGrenade*>(_SrcObj)->SetReflectDegree(fReflectDegree);
			dynamic_cast<CGrenade*>(_SrcObj)->SetIsCollide(true);
		}
		// ����ź�� ���� ������ �΋H�� ��
		if (_SrcObj->GetX() > _pDstObj->GetX() && _SrcObj->GetY() > _pDstObj->GetTop() && _SrcObj->GetY() < _pDstObj->GetBottom())
		{
			fIncidenceDegree = dynamic_cast<CGrenade*>(_SrcObj)->GetIncidenceDegree();
			fReflectDegree = 180.f - fIncidenceDegree;
			dynamic_cast<CGrenade*>(_SrcObj)->SetReflectDegree(fReflectDegree);
			dynamic_cast<CGrenade*>(_SrcObj)->SetIsCollide(true);
		}
		// ����ź�� ���� ��ܿ� �΋H�� ��
		// ����ź�� ���� �ϴܿ� �΋H�� ��
		return true;
	}

	return false;
}

// ���� �������� �̿��� ������ �������� ����ؼ� ���� ���� �浹üũ
bool CCollisionManager::CollideLineToLine(LINEINFO& _DstLine, LINEINFO& _SrcLine)
{
	// �� ���� ������
	// �� ���� ����
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

// ���� �������� �̿��� ������ �������� ����ؼ� ���� ���� �浹üũ
bool CCollisionManager::CollideLineToLineReturn(LINEINFO& _DstLine, LINEINFO& _SrcLine, LINEPOS* _pOut)
{
	// �� ���� ������
	// �� ���� ����
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

	// ��ؿ��� ������ ���ϱ�
	_pOut->fX = fAP1.fX + fT * (float)(fAP2.fX - fAP1.fX);
	_pOut->fY = fAP1.fY + fT * (float)(fAP2.fY - fAP1.fY);

	return true;
}

// �÷��̾� Ÿ��
bool CCollisionManager::CollideCharacterTile(CObj* _pDstObj, TILE* _pTile)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	if (_pTile->iOption == 0) // 0�̸� ������ �� �ְ�
		return false;

	RECT rcDst = _pDstObj->GetRect(); // �÷��̾�
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
		// �浹�ߴٸ� player ���ĳ���
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

bool CCollisionManager::CollideCharacterStructure(CObj* _pDstObj, CObj* _pSrcObj)
{
	DO_IF_IS_NOT_VALID_OBJ(_pDstObj)
		return false;

	DO_IF_IS_NOT_VALID_OBJ(_pSrcObj)
		return false;

	RECT rcDst = _pDstObj->GetRect(); // �÷��̾�
	RECT rcSrc = _pSrcObj->GetRect(); // Structure

	RECT rc = {};
	if (IntersectRect(&rc, &rcSrc, &rcDst) == TRUE)
	{
		// �浹�ߴٸ� player ���ĳ���
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

CCollisionManager::~CCollisionManager()
{
}
