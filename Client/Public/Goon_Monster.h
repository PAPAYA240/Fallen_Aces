#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CGoon_Monster final : public CMonster
{
private:
	CGoon_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGoon_Monster(const CGoon_Monster& rhs);
	virtual ~CGoon_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	virtual HRESULT Add_Components();

private :
	void	Monster_Patton_Ready(_float fTimeDelta);
	void	Change_Texture(MONSTER_STATE _state);

	void	Player_Teasing(); // Player 발견 시 도발 자세
	void	Monster_Walk(_float fTimeDelta);
	void	Monster_Dash(_float fTimeDelta);
	void	Random_DashPos(_uint iPos, _float fTimeDelta);

private : 
	// 상태 값
	_bool		m_bRightWalk	= { false };
	_bool		m_bDash_Start	= { false };	// 대쉬 시작
	_bool		m_bDash_Pos		= { false };	// 대쉬의 움직임 수락
	_bool		m_bRandomTrue	= { false };
	_uint		m_bDash_Count = { 0 };		// 몇 번 대쉬할 것인가?

	_float3		m_vCurrent_Pos;					// 현재 몬스터 위치 저장
	_float		m_vDst_Pos_Distance;			// 도착할 위치 거리 간격
	_uint		m_iRandom_Pos;
	// Time
	_float		m_fTeasing_Time		= { 0.0f }; // 도발 시간
	_float		m_fDash_Time		= { 0.0f }; // 대쉬 시간

	_uint		m_iDash_Count		= { 2 };

public:
	static CGoon_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};
END

