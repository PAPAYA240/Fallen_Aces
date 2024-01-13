#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Inventory final : public CUI
{
private:
	CUI_Inventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Inventory(CUI_Inventory& rhs);
	virtual ~CUI_Inventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	// 클래스 내부에서만 사용할 컴포넌트 멤버 변수
	vector<CVIBuffer_Rect*>		m_vecVIBufferCom = { nullptr };
	vector<CTexture*>			m_vecTextureCom = { nullptr };

	vector<class CItem*>*		m_pInven;

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CUI_Inventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END