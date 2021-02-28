//=============================================================================
//
// characterヘッダ [character.cpp]
// Author : 増澤 未来
//
//=============================================================================

//******************************
// インクルード
//******************************
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "collision.h"
#include "course.h"
#include "checkpoint.h"
#include "cpu_point.h"
#include "player.h"
#include "destination.h"
#include "attack.h"
#include "trap.h"
#include "locus.h"
#include "stan_effect.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define CHARACTER_GRAVITY D3DXVECTOR3(0.0f,-50.0f,0.0f) // 重力量
#define CHARACTER_GRAVITY_RATE 0.03f                    // 重力の係数
//#define CHARACTER_MOVE_RATE 1.0f                      // 移動量の係数
#define CHARACTER_SPEED_BASE 30.0f                      // 移動スピード
#define CHARACTER_SPEED_ACCELERATION 45.0f              // 移動スピード*加速時
#define CHARACTER_ACCELERATION_RATE 0.06f               // 加速時のスピード変化の係数
#define CHARACTER_ACCELERATION_COUNT 80                 // 加速フレーム数
#define CHARACTER_STAN_FRAME 60                         // スタンの長さ(フレーム)
#define LOCUS_DISTANCE_DEFAULT 80.0f                        // 軌跡を出す距離デフォルト
#define LOCUS_ADJUST_DEFAULT 10.0f                          // 軌跡を出す高さの調整デフォルト
#define LOCUS_COL_DEFAULT D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f) // 奇跡カラーデフォルト

//*****************************
// 静的メンバ変数宣言
//*****************************

//******************************
// コンストラクタ
//******************************
CCharacter::CCharacter(int nPliority) :CModelShader(nPliority)
{
	// 変数のクリア
	m_move       = VEC3_ZERO;  // 移動量
	m_bMove      = false;      // 移動フラグ
	m_moveDist   = VEC3_ZERO;  // 移動量の目標値
	m_gravityVec = VEC3_ZERO;  // 重力量
	m_bGravity   = false;       // 重力フラグ
	m_pCollision = NULL;       // コリジョン
	m_rankData   = {};         // 順位付け用データ
	m_bGoal = false;           // ゴールフラグ
	m_impact = VEC3_ZERO;      // 衝突したときの移動量
	m_item = CItem::ITEM_NONE; // 所持しているアイテム
	m_bAcceleration = false;   // 加速フラグ
	m_fSpeed = CHARACTER_SPEED_BASE; // 移動速度
	m_nCntAccleration = 0;     // 加速時のカウント
	m_bStan = false;           // スタンフラグ
	m_nCntStan = 0;            // スタンカウント
	m_nCntGravity = 0;         // 重力フラグが経っている間のカウント
	m_locusData = { LOCUS_DISTANCE_DEFAULT,LOCUS_ADJUST_DEFAULT ,LOCUS_COL_DEFAULT }; // 奇跡情報

	// オブジェクトタイプがプレイヤーの時、プレイヤーフラグをtrueにする
	if (nPliority == OBJTYPE_PLAYER) m_bIsPlayer = true;
	else m_bIsPlayer = false;
}

//******************************
// デストラクタ
//******************************
CCharacter::~CCharacter()
{
}

//******************************
// 初期化処理
//******************************
HRESULT CCharacter::Init(void)
{
	// モデルクラスの初期化処理
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}
	
	// コリジョンの生成
	m_pCollision = CCollision::CreateSphere(GetPos(), CHARACTER_RADIUS);

	// 変数の取得
	m_bMove = false;                 // 移動フラグ
	m_bGoal = false;                 // ゴールフラグ
	m_item = CItem::ITEM_NONE;       // 所持しているアイテム
	m_bAcceleration = false;         // 加速フラグ
	m_fSpeed = CHARACTER_SPEED_BASE; // 移動速度
	m_nCntAccleration = 0;           // 加速時のカウント
	m_bStan = false;                 // スタンフラグ
	m_nCntStan = 0;                  // スタンカウント
	m_bGravity = false;              // 重力フラグ
	m_nCntGravity = 0;               // 重力フラグが経っている間のカウント

	// 順位付けデータの初期化
	m_rankData.nCheck = 0;
	m_rankData.nLap = 0;
	m_rankData.nRank = 1;

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCharacter::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}
	CModelShader::Uninit();
}

