#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

class CPadLock_Broken final : public CGameObject
{
private:
	CPadLock_Broken(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CPadLock_Broken(const CPadLock_Broken& rhs);
	virtual ~CPadLock_Broken() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool	OnDoor() { return m_bDoorOn; }

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_MyComponents();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	//void	UnLockingControl();
	void	UnLockAnimation(_float fTimeDelta);

private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };

private:
	_float		m_Drop_Time = { 0.0f };
	_bool		m_bUnLock = false;
	_bool		m_bDoorOn = false;

public:
	static CPadLock_Broken* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection);
	virtual void Free() override;
};


