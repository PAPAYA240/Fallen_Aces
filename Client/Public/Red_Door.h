#pragma once

#include "Client_Defines.h"
#include "DoorObj.h"

class CRed_Door final : public CDoorObj
{
private:
	CRed_Door(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CRed_Door(const CRed_Door& rhs);
	virtual ~CRed_Door() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_MyComponents();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

	virtual	void		UnLockingControl(_float fTimeDelta) override;
public:
	static CRed_Door* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection);
	virtual void Free() override;
};

