////////////////////////////////////////////////////
//
//    FeverLogoの処理[FeverLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "goal_ui.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"

//**********************************
// マクロ定義
//**********************************
#define TEXTURE_PATH "./data/Textures/Goal.png"    // テクスチャのパス
#define SIZE D3DXVECTOR3( 960.0f / 2.5f, 276.0f/2.5f, 0.0f)   // サイズ*画像の解像度に合わせる
#define COL D3DXCOLOR(1.0f, 1.0f, 0.6f, 1.0f)                    // カラー

#define MAX_ANIMATION_X 19                                    // アニメーション数 横
#define MAX_ANIMATION_Y 1                                     // アニメーション数 縦
#define ANIMATION_COUNT 3                                     // アニメーションの速度*低い方が早い

// 位置
#define POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/2                   , 0.0f)  // 画面分割してないとき
#define POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4                   , 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4                   , 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4                   , 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // プレイヤー3
#define POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // プレイヤー4

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CGoalUi::m_pTexture = NULL;

//=============================
// コンストラクタ
//=============================
CGoalUi::CGoalUi() :CScene2d(OBJTYPE_UI)
{
	// 変数のクリア
	m_nPlayerNum = 0; // プレイヤー番号
	m_nCntAnim = 0;           // アニメーションカウント
	m_nAnimX = 0;             // アニメーションX軸
	m_nAnimY = 0;             // アニメーションY軸
}

//=============================
// デストラクタ
//=============================
CGoalUi::~CGoalUi()
{
}

//=============================
// クリエイト
//=============================
CGoalUi * CGoalUi::Create(int nPlayerNum)
{
	// メモリの確保
	CGoalUi *pRank = new CGoalUi;
	// プレイヤー番号の代入
	pRank->m_nPlayerNum = nPlayerNum;
	// 初期化
	pRank->Init();
	return pRank;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CGoalUi::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CGoalUi::Unload(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================
// 初期化処理
//=============================
HRESULT CGoalUi::Init(void)
{
	CScene2d::Init();
	// テクスチャの設定
	BindTexture(m_pTexture);

	// 座標の設定

	// プレイヤー人数の取得
	int nNumPlayer = CGame::GetPlayerNum();
	// プレイヤー番号によって分岐
	switch (m_nPlayerNum)
	{
	case 0:
		// プレイヤー1

		if (nNumPlayer == 1)
		{// 分割なし
			SetPos(POS_PLAYER1_1);
		}
		else if (nNumPlayer == 2)
		{// 二分割
			SetPos(POS_PLAYER1_2);
		}
		else
		{// 四分割
			SetPos(POS_PLAYER1_4);
		}
		break;
	case 1:
		// プレイヤー2

		if (nNumPlayer == 2)
		{// 二分割
			SetPos(POS_PLAYER2_2);
		}
		else
		{// 四分割
			SetPos(POS_PLAYER2_4);
		}
		break;
	case 2:
		// プレイヤー3
		SetPos(POS_PLAYER3);
		break;
	case 3:
		// プレイヤー4
		SetPos(POS_PLAYER4);
		break;
	default:
		break;
	}

	if (nNumPlayer <= 2)
	{
		// サイズの設定
		SetSize(SIZE);
	}
	else
	{
		// サイズの設定
		SetSize(SIZE / 2);
	}

	// テクスチャアニメーションの初期化
	m_nAnimX = 0;     // アニメーションX軸
	m_nAnimY = 0;     // アニメーションY軸
	m_nCntAnim = 0;   // アニメーションカウント

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

	// 色の設定
	SetColor(COL);

	return S_OK;
}

//=============================
// 終了処理
//=============================
void CGoalUi::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// 更新処理
//=============================
void CGoalUi::Update(void)
{
	// テクスチャアニメーションの処理
	if (m_nAnimX < MAX_ANIMATION_X-1)
	{
		// カウントを進める
		m_nCntAnim++;
		if (m_nCntAnim % ANIMATION_COUNT == 0)
		{// 一定のカウントでX軸のアニメーションを進める
			m_nAnimX++;
		}

		// UV座標の更新
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / MAX_ANIMATION_X;
		float fv = 1.0f / MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);
		// UV座標セット
		SetTextureUV(uv);
	}


	if (CGame::GetState() == CGame::GAME_RESULT)
	{// リザルト状態の時
	 // UIを消す
		Uninit();
	}
}


//=============================
// 描画処理
//=============================
void CGoalUi::Draw(void)
{
	CScene2d::Draw();
}