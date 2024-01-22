#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

class CCombo_Lock final : public CGameObject
{
public:
	enum NUMBER { FIRST, SECEND, THIRED, _END };
	typedef struct LockDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float3  vPos;
		NUMBER eNum;

		_float	fObj_Distance;
		_float	fDistance;

	}LOCK_DESC;

public:
	CCombo_Lock(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CCombo_Lock(const CCombo_Lock& _rhs);
	virtual ~CCombo_Lock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_bool Get_Access() { return m_bAccess;  }
	_bool Get_ProjOpen() { return m_bProj; }
private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_MyComponents();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

	void	ProjTrans();
	void	Origin_Trans();
private :
	_bool						m_bProj			= { false };
	_bool						m_bAccess		= { false };

	_float4x4					m_ViewMatrix, m_ProjMatrix;
	_float						m_fX, m_fY, m_fSizeX, m_fSizeY;
	
	_float4x4					m_OriginalMatrix;
	_float3						m_vOrthoProjectedPoint;

private :
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };
public:
	static CCombo_Lock* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection);
	virtual void Free() override;
};

