#include "UI.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CUI::CUI(const CUI& rhs)
	:CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	UI_DESC* pDesc = (UI_DESC*)pArg;

	m_vPos = pDesc->vPos;
	m_vSize = pDesc->vSize;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
}

void CUI::Late_Tick(_float fTimeDelta)
{
}

void CUI::Initialize_UI_Setting(_uint iWinSizeX, _uint iWinSizeY)
{
	D3DXMatrixIdentity(&m_ViewMatrix);

	/* 직교투영행렬을 만들어주는 함수. */
	/* 직교투영을 수행하여 투영스페이로 옮겨주기위한 영역(뷰볼륨whnf)을 설정해달라. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)iWinSizeX, (_float)iWinSizeY, 0.0f, 1.f);

	m_pTransformCom->Set_Scaled(m_vSize.x, m_vSize.y, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(
		m_vPos.x - iWinSizeX * 0.5f,
		-m_vPos.y + iWinSizeY * 0.5f,
		m_vPos.z
	));
}

CGameObject* CUI::Clone(void* pArg)
{
	return nullptr;
}

void CUI::Free()
{
}
