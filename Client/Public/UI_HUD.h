#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_HUD final : public CUI
{
private:
	CUI_HUD(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_HUD(CUI_HUD& rhs);
	virtual ~CUI_HUD() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	// 클래스 내부에서만 사용할 컴포넌트 멤버 변수
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CUI_HUD* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END