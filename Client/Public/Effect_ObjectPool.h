#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CEffect_ObjectPool final : public CGameObject
{
private:
	CEffect_ObjectPool(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect_ObjectPool(const CEffect_ObjectPool& rhs);
	virtual ~CEffect_ObjectPool() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public :
	void		New_Object();

private:
	list<CGameObject*>	m_ObjectList;
	_uint				m_iPoolSize;

public:
	static CEffect_ObjectPool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
