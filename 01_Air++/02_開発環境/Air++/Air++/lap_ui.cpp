////////////////////////////////////////////////////
//
//    Startクラスの処理[Start.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "lap_ui.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//*****************************
// マクロ定義
//*****************************

// 座標
#define POS   D3DXVECTOR3(1100.0f, 140.0f, 0.0f)       // 座標
#define SIZE  D3DXVECTOR3(604.0f/7, 400.0f/7, 0.0f)   // サイズ*画像サイズに合わせる

#define MAX_ANIMATION_X 3      // アニメーション数 横
#define MAX_ANIMATION_Y 1      // アニメーション数 縦

#define TEXTURE_PATH "./data/Textures/Lap_ui.png"    //テクスチャのパス

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9  CLapUi::m_pTexture = NULL; // テクスチャポインタ

//******************************
// コンストラクタ
//******************************
CLapUi::CLapUi() :CScene2d(OBJTYPE_UI)
{
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	m_nPlayerNum = 0;
}

//******************************
// デストラクタ
//******************************
CLapUi::~CLapUi()
{
}

//******************************
// クリエイト
//******************************
CLapUi * CLapUi::Create(int nPlayerNum)
{
	// メモリの確保
	CLapUi *pStart;
	pStart = new CLapUi;

	// プレイヤー番号の取得
	pStart->m_nPlayerNum = nPlayerNum;
	// 初期化
	pStart->Init();

	return pStart;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CLapUi::Load(void)
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
void CLapUi::Unload(void)
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
HRESULT CLapUi::Init(void)
{
	if (FAILED(CScene2d::Init()))
	{
		return E_FAIL;
	}

	SetPos(POS);
	// サイズの設定
	SetSize(SIZE);

	// テクスチャ割り当て
	BindTexture(m_pTexture);


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

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CLapUi::Uninit(void)
{

	CScene2d::Uninit();
}

//******************************
// 更新処理
//******************************
void CLapUi::Update(void)
{
	// テクスチャUV座標の初期化
	m_nAnimX = CPlayer::GetPlayer(m_nPlayerNum)->GetRankData().nLap;

	if (m_nAnimX > MAX_ANIMATION_X-1)
	{
		m_nAnimX = MAX_ANIMATION_X - 1;
	}
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

	if (CGame::GetState() == CGame::GAME_RESULT)
	{// リザルト時消す
		Uninit();
	}
}

//******************************
// 描画処理
//******************************
void CLapUi::Draw(void)
{
	CScene2d::Draw();
}
