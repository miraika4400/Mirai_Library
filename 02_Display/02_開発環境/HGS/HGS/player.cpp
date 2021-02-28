//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "player.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "game.h"
#include "joypad.h"
#include "sound.h"
#include "bullet.h"
#include "grid.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_LIFE				(1)			// プレイヤーの体力
#define PLAYER_SPEED			(10.0f)	 	// プレイヤーの速度
#define PLAYER_INERTIA			(0.08f)		// 慣性の大きさ
#define BULLET_INTERVAL			(12)		// 弾の連射間隔

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
bool CPlayer::m_bAlive = true;
//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() :CScene2d(OBJTYPE_PLAYER)
{
	m_nBulletCnt = 0;
	m_nLife = 0;
	m_nStock = 0;
	m_State = PLAYER_STATE_NONE;
	m_StateCount = 0;
	m_Inertia = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bAlive = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Player_Animatio.png",
		&m_pTexture);		// テクスチャへのポインタ

	return S_OK;
}

//=============================================================================
// インスタンス生成
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// メモリ確保
	CPlayer *pPlayer = new CPlayer;

	if (pPlayer != NULL)
	{	
		// 初期化処理
		pPlayer->Init();
		pPlayer->SetPos(pos);		// プレイヤーの位置設定
		pPlayer->SetSize(size);		// サイズの設定
		pPlayer->BindTexture(m_pTexture);	// テクスチャ設定
		pPlayer->InitAnimation(5, 7, -1);
	}
	return pPlayer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// 値の代入
	m_nBulletCnt = 0;			// 弾の間隔初期化
	m_nLife = PLAYER_LIFE;		// 体力

	// テクスチャ受け渡し
	BindTexture(m_pTexture);

	// ポリゴン初期化
	CScene2d::Init();

	m_bAlive = true;
	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CPlayer::UnLoad(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 終了処理
	CScene2d::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// 更新処理
	CScene2d::Update();

	// プレイヤーの状態
//	UpdateState();

	// プレイヤー制御関数
	PlayerControl();

	// 弾の発射
	Shot();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 描画処理
	CScene2d::Draw();
}

//=============================================================================
// プレイヤー制御関数
//=============================================================================
void CPlayer::PlayerControl(void)
{
	// 座標受け取り
	D3DXVECTOR3 pos = GetPos();

	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	if (js.lX != 0.0f || js.lY != 0)
	{
		float fAngle = atan2f((float)js.lX, (float)js.lY);

		// ジョイパッド操作
		m_Inertia.x = sinf(fAngle)* PLAYER_SPEED;
		m_Inertia.y = cosf(fAngle)* PLAYER_SPEED;
	}
	else
	{
		m_Inertia = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_move += (m_Inertia - m_move)*PLAYER_INERTIA;

	pos += m_move;
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// Wキーを押したとき
	if (pKeyboard->GetKeyPress(DIK_W))
	{
		pos.y += cosf(D3DX_PI)*PLAYER_SPEED;
	}
	// Sキーを押したとき
	if (pKeyboard->GetKeyPress(DIK_S))
	{
		pos.y += cosf(0)*PLAYER_SPEED;
	}
	// Aキーを押したとき
	if (pKeyboard->GetKeyPress(DIK_A))
	{
		pos.x -= sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}
	// Dキーを押したとき
	if (pKeyboard->GetKeyPress(DIK_D))
	{
		pos.x += sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}


	// 左判定
	if (pos.x - PLAYER_SIZE_X / 2 < 0.0f)
	{
		pos.x = PLAYER_SIZE_X / 2;
	}
	// 右判定
	if (pos.x + PLAYER_SIZE_X / 2 > SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - PLAYER_SIZE_X / 2;
	}
	// 上判定
	if (pos.y - PLAYER_SIZE_Y / 2 < 0.0f)
	{
		pos.y = PLAYER_SIZE_Y / 2;
	}
	// 下判定
	if (pos.y + PLAYER_SIZE_Y / 2 > SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - PLAYER_SIZE_Y / 2;
	}

	// 座標を渡す
	SetPos(pos);

}

//=============================================================================
// 弾の発射
//=============================================================================
void CPlayer::Shot(void)
{
	// 座標受け取り
	D3DXVECTOR3 pos = GetPos();

	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// バレットの間隔
	m_nBulletCnt++;

	// バレットの間隔
	if (m_nBulletCnt >= BULLET_INTERVAL)
	{
		// Wキーを押したとき
		if (pKeyboard->GetKeyTrigger(DIK_UP) || CManager::GetJoypad()->GetJoystickTrigger(1, 0))
		{
			// 弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(0.0f, -BULLET_SPEED, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// Sキーを押したとき
		else if (pKeyboard->GetKeyTrigger(DIK_DOWN) || CManager::GetJoypad()->GetJoystickTrigger(2, 0))
		{
			// 弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(0.0f, BULLET_SPEED, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// Aキーを押したとき
		else if (pKeyboard->GetKeyTrigger(DIK_LEFT) || CManager::GetJoypad()->GetJoystickTrigger(0, 0))
		{
			// 弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(-BULLET_SPEED, 0.0f, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// Dキーを押したとき
		else if (pKeyboard->GetKeyTrigger(DIK_RIGHT) || CManager::GetJoypad()->GetJoystickTrigger(3, 0))
		{
			// 弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(BULLET_SPEED, 0.0f, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// SE再生
		CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);
	}

}

//=============================================================================
// ダメージを受けたとき
//=============================================================================
void CPlayer::HitDamage(int nCount)
{
	m_nLife -= nCount;

	// 体力が無くなったら
	if (m_nLife <= 0)
	{
		CGrid::BreakAll(GetPos());
		// 生存状態をfalseに
		m_bAlive = false;

		// SE再生
		CManager::GetSound()->Play(CSound::LABEL_SE_GAME_OVER);
		// 終了処理
		Uninit();
		
		// フェード
		//CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		return;
	}
}