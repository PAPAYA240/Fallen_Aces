#include "stdafx.h"
#include "Player_UI_Manager.h"

#include "Player.h"
#include "UI_HUD.h"
#include "UI_Player_State.h"
#include "UI_Select.h"
#include "UI_Equip.h"
#include "UI_Slot.h"

#include "CItem.h"

CPlayer_UI_Manager::CPlayer_UI_Manager(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CPlayer_UI_Manager::CPlayer_UI_Manager(CPlayer_UI_Manager& rhs)
	:CGameObject(rhs)
{
}

HRESULT CPlayer_UI_Manager::Initialize_Prototype()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HUD"),
		CUI_HUD::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_State"),
		CUI_Player_State::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Select"),
		CUI_Select::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Slot"),
		CUI_Slot::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Equip"),
		CUI_Equip::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_UI_Manager::Initialize(void* pArg)
{
	Player_UI_Manager_DESC* Desc = (Player_UI_Manager_DESC*)pArg;

	m_pPlayer = Desc->pPlayer;

	Safe_AddRef(m_pPlayer);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


#pragma region Cloned HUD UI
	CUI_HUD::UI_DESC HUDDesc = {};
	HUDDesc.vSize = { _uint(873.f * 0.3f), _uint(653.f * 0.3f) };
	// 화면 좌하단
	HUDDesc.vPos = { HUDDesc.vSize.x * 0.5f, g_iWinSizeY - HUDDesc.vSize.y * 0.55f, 0.f };

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Player_UI"), TEXT("Prototype_GameObject_UI_HUD"), &HUDDesc)))
		return E_FAIL;
#pragma endregion

#pragma region Cloned Player State UI
	CUI_Player_State::UI_PLAYER_STATE_DESC StateDesc = {};
	StateDesc. vSize = { 565.f * 0.2f, 665.f * 0.2f};
	// 화면 좌하단
	StateDesc.vPos = { HUDDesc.vPos.x - (HUDDesc.vSize.x * 0.2f), HUDDesc.vPos.y - (HUDDesc.vSize.y * 0.1f) ,0.f };
	StateDesc.vFontSize = { HUDDesc.vSize.x * 0.075f, HUDDesc.vSize.y * 0.15f };
	StateDesc.vFontPos = { HUDDesc.vPos.x - (HUDDesc.vSize.x * 0.3f), HUDDesc.vPos.y + (HUDDesc.vSize.y * 0.3f) };
	// 플레이어의 체력 멤버변수의 주소를 가져와 연동
	StateDesc.pPlayerHp = m_pPlayer->Get_Hp_Address();

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Player_UI"), TEXT("Prototype_GameObject_UI_Player_State"), &StateDesc)))
		return E_FAIL;
#pragma endregion

#pragma region Cloned Player Select UI
	CUI_Select::UI_SELECT_DESC SelectDesc = {};
	SelectDesc.vSize = { _uint(324.f * 0.3f), _uint(186.f * 0.3f) };
	for (_int i = 0; i < 3; i++)
	{
		SelectDesc.SlotPosArr[i] = { HUDDesc.vPos.x + HUDDesc.vSize.x * 0.275f, HUDDesc.vPos.y + (SelectDesc.vSize.y * (i - 1) )};
	}
	// 화면 좌하단
	SelectDesc.vPos = { SelectDesc.SlotPosArr[0].x, SelectDesc.SlotPosArr[0].y , 0.f};
	// 플레이어의 체력 멤버변수의 주소를 가져와 연동
	SelectDesc.pSelectNum = m_pPlayer->Get_SelectItemNum_Address();

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Player_UI"), TEXT("Prototype_GameObject_UI_Select"), &SelectDesc)))
		return E_FAIL;
#pragma endregion

#pragma region Cloned Player Slot UI

	for (_int i = 0; i < 3; i++)
	{
		CUI_Slot::UI_SLOT_DESC SlotDesc = {};
		SlotDesc.vSize = { 304.f * 0.4f, 171.f * 0.4f };
		// 화면 우하단
		SlotDesc.vPos = { SelectDesc.SlotPosArr[i].x, SelectDesc.SlotPosArr[i].y, 0.f };
		// 플레이어의 장착 아이템 멤버변수의 주소를 가져와 연동
		SlotDesc.pSlotItem = m_pPlayer->Get_SlotItem_Address(i);

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Player_UI"), TEXT("Prototype_GameObject_UI_Slot"), &SlotDesc)))
			return E_FAIL;
	}
#pragma endregion

#pragma region Cloned Player Equipment UI
	CUI_Equip::UI_EQUIP_DESC EquipDesc = {};
	EquipDesc.vSize = { 304.f * 0.4f, 171.f * 0.4f };
	// 화면 우하단
	EquipDesc.vPos = { g_iWinSizeX - (EquipDesc.vSize.x * 0.5f), g_iWinSizeY - EquipDesc.vSize.y * 0.5f, 0.f };
	// 플레이어의 장착 아이템 멤버변수의 주소를 가져와 연동
	EquipDesc.pEquipItem = m_pPlayer->Get_EquipItem_Address();

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Player_UI"), TEXT("Prototype_GameObject_UI_Equip"), &EquipDesc)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

void CPlayer_UI_Manager::Tick(_float fTimeDelta)
{
}

void CPlayer_UI_Manager::Late_Tick(_float fTimeDelta)
{
}

HRESULT CPlayer_UI_Manager::Render()
{
	return S_OK;
}

CPlayer_UI_Manager* CPlayer_UI_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer_UI_Manager* pInstance = new CPlayer_UI_Manager(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : Player_UI_Manager Class"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_Manager::Clone(void* pArg)
{
	CPlayer_UI_Manager* pInstance = new CPlayer_UI_Manager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : Player_UI_Manager Class"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
}
