#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CMatches_Monster final : public CMonster
{
private:
	CMatches_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMatches_Monster(const CMatches_Monster& rhs);
	virtual ~CMatches_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	virtual HRESULT Add_Components();

public:
	static CMatches_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};
END

