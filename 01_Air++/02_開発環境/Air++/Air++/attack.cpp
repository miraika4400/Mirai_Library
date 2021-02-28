//=============================================================================
//
// attackヘッダ [attack.cpp]
// Author : 増澤 未来
//
//=============================================================================

//******************************
// インクルード
//******************************
#include "attack.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "collision.h"
#include "cpu_point.h"
#include "player.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH   "./data/Models/attack.x" // モデルのパス*ワープスター
#define SHADER_PATH  "./data/HLSL/Shader.fx"  // HLSLファイルのパス
#define ATTACK_SIZE D3DXVECTOR3( 0.5f, 0.5f, 0.5f) // モデルサイズ
#define ATTACK_DIST_RADIUS 100.0f             // 目標の地点半径
#define ATTACK_TARGET_RADIUS 500.0f           // 攻撃対象検索当たり判定
#define DIST_RAND_AMPLITUDE 800               // 目標地点設定の乱数の振幅
#define ATTACK_SPEED 45.0f                    // 移動速度
#define ATTACK_LIFE 1200.0f                   // 寿命

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model  CAttack::m_model = {};  //メッシュ情報へのポインタ
CScene::Shader CAttack::m_shader = {}; // シェーダー構造体

//******************************
// コンストラクタ
//******************************
CAttack::CAttack() :CCharacter(OBJTYPE_ATTACK)
{
	m_nPointNum = 0;           // コースデータ配列管理用
	m_pDIstCollision = NULL;   // 移動目標値当たり判定
	m_nRank = 0;               // 放った人の順位
	m_nLife = 0;               // 寿命
	m_pTargetCollision = NULL; // 攻撃目標当たり判定
	m_nRoot = 0;               // ルート
}

//******************************
// デストラクタ
//******************************
CAttack::~CAttack()
{
}

