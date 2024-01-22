#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

class CCombo_Lock_Obj final : public CGameObject
{
public:
	CCombo_Lock_Obj(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CCombo_Lock_Obj(const CCombo_Lock_Obj& _rhs);
	virtual ~CCombo_Lock_Obj() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_bool	Get_Access() { return m_bAccess; }

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_MyComponents();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

	void	Access_LockObj();
private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };
	_bool			m_bAccess = { false };
public:
	static CCombo_Lock_Obj* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection);
	virtual void Free() override;
};