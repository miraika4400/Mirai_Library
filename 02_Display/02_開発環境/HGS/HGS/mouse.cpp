#include "mouse.h"


CInputMouse::CInputMouse()
{
	memset(m_aMouseState, 0, sizeof(m_aMouseState));
	memset(m_aMouseStateTrigger, 0, sizeof(m_aMouseStateTrigger));
	memset(m_aMouseStateRelease, 0, sizeof(m_aMouseStateRelease));
}

CInputMouse::~CInputMouse()
{
}

HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�i�L�[�{�[�h�j�̍쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

void CInputMouse::Update(void)
{
	int nCntKey;
	DIMOUSESTATE  ms;
	HRESULT hr;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();

		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &ms);

		//�f�o�C�X����f�[�^���擾
		if (SUCCEEDED(hr))
		{
			for (nCntKey = 0; nCntKey < MAX_MOUSE_BOTTON; nCntKey++)
			{
				//�L�[�g���K�[
				m_aMouseStateTrigger[nCntKey] = (m_aMouseState[nCntKey] ^ ms.rgbButtons[nCntKey]) &  ms.rgbButtons[nCntKey];

				//�L�[�����[�X
				m_aMouseStateRelease[nCntKey] = (m_aMouseState[nCntKey] ^ ms.rgbButtons[nCntKey]) & ~ms.rgbButtons[nCntKey];

				//�L�[�v���X����ۑ�
				m_aMouseState[nCntKey] = ms.rgbButtons[nCntKey];
			}
		}
		else
		{
			m_pDevice->Acquire();
		}
	}
}
bool CInputMouse::GetMousePress(int nKey)
{
	return m_aMouseState[nKey] & 0x80 ? true : false;
}

bool CInputMouse::GetMouseTrigger(int nKey)
{
	return m_aMouseStateTrigger[nKey] & 0x80 ? true : false;
}

bool CInputMouse::GetMouseRelease(int nKey)
{
	return m_aMouseStateRelease[nKey] & 0x80 ? true : false;
}

D3DXVECTOR3 CInputMouse::GetMousePos(void)
{
	// �}�E�X���W���擾����
	POINT p;
	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	return D3DXVECTOR3(p.x, p.y, 0);
}

void CInputMouse::SetMousePos(D3DXVECTOR3 pos)
{
	POINT p;
	p.x = pos.x;
	p.y = pos.y;
	// �N���C�A���g���W���X�N���[�����W�ɕϊ�����
	ClientToScreen(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	SetCursorPos(p.x, p.y);
}
