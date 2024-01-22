#include "stdafx.h"
#include "Butten_Switch.h"

CButten_Switch::CButten_Switch(LPDIRECT3DDEVICE9 _pGraphic_Device)
	:CSwitchObj(_pGraphic_Device)
{
}

CButten_Switch::CButten_Switch(const CButten_Switch& rhs)
	: CSwitchObj(rhs)
{
}

HRESULT CButten_Switch::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButten_Switch::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC MyDesc = {};

	if (nullptr != pArg)
	{
		CGameObject::GAMEOBJECT_DESC* pDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;
	}

	if (FAILED(__super::Initialize(&pArg)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_ePuzzleID = PUZZLE_ID::BUTTEN_SWITCH;
	m_bSwitch_Check = SWITCH::OFF;

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Butten_Switch_Off"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(rand() % 100), 3.f, _float(rand() % 100)));
	//m_pTransformCom->Set_Scaled(0.3f, 0.3f, 1.f);

	return S_OK;
}

void CButten_Switch::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CButten_Switch::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CButten_Switch::Render()
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

HRESULT CButten_Switch::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CButten_Switch::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CButten_Switch* CButten_Switch::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CButten_Switch* pInstance = new CButten_Switch(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CButten_Switch "));

		Safe_Release<CButten_Switch*>(pInstance);
	}

	return pInstance;
}

HRESULT CButten_Switch::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}


CGameObject* CButten_Switch::Clone(void* pArg)
{
	CButten_Switch* pInstance = new CButten_Switch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CButten_Switch Clone "));

		Safe_Release<CButten_Switch*>(pInstance);
	}

	return pInstance;
}

CGameObject* CButten_Switch::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}


void CButten_Switch::Free()
{
	__super::Free();
}
