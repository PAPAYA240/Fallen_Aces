#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Client)

class CItem abstract : public CLandObject
{
protected:
	enum BILLBORAD_TYPE { XYZ, XZ };

public:
	typedef struct tagItemDesc : public LANDOBJECT_DESC
	{
		ITEM			eItemType = { ITEM::_END };
		_float			fDurability = { 0.f };
		_float			fAttackDamage = { 0.f };
	}ITEM_DESC;

protected:
	CItem(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CItem(const CItem& _rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT			Initialize_Prototype()			override;
	virtual HRESULT			Initialize(void* pArg)			override;

public:
	const ITEM&				Get_ItemType()			const		{ return m_eItemType; }			/* 무기 종류 정보를 수집해서 플레이어가 들고있는 아이템 바꾸면 됨 */
	const ITEM_DURA&		Get_Durability()		const		{ return m_eDuraType; }		/* 무기 내구도 정보 수집해서 플레이어 손 모양 바꾸면 됨 */

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();

protected:
	void					BillBoard_Camera();
	void					Check_Durability();
	void					Check_PlayerRadius();

private:
	CGameObject*			Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr) = 0;

protected:
	ITEM					m_eItemType = { ITEM::_END };					/* 아이템 타입 */
	ITEM_DURA				m_eDuraType = { ITEM_DURA::_END };				/* 현재 무기 내구도 상태 */
	BILLBORAD_TYPE			m_eBillbordType = { BILLBORAD_TYPE::XZ };		/* 빌보드 타입 */

	_float					m_fDurability = { 0.f };						/* 아이템 내구도 */
	_float					m_fAttackDamage = { 0.f };						/* 아이템 공격력 */

	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIbufferCom = { nullptr };
};

END