//=============================================================================
//
// rank�w�b�_ [rank.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _RANK_H_
#define _RANK_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//�N���X��`
//*****************************

// �����N�N���X
class CRank : public CScene
{
public:
	//============
	// �����o�֐�
	//============
	CRank();
	~CRank();

	static CRank *Create(); // �N���X����

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	void RankManager(void);   // ���ʊǗ�
	float CheckProgress(int nCheckNum, D3DXVECTOR3 pos); // �i�s�x�̎Z�o

	//============
	// �����o�ϐ�
	//============

};

#endif