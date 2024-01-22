#pragma once

#include "Base.h"

BEGIN(Engine)

class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	~CKey_Manager() = default;

public:
	HRESULT		Initialize();
	void		Update();
	EKeyState Get_KeyState(int _iKey) { return m_eKeyState[_iKey]; }

private:
	EKeyState		m_eKeyState[VK_MAX];
	bool			m_bPrePush[VK_MAX];

public:
	static CKey_Manager* Create();
	virtual void Free() override;
};

END