//******************************
// 更新処理
//******************************
void CCharacter::Update(void)
{
	// 軌跡用データの保持
	D3DXVECTOR3 posOld = GetPos();
	float fAngeleOld = -GetRot().y;

	if (m_bIsPlayer)
	{// プレイヤーの時
		((CPlayer*)this)->GetDest()->Update();
	}

	// 重力処理
	Gravity();
	
	if (m_bMove)
	{// 移動フラグがtrueの時

		// 向きの処理
		Direction();
		// スピードの管理
		SpeedManager();

		// 移動量*徐々に目標値に近づける
		m_move += (m_moveDist - m_move)*CHARACTER_MOVE_RATE;

		// 座標に移動量を足す
		SetPos(GetPos() + m_move);
	}
	
	// モデルクラスの更新処理
	CModelShader::Update();

	if (m_pCollision != NULL)
	{
		// 当たり判定の位置更新
		m_pCollision->SetPos(GetPos());
	}

	// コースのと当たり判定
	CGame::GetCourse()->CollisionCharacter(this);

	// キャラ同士の当たり判定の処理
	CollisionCharacter();

	// 加速状態時に奇跡を出す

	if (GetActiveAcceleration())
	{// 加速状態の時

		// 軌跡を出す
		// プレイヤーの向いている向きの取得
		float fAngele = -GetRot().y;

		// 頂点座標の計算
		D3DXVECTOR3 aPos[NUM_VERTEX / 2] =
		{
			{ GetPos().x + cosf(fAngele) *  m_locusData.fWidth, GetPos().y + m_locusData.fHeight, GetPos().z + sinf(fAngele) *  m_locusData.fWidth },
			{ GetPos().x + cosf(fAngele) * -m_locusData.fWidth, GetPos().y + m_locusData.fHeight, GetPos().z + sinf(fAngele) * -m_locusData.fWidth },
		};

		D3DXVECTOR3 aPosOld[NUM_VERTEX / 2] =
		{
			{ posOld.x + cosf(fAngeleOld) *  m_locusData.fWidth , posOld.y + m_locusData.fHeight, posOld.z + sinf(fAngeleOld) *  m_locusData.fWidth },
			{ posOld.x + cosf(fAngeleOld) * -m_locusData.fWidth , posOld.y + m_locusData.fHeight, posOld.z + sinf(fAngeleOld) * -m_locusData.fWidth },
		};

		CLocus::Create(aPos, aPosOld, m_locusData.col);
	}

}

//******************************
// 描画処理
//******************************
void CCharacter::Draw(void)
{
	CModelShader::Draw();
}

//******************************
// 加速フラグのセット
//******************************
void CCharacter::SetActiveAcceleration(bool bBool)
{
	// 引数のセット
	m_bAcceleration = bBool;

	if (m_bAcceleration)
	{// 引数がtrueだった時
		// カウントの初期化
		m_nCntAccleration = 0;

		// スタン状態の解除
		m_bStan = false;
	}
}

//******************************
// スタンフラグのセット
//******************************
void CCharacter::SetStan(bool bBool)
{
	// 引数のセット
	m_bStan = bBool;
	
	if (m_bStan)
	{// 引数がtrueだった時

		// カウントの初期化
		m_nCntStan = 0;
		// 加速状態の解除
		m_bAcceleration = false;
		// エフェクト生成
		CStanEffect::SetEffect(GetPos());
	}
}

