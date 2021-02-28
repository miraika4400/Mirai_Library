////////////////////////////////////////////////////
//
//    FeverLogoの処理[FeverLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "rank.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"

//**********************************
// マクロ定義
//**********************************

//**********************************
// 静的メンバ変数宣言
//**********************************


//=============================
// コンストラクタ
//=============================
CRank::CRank() :CScene(OBJTYPE_SYSTEM)
{
	
}

//=============================
// デストラクタ
//=============================
CRank::~CRank()
{
}

//=============================
// クリエイト
//=============================
CRank * CRank::Create()
{
	// メモリの確保
	CRank *pRank = new CRank;

	// 初期化
	pRank->Init();
	return pRank;
}

//=============================
// 初期化処理
//=============================
HRESULT CRank::Init(void)
{

	return S_OK;
}

//=============================
// 終了処理
//=============================
void CRank::Uninit(void)
{
	Release();
}

//=============================
// 更新処理
//=============================
void CRank::Update(void)
{
	// 順位の管理
	RankManager();
}

//=============================
// 描画処理
//=============================
void CRank::Draw(void)
{
}

//==================================
// 順位の管理
//==================================
void CRank::RankManager(void)
{
	// プレイヤー配列
	CCharacter*pCharacter[MAX_CHARACTER_NUM] = {};

	// プレイヤー人数の取得
	int nNumCharacter = CGame::GetCharacterNum();

	// 配列にプレイヤーを格納
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		if (CGame::GetCharacter(nCnt) != NULL)
		{
			pCharacter[nCnt] = CGame::GetCharacter(nCnt);
		}
	}

	//	現在のランク順にソート処理
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < nNumCharacter; nCnt2++)
		{
			if (pCharacter[nCnt]->GetRankData().nRank < pCharacter[nCnt2]->GetRankData().nRank)
			{// チェックポイントの数でソート

				CCharacter*pSave = pCharacter[nCnt];
				pCharacter[nCnt] = pCharacter[nCnt2];
				pCharacter[nCnt2] = pSave;

				continue;
			}
		}
	}

	//	周回数、ラップ数、チェックポイントまでの距離でソート処理
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < nNumCharacter; nCnt2++)
		{

			if (pCharacter[nCnt] != NULL && pCharacter[nCnt2] != NULL)
			{

				if (!pCharacter[nCnt]->GetGoalFlag() && !pCharacter[nCnt2]->GetGoalFlag())
				{
					if (pCharacter[nCnt]->GetRankData().nLap > pCharacter[nCnt2]->GetRankData().nLap)
					{// チェックポイントの数でソート

						CCharacter*pSave = pCharacter[nCnt];
						pCharacter[nCnt] = pCharacter[nCnt2];
						pCharacter[nCnt2] = pSave;

						continue;
					}
					else if (pCharacter[nCnt]->GetRankData().nLap == pCharacter[nCnt2]->GetRankData().nLap &&
						pCharacter[nCnt]->GetRankData().nCheck > pCharacter[nCnt2]->GetRankData().nCheck)
					{// チェックポイントの数でソート
						CCharacter*pSave = pCharacter[nCnt];
						pCharacter[nCnt] = pCharacter[nCnt2];
						pCharacter[nCnt2] = pSave;

						continue;
					}
					else if (pCharacter[nCnt]->GetRankData().nCheck == pCharacter[nCnt2]->GetRankData().nCheck)
					{// チェックポイント数が一緒だった時

						int nCheck = pCharacter[nCnt]->GetRankData().nCheck;
						
						float fProgress = CheckProgress(nCheck, pCharacter[nCnt]->GetPos());
						
						if (fProgress > CheckProgress(nCheck, pCharacter[nCnt2]->GetPos()))
						{
							CCharacter*pSave = pCharacter[nCnt];
							pCharacter[nCnt] = pCharacter[nCnt2];
							pCharacter[nCnt2] = pSave;
							continue;
						}

						//D3DXVECTOR3 checkPos = CGame::GetCheckPoint()->GetCollision(pCharacter[nCnt]->GetRankData().nCheck)->GetPos();
						//
						//// チェックポイントとの距離でソート
						//float fDistance = sqrtf(powf(checkPos.x - pCharacter[nCnt]->GetPos().x, 2) +
						//	powf(checkPos.y - pCharacter[nCnt]->GetPos().y, 2) +
						//	powf(checkPos.z - pCharacter[nCnt]->GetPos().z, 2));
						//
						//if (fDistance < sqrtf(powf(checkPos.x - pCharacter[nCnt2]->GetPos().x, 2) +
						//	powf(checkPos.y - pCharacter[nCnt2]->GetPos().y, 2) +
						//	powf(checkPos.z - pCharacter[nCnt2]->GetPos().z, 2)))
						//
						//{
						//	CCharacter*pSave = pCharacter[nCnt];
						//	pCharacter[nCnt] = pCharacter[nCnt2];
						//	pCharacter[nCnt2] = pSave;
						//	continue;
						//}
					}
				}
			}
		}
	}

	// 配列の順番がそのまま順位
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		if (pCharacter[nCnt] != NULL)
		{
			// ランクデータの取得
			CCharacter::RankData rankData = pCharacter[nCnt]->GetRankData();
			// 順位は入れ番号+1
			rankData.nRank = nCnt + 1;
			// ランクデータのセット
			pCharacter[nCnt]->SetRankData(rankData);
		}
	}
}

//==================================
// 進行度の算出
//==================================
float CRank::CheckProgress(int nCheckNum , D3DXVECTOR3 pos)
{
	// 最新で通過しているチェックポイントの座標
	D3DXVECTOR3 nextCheckPos = CGame::GetCheckPoint()->GetCollision(nCheckNum)->GetPos();
	
	// 次のチェックポイント番号
	int nowCheckNum = nCheckNum - 1;
	if (nowCheckNum < 0) nowCheckNum = CGame::GetCheckPoint()->GetCheckPointNum()-1;
	// 次のチェックポイントの座標の取得
	D3DXVECTOR3 nowCheckPos = CGame::GetCheckPoint()->GetCollision(nowCheckNum)->GetPos();

	// 進行方向への単位ベクトル
	D3DXVECTOR3 progressDir;
	progressDir = nextCheckPos - nowCheckPos;
	
	// 内積を用いて進行度の算出して返す
	return (float)D3DXVec3Dot(&progressDir, &(pos - nowCheckPos));
}
