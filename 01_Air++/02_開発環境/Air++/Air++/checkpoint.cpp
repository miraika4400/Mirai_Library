////////////////////////////////////////////////////
//
//    particleクラスの処理[particle.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "checkpoint.h"
#include "manager.h"
#include "keyboard.h"
#include "debug_log.h"
#include "collision.h"
#include "game.h"
#include "character.h"
#include "player.h"

//*****************************
// マクロ定義
//*****************************
#define CHECKPOINT_RADIUS 700                        // 当たり判定の半径
#define CHECKPOINT_TEXT "data/Texts/CoursePoint.txt" // チェックポイントテキストのパス
#define GOAL_LAP_NUM 3                               // 何週でゴールか

// チェックポイント配置モード切り替えマクロ
#define SET_MODE
#undef  SET_MODE

//*****************************
// 静的メンバ変数宣言
//*****************************

//******************************
// コンストラクタ
//******************************
CCheckPoint::CCheckPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// 変数のクリア
	m_nNumPoint = 0;
	memset(&m_pCollision, 0, sizeof(m_pCollision));
	memset(&m_pointPos, 0, sizeof(m_pointPos));
}

//******************************
// デストラクタ
//******************************
CCheckPoint::~CCheckPoint()
{
}

//******************************
// クリエイト
//******************************
CCheckPoint * CCheckPoint::Create(void)
{
	// メモリの確保
	CCheckPoint* pCheckPoint = new CCheckPoint;

	// 初期化
	pCheckPoint->Init();

	return pCheckPoint;
}

//******************************
// 初期化処理
//******************************
HRESULT CCheckPoint::Init(void)
{
	// オブジェクトタイプの設定
	SetPriority(OBJTYPE_CHECKPOINT);

#ifndef SET_MODE
	// チェックポイントの読み込み
	LoadCheckpoint();

#endif // !SET_MODE

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCheckPoint::Uninit(void)
{
	Release();
}

//******************************
// 更新処理
//******************************
void CCheckPoint::Update(void)
{

#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_pointPos[m_nNumPoint] = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();

		m_pCollision[m_nNumPoint] = CCollision::CreateSphere(m_pointPos[m_nNumPoint], CHECKPOINT_RADIUS);
	
		m_nNumPoint++;
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE))
	{
		m_nNumPoint--;
		m_pCollision[m_nNumPoint]->Uninit();
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SaveCheckpoint();

		CDebugLog::Init();
		CDebugLog::Print("save");
	}
#else
	// キャラクター数分ループ
	for (int nCntCharacter = 0; nCntCharacter < CGame::GetCharacterNum(); nCntCharacter++)
	{
		// キャラクター情報の取得
		CCharacter*pChara = CGame::GetCharacter(nCntCharacter);
		if (pChara != NULL && !pChara->GetGoalFlag())
		{
			// 順位付けデータの取得
			CCharacter::RankData rankData = pChara->GetRankData();

			// チェックポイント数分ループ
			for (int nCntPoint = 0; nCntPoint < m_nNumPoint; nCntPoint++)
			{
				// チャックポイント番号の比較
				if (rankData.nCheck == nCntPoint)
				{
					// チャックポイントとの当たり判定
					if (CCollision::CollisionSphere(pChara->GetCollision(), m_pCollision[nCntPoint]))
					{
						// チェックポイントを増やす
						rankData.nCheck++;

						// 一周判定
						if (rankData.nCheck >= m_nNumPoint)
						{
							rankData.nCheck = 0;
							rankData.nLap++;

							// ラップ数がゴールに達したら
							if (rankData.nLap >= GOAL_LAP_NUM)
							{
								// ゴール状態にする
								pChara->GoalAction();
							}
						}
						// キャラクタークラスにランクデータをセットする
						pChara->SetRankData(rankData);
					}
				}
			}
		}
	}

#endif
}

//******************************
// 描画処理
//******************************
void CCheckPoint::Draw(void)
{
}

//******************************
// セーブ
//******************************
void CCheckPoint::SaveCheckpoint(void)
{
	// ファイルオープン
	FILE *pFile = NULL;

	// コースのタイプにっよって分ける
	pFile = fopen(CHECKPOINT_TEXT, "w");

	if (pFile != NULL)
	{
		// チェックポイント数の書き込み
		fprintf(pFile, "%d\n", m_nNumPoint);

		// 座標の書き込み
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n", m_pointPos[nCnt].x, m_pointPos[nCnt].y, m_pointPos[nCnt].z);
		}
		// ファイルクローズ
		fclose(pFile);
	}
}

//******************************
// ロード
//******************************
void CCheckPoint::LoadCheckpoint(void)
{
	// ファイルオープン
	FILE *pFile = NULL;

	// コースのタイプにっよって分ける
	pFile = fopen(CHECKPOINT_TEXT, "r");

	if (pFile != NULL)
	{
		// チェックポイント数の読み込み
		fscanf(pFile, "%d", &m_nNumPoint);

		// チェックポイント数分ループ
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			// 座標の読み込み
			fscanf(pFile, "%f,%f,%f\n", &m_pointPos[nCnt].x, &m_pointPos[nCnt].y, &m_pointPos[nCnt].z);
			
			// コリジョンの生成
			m_pCollision[nCnt] = CCollision::CreateSphere(m_pointPos[nCnt], CHECKPOINT_RADIUS);
		}

		// ファイルクローズ
		fclose(pFile);
	}
}
