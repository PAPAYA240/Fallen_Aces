#include "stdafx.h"
#include "Boss_Key.h"

CBossKey::CBossKey(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CKeyObject(_pGraphic_Device)
{
}

CBossKey::CBossKey(const CBossKey& rhs)
	: CKeyObject(rhs)
{
}

HRESULT CBossKey::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossKey::Initialize(void* pArg)
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

	m_ePuzzleID = PUZZLE_ID::BOSS_KEY;
	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Boss_Key"));
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(6.f, 2.f, 6.f));
	m_pTransformCom->Set_Scaled(0.6f, 0.3f, 1.f);

	return S_OK;
}

void CBossKey::Tick(_float fTimeDelta)
{
	Key_Control();
}

void CBossKey::Late_Tick(_float fTimeDelta)
{
	BillBoard_Camera();
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBossKey::Render()
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


HRESULT CBossKey::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossKey::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CBossKey::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CBossKey* CBossKey::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossKey* pInstance = new CBossKey(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBossKey"));

		Safe_Release<CBossKey*>(pInstance);
	}

	return pInstance;
}

CGameObject* CBossKey::Clone(void* pArg)
{
	CBossKey* pInstance = new CBossKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBossKey Clone "));

		Safe_Release<CBossKey*>(pInstance);
	}
	return pInstance;
}


CGameObject* CBossKey::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;

}

void CBossKey::Free()
{
	__super::Free();
}
