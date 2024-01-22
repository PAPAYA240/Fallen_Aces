#include "stdafx.h"
#include "Boss_Door.h"
#include "Player.h"
#include "KeyObject.h"

CBoss_Door::CBoss_Door(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CDoorObj(_pGraphic_Device)
{
}

CBoss_Door::CBoss_Door(const CBoss_Door& rhs)
	: CDoorObj(rhs)
{
}

HRESULT CBoss_Door::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Door::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC  MyDesc = {};
	MyDesc.fRotationPerSec = D3DXToRadian(90.0f);
	MyDesc.fSpeedPerSec = 2.f;

	if (nullptr != pArg)
	{
		CGameObject::GAMEOBJECT_DESC* pDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;
	}

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(2.f, 5.f, 2.f));
	m_pTransformCom->Set_Scaled(2.f, 3.f, 1.f);

	m_ePuzzleID = PUZZLE_ID::BOSS_KEY;

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Boss_Door"));

	return S_OK;
}

void CBoss_Door::Tick(_float fTimeDelta)
{
	//m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fTimeDelta * 60.f);
	UnLockingControl(fTimeDelta);

	if (m_bOpenDoor)
	{
		if (DOWN == m_pGameInstance->Get_KeyState('E') || m_bDoorRot)
		{
			m_bDoorRot = true;
			UnLockAnimation(fTimeDelta);
		}
	}
	//
	
	// 만약 좌물쇠가 열리면 움직임 가능
}

void CBoss_Door::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBoss_Door::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIbufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Door::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Door::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CBoss_Door::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CBoss_Door::UnLockingControl(_float fTimeDelta)
{
	const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDiff = vMyPos - vPlayerPos;

	_float fDistance = D3DXVec3Length(&vDiff);

	if (abs(fDistance) <= 3.0f)
	{
		list<class CGameObject*>* pPlayerLayerList = m_pGameInstance->Get_LayerList(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
		CGameObject* pGameObject = *(pPlayerLayerList->begin());
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		for (auto iter = pPlayer->Get_Key_Address()->begin(); iter != pPlayer->Get_Key_Address()->end();)
		{
			if (*iter != nullptr && (*iter)->Get_Key() == m_ePuzzleID)
			{
				if (DOWN == m_pGameInstance->Get_KeyState('E'))
				{
					iter = pPlayer->Get_Key_Address()->erase(iter);
					m_bOpenDoor = true;
				}
				else
					++iter;
			}
			else
				++iter;
		}
	}
}

CBoss_Door* CBoss_Door::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Door* pInstance = new CBoss_Door(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBoss_Door"));

		Safe_Release<CBoss_Door*>(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Door::Clone(void* pArg)
{
	CBoss_Door* pInstance = new CBoss_Door(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBoss_Door Clone "));

		Safe_Release<CBoss_Door*>(pInstance);
	}
	return pInstance;
}

CGameObject* CBoss_Door::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}

void CBoss_Door::Free()
{
	__super::Free();

}
