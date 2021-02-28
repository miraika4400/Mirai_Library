//=================================================
//
// displayon�w�b�_[displayon.h]
// Author: ���V ����
//
//=================================================

//��d�C���N���[�h�h�~
#ifndef _DISPLAY_ON_H_
#define _DISPLAY_ON_H_

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
class CDisplayON : public CScene2d
{
public:
	//======================
	// �񋓐錾
	//======================
	//======================

	// �����o�֐�
	//======================
	CDisplayON();
	~CDisplayON();
	static CDisplayON* Create(void);

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