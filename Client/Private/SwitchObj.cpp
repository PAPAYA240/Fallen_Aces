#include "stdafx.h"
#include "SwitchObj.h"
#include "Player.h"

CSwitchObj::CSwitchObj(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSwitchObj::CSwitchObj(const CSwitchObj& rhs)
	: CGameObject (rhs)
{
}

HRESULT CSwitchObj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSwitchObj::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSwitchObj::Tick(_float _DeltaTime)
{
	Switch_Control();
	Check_Switch();
}

void CSwitchObj::Switch_Control()
{
	const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDiff = vMyPos - vPlayerPos;

	_float fDistance = D3DXVec3Length(&vDiff);

	if (abs(fDistance) <= 1.0f)
	{
		if (DOWN == m_pGameInstance->Get_KeyState('E'))
		{
			list<class CGameObject*>* pPlayerLayerList = m_pGameInstance->Get_LayerList(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
			CGameObject* pGameObject = *(pPlayerLayerList->begin());
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

			if (m_bSwitch_Check == SWITCH::OFF)
				m_bSwitch_Check = SWITCH::ON;

			else if (m_bSwitch_Check == SWITCH::ON)
				m_bSwitch_Check = SWITCH::OFF;
		}
	}
}

void CSwitchObj::Check_Switch()
{
	switch (_uint(m_ePuzzleID))
	{
	case _uint(PUZZLE_ID::BUTTEN_SWITCH):
	{
		if (SWITCH::ON == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Butten_Switch_On"));

		else if (SWITCH::OFF == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Butten_Switch_Off"));
	}
	break;

	case _uint(PUZZLE_ID::LIGHT_SWITCH):
	{
		if (SWITCH::ON == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Light_Switch_On"));

		else if (SWITCH::OFF == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Light_Switch_Off"));
	}
	break;

	case _uint(PUZZLE_ID::LEVER_SWITCH):
	{
		if (SWITCH::ON == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Lever_Switch_On"));

		else if (SWITCH::OFF == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Lever_Switch_Off"));
	}
	break;
	case _uint(PUZZLE_ID::RED_SWITCH):
	{
		if (SWITCH::ON == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Red_Butten_Switch_On"));

		else if (SWITCH::OFF == m_bSwitch_Check)
			m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Red_Butten_Switch_Off"));
	}
	break;
	}
}

void CSwitchObj::Free()
{
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

	__super::Free();
}