//******************************
// クリエイト
//******************************
CAttack * CAttack::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,int nRank,int nUseID)
{
	// メモリの確保
	CAttack *pAttack;
	pAttack = new CAttack;

	// 初期化
	pAttack->Init();

	// 各値の代入・セット
	pAttack->SetPos(pos);       // 座標のセット
	pAttack->SetRot(rot);       // 座標のセット
	pAttack->m_nRank = nRank;   // 順位のセット 
	pAttack->m_nUseID = nUseID; // 放った人のオブジェクト番号
	pAttack->GetCollision()->SetPos(pos); // 当たり判定の位置の設定

	// 一番近い地点の次の番号を初期インデックスにする*一番目だと一回逆走しそう*
	pAttack->m_nPointNum = CGame::GetCpuPoint()->GetNearPosIndex(pAttack->m_nRoot,pos) + 1;
	if (CGame::GetCpuPoint()->GetPointNum(pAttack->m_nRoot) < pAttack->m_nPointNum) pAttack->m_nPointNum = 0;
	// 目標地点
	pAttack->m_pDIstCollision = CCollision::CreateSphere(CGame::GetCpuPoint()->GetPointData(pAttack->m_nRoot , pAttack->m_nPointNum).pos, ATTACK_DIST_RADIUS);

	// 攻撃対象検索当たり判定の生成
	pAttack->m_pTargetCollision = CCollision::CreateSphere(pos, ATTACK_TARGET_RADIUS);
	return pAttack;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CAttack::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	// ワープスター
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
	D3DXCreateEffectFromFile(CManager::GetRenderer()->GetDevice(), SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CAttack::Unload(void)
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
// 初期化処理
//******************************
HRESULT CAttack::Init(void)
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
	// コースデータ配列管理用変数の初期化
	m_nPointNum = 0;
	// サイズのセット
	SetSize(ATTACK_SIZE);

	// ルートランダムで設定
	m_nRoot = rand() % ROOT_NUM;

	// 移動フラグ
	SetMoveFlag(true);
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CAttack::Uninit(void)
{
	CCharacter::Uninit();

	if (m_pTargetCollision != NULL)
	{
		m_pTargetCollision->Uninit();
		m_pTargetCollision = NULL;
	}

	if (m_pDIstCollision != NULL)
	{
		m_pDIstCollision->Uninit();
		m_pDIstCollision = NULL;
	}
}

//******************************
// 更新処理
//******************************
void CAttack::Update(void)
{

	// 移動量の設定
	Move();

	// 目標地点の管理
	DistManager();

	if (m_pTargetCollision != NULL)
	{
		// 当たり判定の位置を更新
		m_pTargetCollision->SetPos(GetPos());
	}

	// キャラクタークラスの更新処理*移動、重力、当たり判定、etc.
	CCharacter::Update();
}

//******************************
// 描画処理
//******************************
void CAttack::Draw(void)
{
	CCharacter::Draw();
}

//******************************
// 移動量の設定処理
//******************************
void CAttack::Move(void)
{
	// 移動量の取得
	D3DXVECTOR3 move = GetMove();

	if (m_pDIstCollision != NULL)
	{
		// 現在地点から目標地点へのベクトルの取得
		D3DXVECTOR3 moveDist = m_pDIstCollision->GetPos() - GetPos();
		// Y軸は移動しない
		moveDist.y = 0;
		// 正規化
		D3DXVec3Normalize(&moveDist, &moveDist);
		// スピードをかける
		moveDist *= ATTACK_SPEED;

		// 移動量のセット*係数を無視する
		SetMoveDist(moveDist);
	}

}

//******************************
// 目標地点の管理
//******************************
void CAttack::DistManager(void)
{
	// 目標地点にぶつかったとき
	if (CCollision::CollisionSphere(GetCollision(), m_pDIstCollision))
	{
		// 目標地点インデックスを進める
		m_nPointNum++;

		// 目標値点数を超えたらインデックスを0にする
		if (m_nPointNum >= CGame::GetCpuPoint()->GetPointNum(m_nRoot))
		{
			m_nPointNum = 0;
		}

		// コースデータの取得
		CCpuPoint::CpuPointData pointData = CGame::GetCpuPoint()->GetPointData(m_nRoot,m_nPointNum);
		if (m_pDIstCollision != NULL)
		{
			// 次の目標地点
			m_pDIstCollision->SetPos(pointData.pos);
		}
	}

	// 距離を測る用
	float fDistance = 99999.0f;
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		CCharacter * pChara = CGame::GetCharacter(nCnt);
		
		if (pChara != NULL&&pChara->GetRankData().nRank<m_nRank)
		{// Nullチェック&順位が放った人より上の時

			if (pChara->GetID() != m_nUseID)
			{// 放った人には追従市内
				if (m_pTargetCollision != NULL&&pChara->GetCollision() != NULL)
				{
					if (CCollision::CollisionSphere(pChara->GetCollision(), m_pTargetCollision) && fDistance > D3DXVec3Length(&(GetPos() - pChara->GetPos())))
					{// 索敵範囲にキャラが入っていた時&&距離が他のキャラより近かった時

						// 距離の保存
						fDistance = D3DXVec3Length(&(GetPos() - pChara->GetPos()));

						// ポイントのインデックスを一番近い地点の次の番号をにする*一番目だと一回逆走しそう*
						m_nPointNum = CGame::GetCpuPoint()->GetNearPosIndex(m_nRoot, GetPos()) + 1;
						// クランプ処理
						if (CGame::GetCpuPoint()->GetPointNum(m_nRoot) < m_nPointNum) m_nPointNum = 0;

						if (m_pDIstCollision != NULL)
						{
							// 次の目標地点
							m_pDIstCollision->SetPos(pChara->GetPos());
						}
					}
				}
			}
		}
	}
}

//******************************
// プレイヤー・NPCとの当たり判定*キャラクタークラスの更新で呼ばれてる
//******************************
void CAttack::CollisionCharacter(void)
{
	// キャラクター数分ループ
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// キャラクター情報の取得
		CCharacter*pChara = CGame::GetCharacter(nCnt);
		if (pChara->GetID() != m_nUseID)
		{// 放った人には当たらないようにする

			if (CCollision::CollisionSphere(GetCollision(), pChara->GetCollision()))
			{// 当たったとき

				if (!pChara->GetStan())
				{// キャラクターをスタン状態にする
					pChara->SetStan(true);
					if (pChara->GetIsPlayer())
					{// プレイヤーだった時

						// カメラを揺らす
						CGame::GetCamera(((CPlayer*)pChara)->GetPlayerNum())->Shake(true);

						// SE再生
						CManager::GetSound()->Play(CSound::LABEL_SE_HIT);
					}
				}

				// 消す
				Uninit();
				break;
			}
		}
	}
}

//******************************
// 落下時のアクション
//******************************
void CAttack::FallAction(void)
{
	// 消す
	Uninit();
}
