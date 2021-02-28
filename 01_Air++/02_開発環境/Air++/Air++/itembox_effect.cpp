////////////////////////////////////////////////////
//
//    itembox_effectクラスの処理[itembox_effect.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "itembox_effect.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "sound.h"

#include <time.h>

//*****************************
// マクロ定義
//*****************************
#define LIFE  50                               // 寿命
#define PARTICLE_NUM 30                        // エフェクトを出す数
#define TEXTURE_PATH "./data/Textures/Item_Box_Shard.png" // テクスチャのパス
#define MOVE_Y_DIST -5                            // 移動量Y軸の目標値
#define MOVE_Y_RATE 0.05f                         // 移動量Y軸の係数
#define MOVE_Y_MIN 1                              // Y軸速度の最小
#define MOVE_Y_AMPLITUDE 10                       // Y軸速度の振幅
#define SPEED_MIN 100                             // 速度の最小
#define SPEED_AMPLITUDE 30                        // 速度の振幅
#define SPEED_ADJUST 10.0f                        // スピード調整値
#define RAND_ANGLE_360 D3DXToRadian(rand() % 360) // 360度ランダムな角度
#define SIZE_MIN 10                               // サイズの最少
#define SIZE_AMPLITUDE 40                         // サイズの振幅
#define ANGLE_AMPLITUDE 60                        // 角度の振幅
#define ADD_ROT_ADJUST 100.0f                     // 回転軸の加算の調整値

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9  CItemBoxEffect::m_pTexture = NULL; // テクスチャポインタ

//******************************
// コンストラクタ
//******************************
CItemBoxEffect::CItemBoxEffect() :CScene3d(OBJTYPE_PARTICLE)
{
	m_fCharaAngle = 0.0f;  // 当たったキャラクターの向き
	m_move   = VEC3_ZERO;  // 移動力
	m_nLife  = 0;          // 寿命
}

//******************************
// デストラクタ
//******************************
CItemBoxEffect::~CItemBoxEffect()
{
}

//******************************
// エフェクトのセット
//******************************
void CItemBoxEffect::SetEffect(D3DXVECTOR3 pos, float fCharaAngle)
{
	for (int nCnt = 0; nCnt < PARTICLE_NUM; nCnt++)
	{
		Create(pos, fCharaAngle);
	}
}

//******************************
// クリエイト
//******************************
CItemBoxEffect * CItemBoxEffect::Create(D3DXVECTOR3 pos, float fCharaAngle)
{
	// メモリの確保
	CItemBoxEffect *pStart;
	pStart = new CItemBoxEffect;

	// 初期化・引数の代入
	pStart->m_fCharaAngle = fCharaAngle;
	pStart->Init();
	pStart->SetPos(pos);

	return pStart;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CItemBoxEffect::Load(void)
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
void CItemBoxEffect::Unload(void)
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
HRESULT CItemBoxEffect::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(m_pTexture);

	// 加算合成にする
	SetAddMode(true);

	// 移動量をランダムに設定
	// 角度を当たったキャラのY軸をもとにランダム
	float fRandAngle = m_fCharaAngle + D3DXToRadian(rand() % ANGLE_AMPLITUDE - ANGLE_AMPLITUDE/2);
	// 飛んでく速度
	float fSpeed = (float)((rand() % SPEED_AMPLITUDE + SPEED_MIN)) / SPEED_ADJUST;
	// 上記の情報から移動量の生成*Y軸は別で乱数生成
	m_move.x = cosf(fRandAngle)*fSpeed;
	m_move.y = (float)(rand() % MOVE_Y_AMPLITUDE + MOVE_Y_MIN);
	m_move.z = sinf(fRandAngle)*fSpeed;
	
	// 角度をランダムに初期化
	D3DXVECTOR3 randRot;
	randRot.x = RAND_ANGLE_360;
	randRot.y = RAND_ANGLE_360;
	randRot.z = RAND_ANGLE_360;
	SetRot(randRot);
	
	// サイズをランダムに設定
	D3DXVECTOR3 randSize;
	randSize.x = (float)(rand() % SIZE_AMPLITUDE + SIZE_MIN);
	randSize.y = (float)(rand() % SIZE_AMPLITUDE + SIZE_MIN);
	randSize.z = 0.0f;
	SetSize(randSize);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CItemBoxEffect::Uninit(void)
{
	CScene3d::Uninit();
}

//******************************
// 更新処理
//******************************
void CItemBoxEffect::Update(void)
{
	if (m_nLife >= 1)
	{
		SetPos(GetPos() + m_move);
		SetRot(GetRot() + m_move / ADD_ROT_ADJUST);

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
void CItemBoxEffect::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// ライトを消す
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	// カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画
	CScene3d::Draw();

	// ライトをつける
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	// カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
