////////////////////////////////////////////////////
//
//    itembox_effectクラスの処理[itembox_effect.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "stan_effect.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "sound.h"

#include <time.h>

//*****************************
// マクロ定義
//*****************************
#define LIFE  60                                 // 寿命
#define PARTICLE_NUM 10                           // エフェクトを出す数
#define TEXTURE_PATH "./data/Textures/star01.png" // テクスチャのパス
#define MOVE_Y_DIST -20                           // 移動量Y軸の目標値
#define MOVE_Y_RATE 0.05f                         // 移動量Y軸の係数
#define MOVE_Y_MIN 10                             // Y軸速度の最小
#define MOVE_Y_AMPLITUDE 20                       // Y軸速度の振幅
#define SPEED_MIN 50                              // 速度の最小
#define SPEED_AMPLITUDE 50                        // 速度の振幅
#define SPEED_ADJUST 10.0f                        // スピード調整値
#define RAND_ANGLE_360 D3DXToRadian(rand() % 360) // 360度ランダムな角度
#define SIZE_MIN 20                               // サイズの最少
#define SIZE_AMPLITUDE 20                         // サイズの振幅

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9  CStanEffect::m_pTexture = NULL; // テクスチャポインタ

//******************************
// コンストラクタ
//******************************
CStanEffect::CStanEffect() :CBillboard(OBJTYPE_PARTICLE)
{
	m_move = VEC3_ZERO;  // 移動力
	m_nLife = 0;         // 寿命
}

//******************************
// デストラクタ
//******************************
CStanEffect::~CStanEffect()
{
}

//******************************
// エフェクトのセット
//******************************
void CStanEffect::SetEffect(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < PARTICLE_NUM; nCnt++)
	{
		Create(pos);
	}
}

//******************************
// クリエイト
//******************************
CStanEffect * CStanEffect::Create(D3DXVECTOR3 pos)
{
	// メモリの確保
	CStanEffect *pStart;
	pStart = new CStanEffect;

	// 初期化・引数の代入
	pStart->Init();
	pStart->SetPos(pos);

	return pStart;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CStanEffect::Load(void)
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
void CStanEffect::Unload(void)
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
HRESULT CStanEffect::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(m_pTexture);

	// 加算合成にする
	SetAddMode(true);

	// 移動量をランダムに設定

	// 角度を当たったキャラのY軸をもとにランダム
	float fRandAngle = RAND_ANGLE_360;
	// 飛んでく速度
	float fSpeed = (float)((rand() % SPEED_AMPLITUDE + SPEED_MIN)) / SPEED_ADJUST;
	// 上記の情報から移動量の生成*Y軸は別で乱数生成
	m_move.x = cosf(fRandAngle)*fSpeed;
	m_move.y = (float)(rand() % MOVE_Y_AMPLITUDE + MOVE_Y_MIN);
	m_move.z = sinf(fRandAngle)*fSpeed;

	// 角度をランダムに初期化
	SetAngle((float)(rand()%360));

	// サイズをランダムに設定
	float fSize = (float)(rand() % SIZE_AMPLITUDE + SIZE_MIN);
	D3DXVECTOR3 randSize;
	randSize.x = fSize;
	randSize.y = fSize;
	randSize.z = 0.0f;
	SetSize(randSize);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CStanEffect::Uninit(void)
{
	CBillboard::Uninit();
}

//******************************
// 更新処理
//******************************
void CStanEffect::Update(void)
{
	if (m_nLife >= 1)
	{
		SetPos(GetPos() + m_move);
		
		m_move.y += (MOVE_Y_DIST - m_move.y)*MOVE_Y_RATE;
	}
	// 寿命
	m_nLife++;
	if (m_nLife >= LIFE)
	{
		// 消す
		Uninit();
	}
}

//******************************
// 描画処理
//******************************
void CStanEffect::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライトを消す
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// 描画
	CBillboard::Draw();

	// ライトをつける
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
