//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���V ����
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �N���X�錾
//*****************************************************************************

// �T�E���h�N���X
class CSound
{
public:
	//============
	// ��ԗ�
	//============ 
	typedef enum
	{
		LABEL_BGM_TITLE = 0,   // �^�C�g��BGM
		LABEL_BGM_GAME,        // �Q�[��BGM
		LABEL_SE_ACCELERATION, // ����
		LABEL_SE_ATTACK,       // �U��
		LABEL_SE_ENTER,        // ����
		LABEL_SE_HIT,          // �q�b�g
		LABEL_SE_ITEM,         // �A�C�e��
		SOUND_LABEL_SE_COUNT1, // �X�^�[�g�J�E���g
		SOUND_LABEL_SE_COUNT2, // �X�^�[�g�J�E���g
		SOUND_LABEL_SE_FALL,   // ����
		SOUND_LABEL_MAX,     // �T�E���h�ő吔
	}LABEL;

	//============
	// �����o�֐�
	//============
	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);   // ������
	void Uninit(void);         // �I��
	HRESULT Play(LABEL label); // �Đ�
	void Stop(LABEL label);    // ��~
	void Stop(void);           // �Đ����̉��̂��ׂĒ�~
private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//============
	// �\���̒�`
	//============
	// �p�����[�^
	typedef struct
	{
		char *pFilename; // �t�@�C����
		int nCntLoop;    // ���[�v�J�E���g
	} SOUNDPARAM;

	//============
	// �����o�ϐ�
	//============
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];             // �p�����[�^

	IXAudio2 *m_pXAudio2;                                    // XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;              // �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];   // �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];                    // �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];                     // �I�[�f�B�I�f�[�^�T�C�Y
};
#endif