#include "stdafx.h"
#include "PadLock.h"
#include "Player.h"
#include "KeyObject.h"

CPadLock::CPadLock(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CPadLock::CPadLock(const CPadLock& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPadLock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPadLock::Initialize(void* pArg)
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

	m_fFrame = 0.f;
	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("PadLockChain"));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 5.f, 3.f));
	
	return S_OK;
}

void CPadLock::Tick(_float fTimeDelta)
{
	UnLockingControl();
	UnLockAnimation(fTimeDelta);
}

void CPadLock::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPadLock::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0, (_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIbufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPadLock::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPadLock::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPadLock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CPadLock::UnLockingControl()
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
			if (*iter != nullptr && (*iter)->Get_Key() == PUZZLE_ID::PADLOCK_KEY)
			{
				if (DOWN == m_pGameInstance->Get_KeyState('E') && !m_bUnLock)
				{
					m_bUnLock = true;
					iter = pPlayer->Get_Key_Address()->erase(iter);
				}
				else
					++iter;
			}
			else
				++iter; 
		}
	}
}

void CPadLock::UnLockAnimation(_float fTimeDelta)
{
	if (m_bUnLock)
	{
		m_Drop_Time += fTimeDelta;

		if (m_Drop_Time < 0.4f)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0.f, 2.5f * fTimeDelta, 0.f));
		}
		else
		{
			// 아래로 이동
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				m_pTransformCom->Get_State(CTransform::STATE_POSITION) - _float3(0.f, 3.5f * fTimeDelta, 0.f));

			m_fFrame = 1.f;
			m_bDoorOn = true;
		}
	}
	
}

CPadLock* CPadLock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPadLock* pInstance = new CPadLock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPadLock"));

		Safe_Release<CPadLock*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPadLock::Clone(void* pArg)
{
	CPadLock* pInstance = new CPadLock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPadLock Clone "));

		Safe_Release<CPadLock*>(pInstance);
	}
	return pInstance;
}

CGameObject* CPadLock::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}

void CPadLock::Free()
{
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

	__super::Free();
}
