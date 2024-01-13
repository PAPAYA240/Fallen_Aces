#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMonster abstract : public CLandObject
{
public:
	typedef struct tagMonsterDescription : public CLandObject::LANDOBJECT_DESC
	{
		MONSTER_STATE	pMonster_Start_State;
		_float3			fAttack_Direction;

		_float			fRun_Range;
		_float			fWalk_Range;
		_float			fAttack_Range;

	}MONSTER_DESC;

protected :
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);	
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	void	Choice_WalkRun();
	void	Monster_Radius(_float fTimeDleta);
	void	Monster_Attack();
	void	Player_Follow(_float fDeltaTime);
	void	Walk_Range();

protected : // 멤버 변수
	MONSTER_STATE				m_pMonsterState;
	MONSTER_STATE				m_pPast_MonsterState;

	CTexture*					m_pTextureCom = { nullptr };
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };

	_float						m_fSpeedPerSec = { 0.0f };
	// 상태 값
	_bool						m_bPlayer_Follow = { false };	// 플레이어를 따라다니는 가?
	_bool						m_bFollow_Stop = { false };		// 행동 멈춤
	_bool						m_bAttack_Start = { false };	// 공격 및 대쉬 가능 시작
	_bool						m_bAttack = { false };			// 대쉬 후 공격
	_bool						m_bDash = { false };			// 대수 -> 주먹 공격
	_bool						m_bWalk = { false };
	_bool						m_bTaunt = { false };			// 도발 -> 쫓아가기
	_bool						m_bRun = { false };

	_bool						m_bFirstDash = { false };		// 첫 대쉬 : 반경내에 있을 때만 대쉬
	_bool						m_bOneMotion = { false };
	_bool						m_bDashTime_True = { false };

	//  상태 값 : 범위
	_float						m_fRunRange = { 0 };
	_float						m_fWalkRange = { 0 };


	_uint						Frame_Count = { 0 };

	_float						m_fFrame = { 0.0f };	// 프레임
	_float3						m_fAttack_Radius;		// 공격 간격
	_float3						m_fOrigin_Position;		// Monster 처음 위치
	_float						m_fDistance;

	_uint						m_iHeavy_Count = { 0 };

protected :
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

	void Chase_Player(_float fTimeDelta);
	void SetUp_BillBoard();

protected:
	void TestCamera();
	//virtual void CheckToArrow() = 0;

private:
	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr);

public:
	// static CMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override = 0;

	virtual void Free() override;
};

END