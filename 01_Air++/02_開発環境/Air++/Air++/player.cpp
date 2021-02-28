//=============================================================================
//
// playerヘッダ [player.cpp]
// Author : 増澤 未来
//
//=============================================================================

//******************************
// インクルード
//******************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "rank_ui.h"
#include "course.h"
#include "item_ui.h"
#include "mouse.h"
#include "collision.h"
#include "destination.h"
#include "joypad.h"
#include "lap_ui.h"
#include "goal_ui.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH     "./data/Models/player.x"     // モデルのパス
#define SHADER_PATH    "./data/HLSL/Shader.fx"      // HLSLファイルのパス
#define CUBE_TEX_PATH  "./data/Textures/cube_sky.dds" // キューブテクスチャのパス

#define VIEW_MOVE_SPEED_MAX 0.03f                   // マウス移動最大値
#define DRIFT_STICK_TIP 10                          // スティックがどれくらい傾いていたらドリフトになるか
#define DRIFT_ROT_SPEED 0.015f                      // ドリフト時にプラスする値
#define LOCUS_DISTANCE 80.0f                        // 軌跡を出す距離
#define LOCUS_ADJUST 10.0f                          // 軌跡を出す高さの調整
#define LOCUS_COL D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f) // 奇跡カラー

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model   CPlayer::m_model = {};  //メッシュ情報へのポインタ
CScene::Shader CPlayer::m_shader = {};  // シェーダー構造体

//******************************
// コンストラクタ
//******************************
CPlayer::CPlayer() :CCharacter(OBJTYPE_PLAYER)
{
	// 変数のクリア
	m_nPlayerNum = -1;  // プレイヤーID
	m_fDir = 0.0f;
	m_bDriftLeft  = false;      // ドリフト左
	m_bDriftRight = false;      // ドリフト右
}

//******************************
// デストラクタ
//******************************
CPlayer::~CPlayer()
{
}

//******************************
// クリエイト
//******************************
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum)
{
	// メモリの確保
	CPlayer *pPlayer;
	pPlayer = new CPlayer;

	// プレイヤー番号の設定
	pPlayer->m_nPlayerNum = nPlayerNum; 

	// 初期化
	pPlayer->Init();

	// 各値の代入・セット
	pPlayer->SetPos(pos); // 座標のセット
	pPlayer->SetRot(rot); // 回転のセット
	pPlayer->GetCollision()->SetPos(pos); // 当たり判定の位置の設定

	// 移動目標クラスの生成
	pPlayer->m_pDest = CDestination::Create(pPlayer->m_nPlayerNum, pos);

	return pPlayer;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_model.pBuffMat, NULL, &m_model.nNumMat, &m_model.pMesh);

	// テクスチャ読み込み
	if (m_model.nNumMat != 0)
	{
		D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
		for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
		{
			if (pMat[nCnt].pTextureFilename != NULL)
			{
				char cPath[64] = {};
				sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
				// テクスチャの生成
				D3DXCreateTextureFromFile(pDevice, cPath, &m_model.apTexture[nCnt]);
			}
		}
	}

	// HLSLファイルの読み込み
	D3DXCreateEffectFromFile(pDevice, SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	D3DXCreateCubeTextureFromFile(pDevice, CUBE_TEX_PATH, &m_shader.pCubeTex);
	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CPlayer::Unload(void)
{
	//メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	//マテリアルの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}
}

//******************************
// プレイヤーの取得処理
//******************************
CPlayer * CPlayer::GetPlayer(int nPlayerNum)
{
	// return用
	CPlayer*pOut = NULL;
	// プレイヤーリストの取得
	pOut = (CPlayer*)GetTop(OBJTYPE_PLAYER);

	while(pOut != NULL)
	{
		if (pOut->GetPlayerNum()==nPlayerNum)
		{// 引数とプレイヤー番号が一致しているときにポインタを返す
			return pOut;
		}

		// リストのネクストにする
		pOut = (CPlayer*)pOut->GetNext();
	}
	return pOut;
}

//******************************
// 初期化処理
//******************************
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	// モデル割り当て
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);

	//テクスチャの割り当て
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}

	// シェーダーのセット
	SetShader(m_shader);

	// 順位UIの生成
	CRankUi::Create(m_nPlayerNum);
	// アイテムUIの生成
	CItemUi::Create(m_nPlayerNum);
	// ラップ数UIの生成
	CLapUi::Create(m_nPlayerNum);

	// プレイヤーの正面への角度
	m_fDir = GetRot().y + D3DXToRadian(90);

	// ドリフト初期化
	m_bDriftLeft = false;   // ドリフト左
	m_bDriftRight = false;  // ドリフト右

	// 奇跡の情報
	LocusData locusData = { LOCUS_DISTANCE, LOCUS_ADJUST, LOCUS_COL };
	SetLocusData(locusData);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CPlayer::Uninit(void)
{
	// 移動目標クラスの終了処理
	if (m_pDest != NULL)
	{
		m_pDest->Uninit();
		m_pDest = NULL;
	}

	CCharacter::Uninit();
}
	

//******************************
// 更新処理
//******************************
void CPlayer::Update(void)
{
	//m_pDest->Update();

	// 移動不能時意外に処理したいもの
	if (!GetGoalFlag()&& GetMoveFlag() && !GetStan())
	{
		// 移動量の設定
		//Move();

		// ドリフト処理
		Drift();

		// アイテム使用
		UseItem();

		// プレイヤーの正面への角度の保存
		m_fDir = (-GetRot().y) + D3DXToRadian(90);
	}
	else
	{
		// 移動量のセット
		SetMoveDist(VEC3_ZERO);
	}

	// キャラクタークラスの更新処理*移動処理・向きの処理・重力処理・当たり判定
	CCharacter::Update();
}

