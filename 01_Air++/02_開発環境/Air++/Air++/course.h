//=============================================================================
//
// course�w�b�_ [course.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _COURSE_H_
#define _COURSE_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "model_shader.h"

//*****************************
// �}�N����`
//*****************************
#define HOVER_HEIGHT 50   // �L�����N�^�[���ǂꂭ�炢�����Ă��邩

//*****************************
// �O���錾
//*****************************
class CCharacter;

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CCourse : public CModelShader
{
public:
	//==========================
	// ��
	//==========================

	//==========================
	//�����o�֐�
	//==========================
	CCourse();
	~CCourse();

	static CCourse *Create(void); // �N���X����
	static HRESULT Load(void); // ���f���ǂݍ���
	static void Unload(void);  // ���f���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	void CollisionCharacter(CCharacter* pChara);  // �L�����N�^�[�̓����蔻��*���ڌĂяo��

private:
	void CollisionCharacter(void);                // �L�����N�^�[�̓����蔻��*�X�V�ŌĂяo��
	

	//==========================
	//�����o�ϐ�
	//==========================
	static Model m_model;   // ���f���\����
	static Shader m_shader; // �V�F�[�_�[�\����


};
#endif