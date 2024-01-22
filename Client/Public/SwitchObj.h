#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CSwitchObj abstract : public CGameObject
{
public :
	enum SWITCH { ON, OFF, _END};

protected:
	CSwitchObj(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSwitchObj(const CSwitchObj& rhs);
	virtual ~CSwitchObj() = default;

public:
	class CComponent* Get_Component(const wstring& strComTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float _DeltaTime);
	void	Switch_Control();

protected :
	void			Check_Switch();

protected :
	PUZZLE_ID		m_ePuzzleID = { PUZZLE_ID::_END };
	SWITCH			m_bSwitch_Check = { SWITCH::_END };

protected:
	CTexture*		m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END

