////////////////////////////////////////////////////
//
//    bombクラスの処理[bomb.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "break_effect.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "score.h"
#include "game.h"
#include "particle.h"
#include "score.h"
#include "game.h"
#include "player.h"

//*****************************
// マクロ定義
//*****************************
#define MAX_SIZE 1500.0f  // 最大サイズ
#define RATE_SIZE 0.03f   // サイズ係数
#define RATE_ALFA 0.05f    // アルファ値係数

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9 CBreakEffect::m_pTexture = NULL; // テクスチャポインタ

//******************************
// コンストラクタ
//******************************
CBreakEffect::CBreakEffect() :CScene(OBJTYPE_PARTICLE)
{

	m_fMaxsize = 0.0f;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAlfa = 1.0f;
}

//******************************
// デストラクタ
//******************************
CBreakEffect::~CBreakEffect()
{
}

//******************************
// クリエイト
//******************************
CBreakEffect * CBreakEffect::Create(const D3DXVECTOR3 pos)
{
	// メモリの確保
	CBreakEffect *pBreak;
	pBreak = new CBreakEffect;
	// 初期化
	pBreak->Init();

	// 各値の代入・セット
	pBreak->m_pos = pos;                     // 座標
	pBreak->SetPriority(OBJTYPE_PARTICLE);   // オブジェクトタイプ

	return pBreak;
}



//******************************
// 初期化処理
//******************************
HRESULT CBreakEffect::Init(void)
{
	// サイズの初期化
	m_size = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
	m_fMaxsize = MAX_SIZE;
	m_fAlfa = 1.0f;
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CBreakEffect::Uninit(void)
{

	Release();
}

//******************************
// 更新処理
//******************************
void CBreakEffect::Update(void)
{

	int nRnd = rand() % 10 + 15;
	for (int nCntparticle = 0; nCntparticle < nRnd; nCntparticle++)
	{
		// ランダムな距離
		float fRandDistance = (float)(rand() % 500 - 250) / 100;
		// ランダムな角度
		int nRandAngle = rand() % 360;

		// ランダムな座標
		D3DXVECTOR3 randPos;
		randPos.x = m_pos.x + cosf(D3DXToRadian(nRandAngle))*(m_size.x / 2 + fRandDistance);
		randPos.y = m_pos.y + sinf(D3DXToRadian(nRandAngle))*(m_size.y / 2 + fRandDistance);
		randPos.z = 0.0f;


		// ランダムなサイズ
		int nRandSize = rand() % 7 + 20;
		// パーティクルの生成
		CParticle::Create(randPos,
			D3DXVECTOR3((float)(rand() % 3 - 1),
			(float)(rand() % 3 - 1), 0.0f),
			D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
			rand() % 5 + 1,
			D3DXCOLOR(1.0f,(float)(rand() % 100) / 100.0f, 0.5f,  m_fAlfa))->SetAddMode(false);
	}

	nRnd = rand() % 10 + 20;
	for (int nCntparticle = 0; nCntparticle < nRnd; nCntparticle++)
	{
		// ランダムな距離
		float fRandDistance = (float)(rand() % 500 - 250) / 100;
		// ランダムな角度
		int nRandAngle = rand() % 360;

		// ランダムな座標
		D3DXVECTOR3 randPos;
		randPos.x = m_pos.x + cosf(D3DXToRadian(nRandAngle))*( m_fMaxsize/4 - m_size.x/4  + fRandDistance);
		randPos.y = m_pos.y + sinf(D3DXToRadian(nRandAngle))*( m_fMaxsize/4 - m_size.y/4  + fRandDistance);
		randPos.z = 0.0f;

		// ランダムなサイズ
		int nRandSize = rand() % 7 + 15;
		// パーティクルの生成
		CParticle::Create(randPos,
			D3DXVECTOR3((float)(rand() % 3 - 1),(float)(rand() % 3 - 1), 0.0f),
			D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
			rand() % 5 + 1,
			D3DXCOLOR(1.0f, (float)(rand() % 100) / 100.0f, 0.5f, m_fAlfa))->SetAddMode(false);
	}

	


	m_fAlfa += ((0.0f) - m_fAlfa)*RATE_ALFA;

	m_size.x += ((m_fMaxsize + 3.0f) - m_size.x)*RATE_SIZE;
	m_size.y += ((m_fMaxsize + 3.0f) - m_size.y)*RATE_SIZE;

	// 最大サイズに到達したら消す
	if (m_size.x > m_fMaxsize)
	{
		Uninit();
		return;
	}

}

//******************************
// 描画処理
//******************************
void CBreakEffect::Draw(void)
{

}

