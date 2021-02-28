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
class CPlayer;
class CScore;

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


	//*****************************
	// �����o�֐�
	//*****************************
	CGame();
	~CGame();

	static CGame *Create(void); // �N���X����

	HRESULT Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetGame(void);			// �Q�[���̐ݒ�

	static CCamera *GetCamera(void) { return m_pCamera; }	// �J�����̎擾����
	static CPlayer *GetPlayer(void) { return m_pPlayer; }	// �v���C���[�̏��
	static CScore *GetScore(void) { return m_pScore; }
	bool GetSpeedUp(void) { return m_bSpeedUp; }
private:
	//*****************************
	// �����o�ϐ�
	//*****************************
	static CCamera *m_pCamera;	// �J����
	static CPlayer *m_pPlayer;	// �v���C���[
	static CScore *m_pScore;	// �X�R�A
	int m_nEnemyFlame;			// �G�̏o��t���[��
	int m_nGameFlame;			// �Q�[���̃J�E���^�[
	bool m_bSpeedUp;			// �X�s�[�h�A�b�v
};

#endif