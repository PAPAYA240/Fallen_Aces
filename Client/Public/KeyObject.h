#pragma once
#include "Client_Defines.h"
#include "LandObject.h"
#include "CItem.h"
BEGIN(Client)
class CKeyObject abstract : public CLandObject
{
public :
	enum COLOR { RED, BULE, YELLOW, GREEN, _END};

	typedef struct tagPuzzleObjDesc : public CLandObject::LANDOBJECT_DESC
	{
		COLOR	eColor;

	}KEY_DESC;

public :
	CKeyObject(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CKeyObject(const CKeyObject& _rhs);
	virtual ~CKeyObject() = default;

public :
	PUZZLE_ID	 Get_Key() { return m_ePuzzleID;  }
	COLOR		Get_Color() { return m_eColor; }

public :
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

protected :
	virtual void	Key_Control();
	void			BillBoard_Camera();

protected:
	PUZZLE_ID		m_ePuzzleID		= { PUZZLE_ID::_END };

	COLOR			m_eColor		= { COLOR::_END };
	_bool			m_bGetKey		= { false };
	CItem::BILLBORAD_TYPE			m_eBillbordType = { CItem::BILLBORAD_TYPE::XZ };

protected :
	CTexture*		m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };

public :
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END

