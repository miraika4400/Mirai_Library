//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "game.h"
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_BULLET_ANIME_SPEED	(2)				// アニメーションの速度
#define PLAYER_BULLET_ANIME_COUNTER	(4)				// アニメーションの数
#define CURRENT_BULLET_SPEED_X		(5.0f)			// 追従時の弾の速度
#define COUNTER_BULLET_SPEED_Y		(5.0f)			// 追従時の弾の速度
#define BULLET_DAMAGE				(100)			// 弾の威力

//=============================================================================
// static初期化処理
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[MAX_BULLET_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet() :CScene2d(OBJTYPE_BULLET)
{
	m_nLife = 0;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// インスタンス生成
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, 
	D3DXVECTOR3 size,	int nLife)
{
	// インスタンス生成
	CBullet *pBullet = new CBullet;

	// 初期化処理
	pBullet->Init();			// 初期化情報
	pBullet->SetPos(pos);		// 座標
	pBullet->SetSize(size);		// 移動
	pBullet->m_move = move;		// 移動量
	pBullet->m_nLife = nLife;	// 体力
	pBullet->BindTexture(m_apTexture[0]);
	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	// 初期化処理
	CScene2d::Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2d::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	// 座標受け取り
	D3DXVECTOR3 pos = GetPos();

	// 2Dポリゴン更新処理
	CScene2d::Update();

	// 移動の更新
	pos += m_move;

	// 飛距離のカウンター減算
	m_nLife--;

	// 座標を渡す
	SetPos(pos);

	// 当たり判定
	Collision();

	// ライフが0になったら
	if (m_nLife <= 0)
	{
		// 終了処理
		Uninit();
		return;
	}

	// 画面外判定
	if (pos.x  < 0.0f || pos.x  > SCREEN_WIDTH ||
		pos.y  < 0.0f || pos.y  > SCREEN_HEIGHT)
	{
		// 体力を0にする
		m_nLife = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	// 2Dポリゴン描画処理
	CScene2d::Draw();
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CBullet::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Bullet.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CBullet::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BULLET_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CBullet::Collision(void)
{
	// 先頭情報を取得
	CScene *pScene = CScene::GetTop(OBJTYPE_ENEMY);

	// NULLになるまで繰り返す
	while (pScene)
	{
		// 座標取得
		D3DXVECTOR3 pos = GetPos();

		// キャスト
		CEnemy *pEnemy = (CEnemy*)pScene;

		if (pEnemy != NULL)
		{
			//位置の取得
			D3DXVECTOR3 targetPos = pEnemy->GetPos();

			// 当たり判定
			if (
				targetPos.x - ENEMY_SIZE_X / 2 <pos.x +BULLET_SIZE_X &&
				targetPos.x + ENEMY_SIZE_X / 2 >pos.x -BULLET_SIZE_X &&
				targetPos.y - ENEMY_SIZE_Y / 2 <pos.y +BULLET_SIZE_Y &&
				targetPos.y + ENEMY_SIZE_Y / 2 >pos.y -BULLET_SIZE_Y)
			{
				// 弾を消す
				m_nLife = 0;

				// 敵にダメージ
				pEnemy->HitDamage(BULLET_DAMAGE);

				// スコアの加算
				CScore::AddScore(ENEMY_SCORE_NORMAL);
				return true;
			}
		}

		//次の情報を取得
		pScene = pScene->GetNext();
	}

	return false;
}