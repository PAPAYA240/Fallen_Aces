#include "stdafx.h"
#include "UI_Player_State.h"

CUI_Player_State::CUI_Player_State(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUI(pGraphic_Device)
{
}

CUI_Player_State::CUI_Player_State(CUI_Player_State& rhs)
    :CUI(rhs)
{
}

HRESULT CUI_Player_State::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Player_State::Initialize(void* pArg)
{
    UI_PLAYER_STATE_DESC* Desc = (UI_PLAYER_STATE_DESC*)pArg;
    m_pPlayerHp = Desc->pPlayerHp;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    Initialize_UI_Setting(g_iWinSizeX, g_iWinSizeY);

    if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Default"), TEXT("넥슨Lv1 고딕"), m_vSize.x * 0.1f, m_vSize.y * 0.18f, FW_BOLD)))
        return E_FAIL;

    return S_OK;
}

void CUI_Player_State::Tick(_float fTimeDelta)
{
}

void CUI_Player_State::Late_Tick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Player_State::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

    if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
        return E_FAIL;

    if (FAILED(Set_RenderState()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pGameInstance->Render_Font(TEXT("Font_Default"),to_wstring(*m_pPlayerHp), &_float2(m_vPos.x - (m_vSize.x * 0.3f), m_vPos.y + (m_vSize.y * 0.3f)), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

    if (FAILED(Reset_RenderState()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player_State::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player_State::Set_RenderState()
{
    /* 알파테스트를 한다. (그려지는 픽셀들의 알파를 검사하여 그릴지 안그릴지를 판단한다.) */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    /* 내가 설정한 알파는 0이다. */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);

    /* 그려지는 픽세르이 알파가 내가 설정한 알파보다 클때만 그려라. */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CUI_Player_State::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CUI_Player_State* CUI_Player_State::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI_Player_State* pInstance = new CUI_Player_State(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : UI_Player_State Class"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player_State::Clone(void* pArg)
{
    CUI_Player_State* pInstance = new CUI_Player_State(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : UI_Player_State Class"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Player_State::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}
