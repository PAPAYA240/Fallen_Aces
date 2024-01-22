#include "stdafx.h"
#include "Fences_Door.h"
#include "Player.h"
#include "PadLock.h"
#include "KeyObject.h"

CFences_Door::CFences_Door(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CDoorObj(_pGraphic_Device)
{
}

CFences_Door::CFences_Door(const CFences_Door& rhs)
	: CDoorObj(rhs)
{
}

HRESULT CFences_Door::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFences_Door::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC  MyDesc = {};

	if (nullptr != pArg)
	{
		CGameObject::GAMEOBJECT_DESC* pDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;
	}

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Fences_Door"));

	return S_OK;
}

void CFences_Door::Tick(_float fTimeDelta)
{
	UnLockingControl(fTimeDelta);
	// 만약 좌물쇠가 열리면 움직임 가능
}

void CFences_Door::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CFences_Door::Render()
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

void CFences_Door::UnLockingControl(_float fTimeDelta)
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

		CPadLock* bDoor_Open = dynamic_cast<CPadLock*>(pGameObject);

		for (auto iter = pPlayer->Get_Key_Address()->begin(); iter != pPlayer->Get_Key_Address()->end();)
		{
			if (*iter != nullptr && (*iter)->Get_Key() == PUZZLE_ID::PADLOCK_KEY)
			{
				if (DOWN == m_pGameInstance->Get_KeyState('E') && bDoor_Open->Get_Door_Open())
				{
					// 문 오픈
					UnLockAnimation(fTimeDelta);
				}
				else
					++iter;
			}
			else
				++iter;
		}
	}
}


HRESULT CFences_Door::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFences_Door::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CFences_Door::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CFences_Door* CFences_Door::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFences_Door* pInstance = new CFences_Door(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFences_Door"));

		Safe_Release<CFences_Door*>(pInstance);
	}

	return pInstance;
}

CGameObject* CFences_Door::Clone(void* pArg)
{
	CFences_Door* pInstance = new CFences_Door(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CFences_Door Clone "));

		Safe_Release<CFences_Door*>(pInstance);
	}
	return pInstance;
}

CGameObject* CFences_Door::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}

void CFences_Door::Free()
{
	__super::Free();

}
