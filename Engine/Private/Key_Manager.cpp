#include "../Public/Key_Manager.h"

CKey_Manager::CKey_Manager()
{
}

HRESULT CKey_Manager::Initialize()
{
    ZeroMemory(m_eKeyState, sizeof(m_eKeyState));
    ZeroMemory(m_bPrePush, sizeof(m_bPrePush));

    return S_OK;
}

void CKey_Manager::Update()
{
	for (int iKey = 0; iKey < VK_MAX; ++iKey)
	{
		if (GetAsyncKeyState(iKey) & 0x8000)
		{
			if (m_bPrePush[iKey])
			{
				m_eKeyState[iKey] = PRESSING;
			}
			else
			{
				m_eKeyState[iKey] = DOWN;
			}
			m_bPrePush[iKey] = true;
		}
		else
		{
			if (m_bPrePush[iKey])
			{
				m_eKeyState[iKey] = UP;
			}
			else
			{
				m_eKeyState[iKey] = NONE;
			}
			m_bPrePush[iKey] = false;
		}
	}
}

CKey_Manager* CKey_Manager::Create()
{
    CKey_Manager* pInstance = new CKey_Manager();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed To Ctrated : Component Manager Class"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CKey_Manager::Free()
{
}