//******************************
// アイテムのセット処理
//******************************
void CCharacter::SetItemObject(void)
{
	switch (m_item)
	{
	case CItem::ITEM_ACCELERATION:
		// 加速モードにする
		SetActiveAcceleration(true);

		if (m_bIsPlayer)
		{
			// SE再生
			CManager::GetSound()->Play(CSound::LABEL_SE_ACCELERATION);
		}

		break;
	case CItem::ITEM_ATTACK:
	{
		// 頭から出す
		D3DXVECTOR3 attackPos = GetPos();
		// プレイヤーの座標の取得
		float fAngle = (-GetRot().y) + D3DXToRadian(90);
		attackPos.x += cosf(fAngle) * ATTACK_SET_DISTACE;
		attackPos.z += sinf(fAngle) * ATTACK_SET_DISTACE;

		// 攻撃生成
		CAttack::Create(attackPos, D3DXVECTOR3(0.0f, -GetRot().y + D3DXToRadian(90), 0.0f), m_rankData.nRank, GetID());

		if (m_bIsPlayer)
		{
			// SE再生
			CManager::GetSound()->Play(CSound::LABEL_SE_ATTACK);
		}

	}
	break;

	case CItem::ITEM_TRAP:
	{
		// けつから出す
		D3DXVECTOR3 trapPos = GetPos();
		// プレイヤーの後ろ座標の取得
		float fAngle = (-GetRot().y) + D3DXToRadian(-90);
		trapPos.x += cosf(fAngle) * TRAP_SET_DISTACE;
		trapPos.z += sinf(fAngle) * TRAP_SET_DISTACE;

		// トラップ生成
		CTrap::Create(trapPos);

		if (m_bIsPlayer)
		{
			// SE再生
			CManager::GetSound()->Play(CSound::LABEL_SE_ATTACK);
		}
	}
	break;
	default:
		break;
	}
	// アイテム情報の初期化
	SetItem(CItem::ITEM_NONE);
}

//******************************
// 重力の管理
//******************************
void CCharacter::Gravity(void)
{
	if (m_bGravity)
	{// 重力フラグが立ってたら

		// 重力量の設定
		m_gravityVec += (CHARACTER_GRAVITY - m_gravityVec)*CHARACTER_GRAVITY_RATE;

		// 座標に重力量のプラス
		SetPos(GetPos() + m_gravityVec);
		// カウントを進める
		m_nCntGravity++;
		// 一定カウントで落ちた判定
		if (m_nCntGravity >= CHARACTER_FALL_COUNT)
		{
			if (m_rankData.nCheck != 0)
			{
				SetPos(CGame::GetCheckPoint()->GetCollision(m_rankData.nCheck - 1)->GetPos());
			}
			else
			{
				SetPos(CGame::GetCheckPoint()->GetCollision(0)->GetPos());
			}

			// 機体の向きを進行方向に合わせる

			// 一番近い座標の次の座標配列番号の取得
			int nDist = CGame::GetCpuPoint()->GetNearPosIndex(0, GetPos()) + 1;
			// 最大値を越さないようにする
			if (nDist > CGame::GetCpuPoint()->GetPointNum(0)) nDist = 0;
			// 目標位置
			D3DXVECTOR3 distPos = CGame::GetCpuPoint()->GetPointData(0, nDist).pos;

			D3DXVECTOR3 rot = GetRot();
			rot.y = atan2f(distPos.x - GetPos().x, distPos.z - GetPos().z);

			SetRot(rot);

			// 移動量の初期化
			m_move = VEC3_ZERO;
			m_moveDist = VEC3_ZERO;

			// 加速・スタンのフラグ解除
			m_bAcceleration = false;
			SetStan(true);

			// 各派生クラスの落ちたときのアクション
			FallAction();
		}
	}
	else
	{// 重力フラグが立ってないとき
	 // 初期化
		if (m_nCntGravity != 0)m_nCntGravity = 0;

		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// 向きの管理
//******************************
void CCharacter::Direction(void)
{
	// 向きの取得
	D3DXVECTOR3 rot = GetRot();

	// Y軸

	// 移動量が0じゃないとき&&スタン状態じゃないとき
	if (m_move != VEC3_ZERO && !m_bStan)
	{
		
		// Y軸の目標値
		float fRotYDist = atan2f(m_move.x, m_move.z);

		// グルんと回転しないよう調整
		while (rot.y - fRotYDist > D3DXToRadian(180))
		{
			fRotYDist += D3DXToRadian(360);
		}
		while (rot.y - fRotYDist < D3DXToRadian(-180))
		{
			fRotYDist -= D3DXToRadian(360);
		}

		// 徐々に目標値に近づける
		rot.y += (fRotYDist - rot.y) * CHARACTER_DIRECTION_RATE;

		// 向きのセット
		SetRot(rot);
	}

	// X軸

	if (CGame::GetCourse() != NULL)
	{
		// レイを出す位置
		D3DXVECTOR3 rayPos = VEC3_ZERO;
		rayPos.x = GetPos().x + (cosf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);
		rayPos.y = GetPos().y + RAY_HEIGHT;
		rayPos.z = GetPos().z + (sinf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);

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
				D3DXVECTOR3 hitPos = VEC3_ZERO;
				hitPos.x = rayPos.x;
				hitPos.y = rayPos.y - fHitDistance;
				hitPos.z = rayPos.z;

				// X軸の目標値
				float fRotDistX = atan2f((-(hitPos.y - (GetPos().y - HOVER_HEIGHT))), RAY_DISTANCE);

				// 徐々に目標値に近づける
				rot.x += (fRotDistX - rot.x) * CHARACTER_DIRECTION_RATE;
				// 向きのセット
				SetRot(rot);
			}
		}
	}
}

