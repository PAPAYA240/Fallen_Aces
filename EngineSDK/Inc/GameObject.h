#pragma once

#include "Transform.h"

/* 클라이언트 개발자가 만들어야할 게임오브젝트들(플레이어몬스터)의 부모가되는 클래스다. */
/* 객체들을 만들 때 Prototype이라는 디자인패턴으 ㄹ활용하여 객체를 생성한다. */
/* Prototype : 원형. */
/* 로딩간에 다음레벨에 필요한 객체의 원형을 미리 생성한다. */
/* 로딩이 끝나고 실제 사용하고자하는 레벨에 진입하게되면 레벨에 필요한 실제 객체를 Create(x), Clone(o)만든다. */



BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjDesc: public CTransform::TRANSFORM_DESC
	{		
	}GAMEOBJECT_DESC;

protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const wstring& strComTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9					m_pGraphic_Device = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };
	class CTransform*					m_pTransformCom = { nullptr };

	CAMERA_DIR							m_eBodyDraw = { CAMERA_DIR::_END };

protected:
	map<const wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, class CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END