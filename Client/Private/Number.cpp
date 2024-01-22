#include "stdafx.h"
#include "Number.h"
#include "Combo_Lock.h"
#include "Combo_Lock_Obj.h"

CNumber::CNumber(LPDIRECT3DDEVICE9 _pGraphic_Device)
    :CGameObject(_pGraphic_Device)
{
}

CNumber::CNumber(const CNumber& rhs)
    :CGameObject(rhs)
{
}

HRESULT CNumber::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNumber::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        CCombo_Lock::LOCK_DESC* pDesc = (CCombo_Lock::LOCK_DESC*)pArg;
        m_vPos = pDesc->vPos;
        m_fDistance = pDesc->fDistance;
        m_eNumber = pDesc->eNum;
    }

    if (FAILED(__super::Initialize(&pArg)))
        return E_FAIL;

    if (FAILED(Add_MyComponents()))
        return E_FAIL;
   

    m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Combo_Number"));

    if (nullptr != pArg)
    {
        CCombo_Lock::LOCK_DESC* pDesc = (CCombo_Lock::LOCK_DESC*)pArg;
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(pDesc->vPos.x + pDesc->fObj_Distance, pDesc->vPos.y - 0.01f, pDesc->vPos.z - 0.01f));
        m_pTransformCom->Set_Scaled(0.06f, 0.06f, 1.f);
    }
    m_eCurrentNumber = CCombo_Lock::NUMBER::FIRST;

    return S_OK;
}

void CNumber::Tick(_float fTimeDelta)
{
    if(!m_bCorrect && !m_bAllStop)
        Access_Number();

    if (m_bAllStop)
    {
        m_fTime += fTimeDelta;

        if(m_fTime >= 1.f)
            m_bCorrect = true;
    }
}

void CNumber::Late_Tick(_float fTimeDelta)
{
    if (m_bProj)
        m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CNumber::Render()
{
    CGameObject* p = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Number"), 0);

    if (!dynamic_cast<CNumber*>(p)->Get_Correct() && m_bProj)
    {
        if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
            return E_FAIL;

        if (m_bProj)
        {
            m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
            m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
        }

        if (FAILED(m_pTextureCom->Bind_Texture(0, (_uint)m_fFrame)))
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

HRESULT CNumber::Add_MyComponents()
{
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
        return E_FAIL;


    return S_OK;
}

HRESULT CNumber::Set_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CNumber::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}


void CNumber::ProjTrans()
{
    /* 직교 */
    D3DXMatrixIdentity(&m_ViewMatrix);

    /* 직교투영행렬을 만들어주는 함수. */
    /* 직교투영을 수행하여 투영스페이로 옮겨주기위한 영역(뷰볼륨whnf)을 설정해달라. */
    D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, -10.0f, 200.0f);

    m_fX = g_iWinSizeX * m_fDistance;
    m_fY = g_iWinSizeY * 0.02f ;

    m_pTransformCom->Set_Scaled(80.f, 80.f, 100.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX, m_fY, 0.f));
}


void CNumber::Access_Number()
{
    CGameObject* p = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Number"), 0);
    CGameObject* pObj = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_LockObj"), 0);

    if (dynamic_cast<CCombo_Lock_Obj*>(pObj)->Get_Access())
        m_bProj = !m_bProj;

    if (m_bProj)
    {
        ProjTrans();

        if (m_eNumber == m_eCurrentNumber && DOWN == m_pGameInstance->Get_KeyState(VK_UP))
        {
            _uint iMaxSize = m_pTextureCom->Container_MaxSize();

            if (m_fFrame >= iMaxSize - 1)
                m_fFrame = 0.0f;
            else
                m_fFrame += 1.f;
        }

        // Arrow Control
        if (DOWN == m_pGameInstance->Get_KeyState(VK_RIGHT))
        {
            if (m_eCurrentNumber == CCombo_Lock::NUMBER::FIRST)
                m_eCurrentNumber = CCombo_Lock::NUMBER::SECEND;

            else if (m_eCurrentNumber == CCombo_Lock::NUMBER::SECEND)
                m_eCurrentNumber = CCombo_Lock::NUMBER::THIRED;
        }
        else if (DOWN == m_pGameInstance->Get_KeyState(VK_LEFT))
        {
            if (m_eCurrentNumber == CCombo_Lock::NUMBER::SECEND)
                m_eCurrentNumber = CCombo_Lock::NUMBER::FIRST;

            else if (m_eCurrentNumber == CCombo_Lock::NUMBER::THIRED)
                m_eCurrentNumber = CCombo_Lock::NUMBER::SECEND;
        }

        // 정답 5 1 8
        if (m_eCurrentNumber == CCombo_Lock::NUMBER::FIRST)
        {
            if (m_eCurrentNumber == CCombo_Lock::NUMBER::FIRST && m_fFrame == 1)
                m_LockUn = true;
            else
                m_LockUn = false;
        }

        if (m_eCurrentNumber == CCombo_Lock::NUMBER::SECEND)
        {
            if (m_eCurrentNumber == CCombo_Lock::NUMBER::SECEND && m_fFrame == 1)
                m_LockUn = true;
            else
                m_LockUn = false;
        }

        if (m_eCurrentNumber == CCombo_Lock::NUMBER::THIRED)
        {
            if (m_eCurrentNumber == CCombo_Lock::NUMBER::THIRED && m_fFrame == 1)
                m_LockUn = true;
            else
                m_LockUn = false;
        }

        if (!m_bCorrect)
        {
            for (_uint i = 0; i < 3; i++)
            {
                CGameObject* p = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Number"), i);

                if (dynamic_cast<CNumber*>(p)->Get_LockUn())
                {
                    m_iCorrect_Cnt++;

                    if (m_iCorrect_Cnt == 3)
                        m_bAllStop = true;
                }
                else
                    m_iCorrect_Cnt = 0;
            }
        }
    }
    else
        m_fFrame = 0.0f;
}

CNumber* CNumber::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CNumber* pInstance = new CNumber(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CNumber"));

        Safe_Release<CNumber*>(pInstance);
    }

    return pInstance;
}

CGameObject* CNumber::Clone(void* pArg)
{
    CNumber* pInstance = new CNumber(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CNumber Clone "));

        Safe_Release<CNumber*>(pInstance);
    }
    return pInstance;
}

CGameObject* CNumber::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
    return nullptr;
}

void CNumber::Free()
{
    Safe_Release<CTexture*>(m_pTextureCom);
    Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

    __super::Free();

}