//*****************************************
// キャラクター同士の当たり判定処理
//*****************************************
void CCharacter::CollisionCharacter(void)
{
	// キャラクター数分ループ
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// キャラクター情報の取得
		CCharacter *pTarget = CGame::GetCharacter(nCnt);

		// NULLチェック&自分じゃないとき
		if (pTarget != NULL&&pTarget != this)
		{
			if (CCollision::CollisionSphere(m_pCollision, pTarget->m_pCollision))
			{// 当たっていた時
			 
				// 外に押し出す
				D3DXVECTOR3 vec = (GetPos() - pTarget->GetPos());
				D3DXVec3Normalize(&vec, &vec);
				vec *= (m_pCollision->GetCollisionRadius() + pTarget->GetCollision()->GetCollisionRadius())+10;
				// 座標の更新
				SetPos(pTarget->GetPos() + vec);
			}
		}
	}
}

//*****************************************
// スピード管理処理
//*****************************************
void CCharacter::SpeedManager(void)
{
	if (m_bAcceleration)
	{// 加速

		// 加速状態のスピードに近づける
		m_fSpeed = CHARACTER_SPEED_ACCELERATION;
		
		// カウントを進める
		m_nCntAccleration++;
		if (m_nCntAccleration > CHARACTER_ACCELERATION_COUNT)
		{// 加速カウントが一定に達したら
			// 加速終了
			m_bAcceleration = false;
			// カウントの初期化
			m_nCntAccleration = 0;
		}
	}
	else if (m_bStan)
	{// スタン状態

		// 加速状態のスピードに近づける
		m_fSpeed =0.0f;

		// スタン中機体を回転させる
		D3DXVECTOR3 rot = GetRot();
		rot.y += D3DXToRadian(360 / (CHARACTER_STAN_FRAME));
		SetRot(rot);

		// カウントを進める
		m_nCntStan++;
		if (m_nCntStan > CHARACTER_STAN_FRAME)
		{// 加速カウントが一定に達したら
		 // 加速終了
			m_bStan = false;
			// カウントの初期化
			m_nCntStan = 0;

			if (m_bIsPlayer)
			{//プレイヤーだった時
				//rot.y = ((CPlayer*)this)->GetDir() - D3DXToRadian(90);
				SetRot(rot);
			}
		}
	}
	else
	{
		// 非加速状態のスピードに近づける
		m_fSpeed = CHARACTER_SPEED_BASE;
	}
}
