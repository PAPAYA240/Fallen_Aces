#include "stdafx.h"
#include "CItemMedKit.h"

CItemMedKit::CItemMedKit(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CHealItem(_pGraphic_Device)
{
}

CItemMedKit::CItemMedKit(const CItemMedKit& _rhs)
	: CHealItem(_rhs)
{
}

HRESULT CItemMedKit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemMedKit::Initialize(void* pArg)
{

	CHealItem::HEAL_ITEM_DESC MyDesc = {};

	if (nullptr != pArg)
	{
		CHealItem::HEAL_ITEM_DESC* pDesc = (CHealItem::HEAL_ITEM_DESC*)pArg;
		MyDesc.pTerrainTransform = pDesc->pTerrainTransform;
		MyDesc.pTerrainVIBuffer = pDesc->pTerrainVIBuffer;
	}

	MyDesc.eItemID = { ITEM_ID::MEDKIT };
	MyDesc.fAttackDamage = { 0.f };
	MyDesc.fDurability = { 1.f };

	MyDesc.fRotationPerSec = { D3DXToRadian(90.f) };
	MyDesc.fSpeedPerSec = { 0.f };

	MyDesc.fRecorvery_Buff = { 35.f };
	MyDesc.fRBuff_LimitTime = { 0.f };

	MyDesc.fSpeed_Buff = { 0.f };
	MyDesc.fSBuff_LimitTime = { 0.f };

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Medkit"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(rand() % 100), 5.f, _float(rand() % 100)));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(0), 5.f, _float(0)));
	m_pTransformCom->Set_Scaled(0.5f, 0.3f, 1.f);

	return S_OK;
}

void CItemMedKit::Tick(_float fTimeDelta)
{
	Check_PlayerRadius();
	Check_Durability();

	if (FAILED(SetUp_OnTerrain(0.3f)))
		return;
}

void CItemMedKit::Late_Tick(_float fTimeDelta)
{
	BillBoard_Camera();

	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CItemMedKit::Render()
{
	if (true == m_isTestDead)
		return E_FAIL;

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

CItemMedKit* CItemMedKit::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CItemMedKit* pInstance = new CItemMedKit(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : MedKit "));

		Safe_Release<CItemMedKit*>(pInstance);
	}

	return pInstance;
}

CGameObject* CItemMedKit::Clone(void* pArg)
{
	CItemMedKit* pInstance = new CItemMedKit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : MedKit Clone "));

		Safe_Release<CItemMedKit*>(pInstance);
	}

	return pInstance;
}

void CItemMedKit::Free()
{
	__super::Free();
}

HRESULT CItemMedKit::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemMedKit::Set_RenderState()
{
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CItemMedKit::Reset_RenderState()
{
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CGameObject* CItemMedKit::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{


	return nullptr;
}
