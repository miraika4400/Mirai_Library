//=============================================================================
//
// game�w�b�_ [game.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _GAME_H_
#define _GAME_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//=============================
// �O���錾
//=============================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CCamera;
class CLight;
class CCharacter;
class CCpuPoint;
class CCheckPoint;
class CItemPoint;
class CCourse;

//*****************************
//�}�N����`
//*****************************
#define MAX_PLAYER_NUM 1                               // �v���C���[�̍ő吔
#define MAX_NPC_NUM 7                                  // NPC�̍ő吔
#define MAX_CHARACTER_NUM MAX_PLAYER_NUM + MAX_NPC_NUM // �L�����N�^�[�̍ő吔(�v���C���[�ő吔 + NPC�ő吔)

//*****************************
//�N���X��`
//*****************************

//2d�|���S���N���X
class CGame : public CScene
{
public:
	//*****************************
	// �񋓒�`
	//*****************************
	typedef enum
	{
		GAME_NORMAL,
		GAME_RESULT,
		GAME_MAX
	}GAME_STATE;

	//*****************************
	// �����o�֐�
	//*****************************
	CGame();
	~CGame();

	static CGame *Create(void); // �N���X����

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	// �擾����
	static CCamera     *GetCamera(int nIndex) { return m_apCamera[nIndex]; }       // �J�����|�C���^
	static CCharacter  *GetCharacter(int nIndex) { return m_apCharacter[nIndex]; } // �L�����N�^�[�|�C���^(�����F�z��ԍ�)
	static CCpuPoint   *GetCpuPoint(void) { return m_pCpuPoint; }                  // CPU�p�R�[�X�|�C���g
	static CCheckPoint *GetCheckPoint(void) { return m_pCheckPoint; }              // �`�F�b�N�|�C���g
	static CCourse     *GetCourse(void) { return m_pCourse; }                      // �`�F�b�N�|�C���g
	static int          GetCharacterNum(void) { return m_nNumCaracter; }           // �L�����N�^�[��
	static int          GetPlayerNum(void) { return m_nNumPlayer; }                // �v���C���[��
	static GAME_STATE   GetState(void) { return m_gameState; }

private:
	void CheckGoal(void); // �v���C���[���S�[�����`�F�b�N���ă��U���g�t���O�𗧂Ă�
	//*****************************
	// �����o�ϐ�
	//*****************************
	static CCamera*m_apCamera[MAX_PLAYER_NUM];            // �J����
	static CLight*m_pLight;                               // ���C�g�|�C���^
	static CCharacter* m_apCharacter[MAX_CHARACTER_NUM];  // �L�����N�^�[�|�C���^(�ő�L���������z��m��)
	static int m_nNumCaracter;                            // ���������L�����N�^�[��
	static int m_nNumPlayer;                              // �v���C���[��
	static CCpuPoint *m_pCpuPoint;                        // CPU�R�[�X�f�[�^
	static CCheckPoint*m_pCheckPoint;                     // �`�F�b�N�|�C���g
	static CItemPoint *m_pItemPoint;                      // �A�C�e�����W�̊Ǘ�
	static CCourse    *m_pCourse;                         // �R�[�X�|�C���^
	static GAME_STATE m_gameState;                        // �Q�[���̏��
	int  m_nCntResult;                                    // ���U���g�J�ڂ܂ł̃J�E���g
	bool m_bResult;                                       // ���U���g�J�ڃt���O
};

#endif