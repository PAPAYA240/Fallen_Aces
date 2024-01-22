#include "stdafx.h"
#include "PadLock_Key.h"

CPadLock_Key::CPadLock_Key(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CKeyObject(_pGraphic_Device)
{
}

CPadLock_Key::CPadLock_Key(const CPadLock_Key& rhs)
	: CKeyObject(rhs)
{
}

HRESULT CPadLock_Key::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPadLock_Key::Initialize(void* pArg)
{
	CKeyObject::KEY_DESC MyDesc = {};

	if(nullptr != pArg)
	{
		CKeyObject::KEY_DESC* pDesc = (CKeyObject::KEY_DESC*)pArg;
		MyDesc.pTerrainTransform = pDesc->pTerrainTransform;
		MyDesc.pTerrainVIBuffer = pDesc->pTerrainVIBuffer;
		m_eColor = MyDesc.eColor = pDesc->eColor;
	}

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_ePuzzleID = PUZZLE_ID::PADLOCK_KEY;
	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("PadLockKey"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(7.f, 2.f, 7.f));
	m_pTransformCom->Set_Scaled(0.6f, 0.3f, 1.f);

	return S_OK;
}

void CPadLock_Key::Tick(_float fTimeDelta)
{
	Key_Control();
}

void CPadLock_Key::Late_Tick(_float fTimeDelta)
{
	BillBoard_Camera();
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPadLock_Key::Render()
{
	if (!m_bGetKey)
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
	}
	return S_OK;
}

HRESULT CPadLock_Key::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPadLock_Key::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPadLock_Key::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CPadLock_Key* CPadLock_Key::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPadLock_Key* pInstance = new CPadLock_Key(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPadLock_Key"));

		Safe_Release<CPadLock_Key*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPadLock_Key::Clone(void* pArg)
{
	CPadLock_Key* pInstance = new CPadLock_Key(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPadLock_Key Clone "));

		Safe_Release<CPadLock_Key*>(pInstance);
	}
	return pInstance;
}


CGameObject* CPadLock_Key::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;

}

void CPadLock_Key::Free()
{
	__super::Free();
}
