#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Number.h"
#include "KeyObject.h"

class CDoorObj abstract : public CGameObject
{

public:
	CDoorObj(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CDoorObj(const CDoorObj& _rhs);
	virtual ~CDoorObj() = default;

public:
	PUZZLE_ID	 Get_Key() { return m_ePuzzleID; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;

	virtual	void		UnLockingControl(_float fTimeDelta) = 0;
	void				UnLockAnimation(_float fTimeDelta);

protected:
	PUZZLE_ID			m_ePuzzleID = { PUZZLE_ID::_END };
	CKeyObject::COLOR	m_eColorKey = { CKeyObject::COLOR::_END };

	_bool			m_bOpenDoor = { false };
	_bool			m_bDoorRot = { false };
	_float			m_bDoorTime = { 0.0f };

protected:
	CTexture*		m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