//******************************
// 描画処理
//******************************
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

void CPlayer::GoalAction(void)
{
	CCharacter::GoalAction();
	
	// ゴールUIの生成
	CGoalUi::Create(m_nPlayerNum);
}

//******************************
// 移動量の設定処理
//******************************
void CPlayer::Move(void)
{
	// 移動方向ベクトル
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
#if 0
	// 移動方向ベクトル
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W)||GetActiveAcceleration())
	{
		//// 座標の取得
		//D3DXVECTOR3 pos = GetPos();
		//// カメラの位置の取得
		//D3DXVECTOR3 cameraPos = CGame::GetCamera(m_nPlayerNum)->GetPos();
		//
		//// カメラから自分の方向ベクトル
		//move = pos - cameraPos;
		//// Y方向には移動しない
		//move.y = 0.0f;
		//// 正規化
		//D3DXVec3Normalize(&move, &move);

		// 移動方向
		move.x = cosf(m_fDir);
		move.y = 0.0f;
		move.z = sinf(m_fDir);
		// スピードをかける
		move *= GetSpeed();
	}
	
	// 移動量のセット
	SetMoveDist(move);
#else
	// 移動量のセット
	SetMoveDist(move);
	SetMove(move);
#endif
}

//******************************
// アイテム使用の処理
//******************************
void CPlayer::UseItem(void)
{
	if (GetItem() != CItem::ITEM_NONE && CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE) || 
		GetItem() != CItem::ITEM_NONE && CManager::GetJoypad()->GetJoystickPress(4, m_nPlayerNum) ||
		GetItem() != CItem::ITEM_NONE && CManager::GetMouse()->GetMousePress(0))
	{
		// アイテムセット
		SetItemObject();
	}
}

//******************************
// 向きの管理*characterクラスからオーバーライド
//******************************
void CPlayer::Direction(void)
{
	D3DXVECTOR3 rot = GetRot();
	if (GetMoveFlag() && !GetStan())
	{// 移動フラグがtrueかつスタン状態なじゃいとき
		// マウスで視点操作
		rot.y += min(max(CManager::GetMouse()->GetMouseMove().x, -VIEW_MOVE_SPEED_MAX), VIEW_MOVE_SPEED_MAX);
	}
	// X軸

	if (CGame::GetCourse() != NULL)
	{
		// レイを出す位置
		D3DXVECTOR3 rayPos = VEC3_ZERO;
		rayPos.x = GetPos().x + (cosf(m_fDir)*RAY_DISTANCE);
		rayPos.y = GetPos().y + RAY_HEIGHT;
		rayPos.z = GetPos().z + (sinf(m_fDir)*RAY_DISTANCE);

		// コースメッシュの取得
		LPD3DXMESH pCourseMesh = CGame::GetCourse()->GetMesh();
		// 
		BOOL bHit = false;
		float fHitDistance;

		// レイによる当たり判定
		D3DXIntersect(pCourseMesh,
			&rayPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fHitDistance,
			NULL,
			NULL);

		if (bHit)
		{// 当たっていた時

			if (fHitDistance <= CHARACTER_ROT_X_ADJUST_RANGE)
			{
				// 当たっている座標
				D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				hitPos.x = rayPos.x;
				hitPos.y = rayPos.y - fHitDistance;
				hitPos.z = rayPos.z;

				// X軸の目標値
				float fRotDistX = atan2f((-(hitPos.y - (GetPos().y - HOVER_HEIGHT))), RAY_DISTANCE);

				// 徐々に目標値に近づける
				rot.x += (fRotDistX - rot.x) * CHARACTER_DIRECTION_RATE;
				
			}
		}
	}

	// 向きのセット
	SetRot(rot);
}

//******************************
// ドリフト判定
//******************************
void CPlayer::Drift(void)
{
	if (!m_bDriftLeft && !m_bDriftRight)
	{// ドリフトが左右どちらもfalse状態の時

		// キーボード
		if (CManager::GetKeyboard()->GetKeyPress(DIK_LSHIFT))
		{
			if (CManager::GetMouse()->GetMouseMove().x > 0)
			{// 右ドリフト
				m_bDriftRight = true;

			}
			else if (CManager::GetMouse()->GetMouseMove().x < 0)
			{// 左ドリフト
				m_bDriftLeft = true;
			}
		}
		// コントローラー
		if (CManager::GetJoypad()->GetJoystickPress(5, m_nPlayerNum))
		{
			if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX >= DRIFT_STICK_TIP)
			{// 右ドリフト
				m_bDriftRight = true;
			}
			else if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX <= -DRIFT_STICK_TIP)
			{// 左ドリフト
				m_bDriftLeft = true;
			}
		}
	}
	else
	{// どっちかのドリフトのフラグが立っていた時

		// 現在の角度の取得
		D3DXVECTOR3 rot = GetRot();

		// 角度の加算
		if (m_bDriftLeft)
		{// 左にドリフト
			rot.y -= DRIFT_ROT_SPEED;
		}
		else if (m_bDriftRight)
		{// 右にドリフト
			rot.y += DRIFT_ROT_SPEED;
		}

		// 角度のセット
		SetRot(rot);

		// ドリフト状態の解除
		if (!CManager::GetJoypad()->GetJoystickPress(5, m_nPlayerNum) && !CManager::GetKeyboard()->GetKeyPress(DIK_LSHIFT))
		{
			m_bDriftLeft = false;
			m_bDriftRight = false;
		}
	}
}

//******************************
// 落下時のアクション
//******************************
void CPlayer::FallAction(void)
{

	m_pDest->Init();
	m_pDest->SetMove(VEC3_ZERO);

	m_bDriftLeft = false;
	m_bDriftRight = false;

	// SE再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FALL);
}
