//=============================================================================
//
// エネミーの処理　[enemy.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "player.h"
#include "score.h"
#include "game.h"
#include "sound.h"
#include "grid.h"
#include "break_effect.h"
#include "collision.h"
#include "tutorial.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMY_NORMAL_LIFE			(4)					// 通常のエネミーのライフ
#define ENEMY_SCORE					(200)				// エネミーのスコア
#define ENEMY_SPEED					(2.0f)				// エネミーの速さ
#define ENEMY_LIFE					(50)				// エネミーの体力
#define ENEMY_DAMAGE				(200)				// エネミーの攻撃力
#define ENEMY_SPEED_UP				(3)					// エネミーのスピードアップ
#define ENEMY_SPEED_DOWN			(1)					// エネミーのスピードダウン

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[MAX_ENEMY_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy():CScene2d(OBJTYPE_ENEMY)
{
	m_nLife = 0;
	m_nSpeed = 0;
	m_fRasian = 0.0f;
	m_fDiagonal = 0.0f;
	m_fAngle = 0.0f;

}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// インスタンス生成
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化処理
	CEnemy *pEnemy = new CEnemy;
	if (pEnemy != NULL)
	{
		// 初期化処理
		pEnemy->Init();							// 初期化情報
		pEnemy->SetPos(pos);
		pEnemy->SetSize(size);					// サイズ設定
		pEnemy->m_nLife = ENEMY_LIFE;			// 体力
		pEnemy->BindTexture(m_apTexture[0]);	// テクスチャ設定
		pEnemy->InitAnimation(5, 7, -1);
	}

	return pEnemy;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Enemy_Animation.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CEnemy::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_ENEMY_TEXTURE; nCount++)
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
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init()
{
	// 初期化処理
	CScene2d::Init();

	CGame *pGame = CManager::GetGame();

	if (pGame != NULL)
	{
		if (pGame->GetSpeedUp() == true)
		{
			m_nSpeed = ENEMY_SPEED + rand() % ENEMY_SPEED_UP - rand() % ENEMY_SPEED_DOWN;
		}
		else
		{
			m_nSpeed = ENEMY_SPEED;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2d::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// 2Dポリゴン更新処理
	CScene2d::Update();

	// 座標更新
	D3DXVECTOR3 pos = GetPos();

	// 現在のモード受け取り
	CManager::MODE mode = CManager::GetMode();

	// 変数宣言
	CPlayer *pPlayer = NULL;

	if(CPlayer::GetAlive())
	{
		switch (mode)
		{
		case CManager::MODE_GAME:
			// プレイヤー情報を取得
			pPlayer = CGame::GetPlayer();
			break;

		case CManager::MODE_TUTORIAL:
			// プレイヤー情報を取得
			pPlayer = CTutorial::GetPlayer();
			break;

		}
	}
	if (mode == CManager::MODE_GAME)
	{
		if (pPlayer != NULL)
		{

			D3DXVECTOR3 Ppos = pPlayer->GetPos();

			//自機を取得する
			float fPposx = Ppos.x, fPposy = Ppos.y;		// 自機の座標
			float fEposx = pos.x, fEposy = pos.y;	// 敵の座標
			float fAngle;								// 角度

														//角度を決める
			fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));
			// 透明な敵の移動
			D3DXVECTOR3 FollowMove = D3DXVECTOR3(
				(sinf(fAngle)*-ENEMY_SPEED),
				cosf(fAngle)*-ENEMY_SPEED, 0.0f);

			// 移動量の設定
			pos += FollowMove;

			// 座標を渡す
			SetPos(pos);

			// 回転
			Rotation();

			if (CollisionCircularAndCircular(pos, Ppos, ENEMY_SIZE_X, DECISION_PLAYER_SIZE_X) == true)
			{
				// 敵にダメージ
				pPlayer->HitDamage(ENEMY_DAMAGE);
			}
		}
	}
	// ライフが0だったら
	if (m_nLife <= 0)
	{
		// エネミー削除
		Uninit();
		CGrid::Break(GetPos());
		CBreakEffect::Create(GetPos());
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// 2Dポリゴン描画処理
	CScene2d::Draw();
}

//=============================================================================
// エネミーヒット時
//=============================================================================
void CEnemy::HitDamage(int nCount)
{
	m_nLife -= nCount;
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CEnemy::Collision(void)
{
	// 先頭情報を取得
	CScene *pScene = CScene::GetTop(OBJTYPE_PLAYER);

	// NULLになるまで繰り返す
	while (pScene)
	{
		// 座標取得
		D3DXVECTOR3 pos = GetPos();

		// キャスト
		CPlayer *pPlayer = (CPlayer*)pScene;

		if (pPlayer != NULL)
		{
			//位置の取得
			D3DXVECTOR3 targetPos = pPlayer->GetPos();

			// 当たり判定
			if (
				targetPos.x - ENEMY_SIZE_X / 2 <pos.x + BULLET_SIZE_X &&
				targetPos.x + ENEMY_SIZE_X / 2 >pos.x - BULLET_SIZE_X &&
				targetPos.y - ENEMY_SIZE_Y / 2 <pos.y + BULLET_SIZE_Y &&
				targetPos.y + ENEMY_SIZE_Y / 2 >pos.y - BULLET_SIZE_Y)
			{
				// 弾を消す
		//		m_nLife = 0;

				// 敵にダメージ
				pPlayer->HitDamage(ENEMY_DAMAGE);
				return true;
			}
		}

		//次の情報を取得
		pScene = pScene->GetNext();
	}

	return false;
}

void CEnemy::Rotation(void)
{
	// 座標更新
	D3DXVECTOR3 pos = GetPos();

	// 現在のモード受け取り
	CManager::MODE mode = CManager::GetMode();

	// 変数宣言
	CPlayer *pPlayer = NULL;

	switch (mode)
	{
	case CManager::MODE_GAME:
		// プレイヤー情報を取得
		pPlayer = CGame::GetPlayer();
		break;

	case CManager::MODE_TUTORIAL:
		// プレイヤー情報を取得
		pPlayer = CTutorial::GetPlayer();
		break;

	}
	if (pPlayer != NULL)
	{

		D3DXVECTOR3 Ppos = pPlayer->GetPos();

		//自機を取得する
		float fPposx = Ppos.x, fPposy = Ppos.y;		// 自機の座標
		float fEposx = pos.x, fEposy = pos.y;	// 敵の座標
		float fAngle;								// 角度

		//角度を決める
		fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));

		// アングルの設定
		SetAngle(D3DXToDegree(-fAngle));
		SetPos(pos);
	}
}