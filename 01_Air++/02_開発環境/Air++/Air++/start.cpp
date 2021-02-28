////////////////////////////////////////////////////
//
//    Startクラスの処理[Start.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "Start.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "sound.h"

#include <time.h>

//*****************************
// マクロ定義
//*****************************
//#define POS   D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f) // 座標
#define POS   D3DXVECTOR3(0.0f,200.0f,-500.0f) // 座標
#define SIZE  D3DXVECTOR3(100.0f,100.0f,0.0f) 

#define MAX_ANIMATION_X 4      // アニメーション数 横
#define MAX_ANIMATION_Y 1      // アニメーション数 縦

#define TEXTURE_PATH "./data/Textures/start.png"    //テクスチャのパス

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9  CStart::m_pTexture = NULL; // テクスチャポインタ

//******************************
// コンストラクタ
//******************************
CStart::CStart() :CScene3d(OBJTYPE_UI)
{
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	m_currentTime = 0;
	m_nTime = 2000;
}

//******************************
// デストラクタ
//******************************
CStart::~CStart()
{
}

//******************************
// クリエイト
//******************************
CStart * CStart::Create(void)
{
	// メモリの確保
	CStart *pStart;
	pStart = new CStart;

	// 初期化
	pStart->Init();

	return pStart;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CStart::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CStart::Unload(void)
{
	// テクスチャの解放処理
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CStart::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// コースにっよって位置の分岐
	SetPos(POS);

	// サイズの設定
	SetSize(SIZE);

	// テクスチャ割り当て
	BindTexture(m_pTexture);

	// 開始時間を入れる
	m_currentTime = timeGetTime();

	// テクスチャUV座標の初期化
	m_nAnimX = 0;
	m_nAnimY = 0;

	// UV座標の設定
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV座標セット
	SetTextureUV(uv);

	m_nTime = 2000;
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CStart::Uninit(void)
{
	CScene3d::Uninit();
}

//******************************
// 更新処理
//******************************
void CStart::Update(void)
{
	// アニメーションが最大未満の時
	if (m_nAnimX + 1 < MAX_ANIMATION_X)
	{
		// 今時間の取得
		DWORD nowTime = timeGetTime();

		// 前回記憶した時間との差を算出
		int nTime = nowTime - m_currentTime;

		// 差が1秒（msec）を超えてたら
		if (nTime >= m_nTime)
		{
			// 前回記憶したタイムの更新
			m_currentTime = nowTime;

			// アニメーションを進める
			m_nAnimX++;

			// UV座標の設定
			D3DXVECTOR2 uv[NUM_VERTEX];
			float fu = 1.0f / MAX_ANIMATION_X;
			float fv = 1.0f / MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

			// UV座標セット
			SetTextureUV(uv);

			if (m_nAnimX + 1 != MAX_ANIMATION_X)
			{
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNT1);
			}
		}

		if (m_nTime == 2000 && nTime >= 1000)
		{// 最初のカウントだけ2倍のタイム

		 // 前回記憶したタイムの更新
			m_currentTime = nowTime;
			// 切り替えタイム
			m_nTime = 1000;
			if (m_nAnimX + 1 != MAX_ANIMATION_X)
			{
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNT1);
			}
		}

		if (m_nAnimX + 1 == MAX_ANIMATION_X)
		{
			// SE再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNT2);
			// ゲームBGM再生
			CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

			// 全員移動可能状態にする
			for (int nCntChara = 0; nCntChara < CGame::GetCharacterNum(); nCntChara++)
			{
				CGame::GetCharacter(nCntChara)->SetMoveFlag(true);
			}
		}
	}

	if (CGame::GetState() == CGame::GAME_RESULT)
	{// リザルト状態の時
	 // UIを消す
		Uninit();
	}

}

//******************************
// 描画処理
//******************************
void CStart::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ライトを消す
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	CScene3d::Draw();

	// ライトをつける
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
