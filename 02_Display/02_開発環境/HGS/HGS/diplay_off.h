//=================================================
//
// diplay_off�w�b�_[diplay_off.h]
// Author: ���V ����
//
//=================================================

//��d�C���N���[�h�h�~
#ifndef _DISPLAY_OFF_H_
#define _DISPLAY_OFF_H_

//=============================
// �C���N���[�h
//=============================
#include "scene2d.h"
//=============================
// �O���錾
//=============================

//=============================
// �}�N����`
//=============================

//=============================
// �N���X��`
//=============================

//�O���b�h�N���X
class CDisplayOff : public CScene2d
{
public:
	//======================
	// �񋓐錾
	//======================
	//======================

	// �����o�֐�
	//======================
	CDisplayOff();
	~CDisplayOff();
	static CDisplayOff* Create(void);

	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


	//======================
	// �����o�ϐ�
	//======================
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int      m_nCntAnim;       // �A�j���[�V�����J�E���g
	int      m_nAnimX;         // �A�j���[�V����X��
	int      m_nAnimY;         // �A�j���[�V����Y��
};


#endif