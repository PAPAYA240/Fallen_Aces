#include "stdafx.h"
#include "KeyCard.h"

CKeyCard::CKeyCard(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CKeyObject(_pGraphic_Device)
{
}

CKeyCard::CKeyCard(const CKeyCard& rhs)
	: CKeyObject(rhs)
{
}

HRESULT CKeyCard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKeyCard::Initialize(void* pArg)
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

	m_ePuzzleID = PUZZLE_ID::COLOR_KEYCARD;
	Check_KeyColor();

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(rand() % 100), 2.f, _float(rand() % 100)));
	m_pTransformCom->Set_Scaled(0.6f, 0.3f, 1.f);

	return S_OK;
}

void CKeyCard::Tick(_float fTimeDelta)
{
	//Check_Switch();
	Key_Control();
}

void CKeyCard::Late_Tick(_float fTimeDelta)
{
	BillBoard_Camera();
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CKeyCard::Render()
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

void CKeyCard::Check_KeyColor()
{
	if (COLOR::BULE == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Keycard_Blue"));

	else if (COLOR::GREEN == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Keycard_Green"));

	else if (COLOR::RED == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Keycard_Red"));

	else if (COLOR::YELLOW == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Keycard_Yellow"));
}

HRESULT CKeyCard::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CKeyCard::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CKeyCard::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CKeyCard* CKeyCard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKeyCard* pInstance = new CKeyCard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CKeyCard"));

		Safe_Release<CKeyCard*>(pInstance);
	}

	return pInstance;
}

CGameObject* CKeyCard::Clone(void* pArg)
{
	CKeyCard* pInstance = new CKeyCard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CKeyCard Clone "));

		Safe_Release<CKeyCard*>(pInstance);
	}
	return pInstance;
}


CGameObject* CKeyCard::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;

}

void CKeyCard::Free()
{
	__super::Free();
}
