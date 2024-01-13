#include "stdafx.h"
#include "Player_UI_Manager.h"

#include "Player.h"
#include "UI_Player_State.h"

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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_State"),
		CUI_Player_State::Create(m_pGraphic_Device))))
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

	CUI_Player_State::UI_PLAYER_STATE_DESC StateDesc = {};
	StateDesc. vSize = { 873.f * 0.4f, 653.f * 0.4f };
	StateDesc.vPos = { StateDesc.vSize.x * 0.5f, g_iWinSizeY - StateDesc.vSize.y * 0.5f };
	StateDesc.pPlayerHp = m_pPlayer->Get_Hp_Address();


	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Player_UI"), TEXT("Prototype_GameObject_UI_Player_State"), &StateDesc)))
		return E_FAIL;

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
