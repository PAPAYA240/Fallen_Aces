#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

/* 지형을 타고 움직이는 개체들의 부모클래스 */
/* 지형의 높이를 구해서 태우는 기능. */

BEGIN(Engine)
class CTransform;
class CVIBuffer_Terrain;
class CCollider;
END

BEGIN(Client)

class CLandObject abstract : public CGameObject
{
public:
	enum COLLISION_EVENT
	{
		EV_OVERLAP, EV_END
	};

	typedef struct tagCollision
	{
		CLandObject*		pCollisionObject;
		COLLISION_EVENT		eEvent;
		_float3				vOverlap;
	}COLLISION;

public:
	typedef struct tagLandObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		CTransform*			pTerrainTransform;
		CVIBuffer_Terrain*	pTerrainVIBuffer;
	}LANDOBJECT_DESC;

protected:
	CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Add_Collision(CLandObject* pCollisionObject, COLLISION_EVENT eEvent, const _float3& vOverlap) {
		if (nullptr == pCollisionObject)
			return E_FAIL;

		COLLISION eCollision = {};
		eCollision.pCollisionObject = pCollisionObject;
		eCollision.eEvent = eEvent;
		eCollision.vOverlap = vOverlap;

		m_Collisions.push_back(eCollision);

		Safe_AddRef(pCollisionObject);

		return S_OK;
	}
	//	각자 내부에서 콜라이더 오브젝트에 따라 다른방식으로 검사할것이므로 오버라이딩해서 사용하기
	virtual CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr) = 0;


protected:
	CTransform*				m_pTerrainTransform = { nullptr };
	CVIBuffer_Terrain*		m_pTerrainVIBuffer = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };

	list<COLLISION>			m_Collisions;

protected:
	HRESULT SetUp_OnTerrain(_float fRevision = 0.f);

public:	
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END