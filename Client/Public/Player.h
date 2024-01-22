#pragma once

#include "Client_Defines.h"
#include "LandObject.h"



BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPlayer final : public CLandObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	// 플레이어 멤버 변수의 주소를 반환해주는 함수들 (UI에 연동해줌)
	_uint* Get_Hp_Address() { return &m_iHp; }
	vector<class CItem*>* Get_Inventory_Address() { return &m_vecInven; }
	class CItem** Get_EquipItem_Address() { return &m_pEquipment; }
	class CItem** Get_SlotItem_Address(_int iSlotIdx) { return &m_vecInven[iSlotIdx]; }
	_uint* Get_SelectItemNum_Address() { return &m_iSelectItemNum; }
	void	Set_PickItem(class CItem* pickItem) { m_pEquipment = pickItem; }
	vector<class CKeyObject*>* Get_Key_Address() { return &m_pKeyList; }

	bool Input_Item(class CItem* input_Item);
	bool Get_Key(class CKeyObject* Get_Key);

private:
	CTexture*				m_pTextureCom		= { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom		= { nullptr };
	class CEffect_ObjectPool*		m_pEffect			= { nullptr };

	// 플레이어의 손 제어를 하기 위한 객체 포인터
	class CPlayer_LH*		m_pPlayer_LH = { nullptr };
	class CPlayer_RH*		m_pPlayer_RH = { nullptr };

	_bool					m_bLeftPunch = { false };

private:
	_bool						m_isMove = { false };
	_float3						m_vTargetPos = {};

	// 체력
	_uint						m_iHp = { 0 };
	// 인벤토리
	vector<class CItem*>		m_vecInven;

	// 키 여부
	vector<class CKeyObject*>		m_pKeyList;

	// 현재 선택한 인벤토리 넘버
	_uint						m_iSelectItemNum = { 0 };

	// 현재 장착한 아이템
	class CItem*				m_pEquipment = { nullptr };

private:
	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr);

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

	_float3 Compute_MovePos();

	HRESULT Ready_Layer_Player_Hands(const wstring& strLayerTag, void* pArg = nullptr);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END