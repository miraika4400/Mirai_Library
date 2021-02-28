////////////////////////////////////////////////////
//
//    cpu_pointクラスの処理[cpu_point.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "cpu_point.h"
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
#define CHECKPOINT_RADIUS 700                   // 当たり判定の半径
#define CPUPOINT_TEXT_1 "data/Texts/CpuPoint.txt" // チェックポイントテキストのパス
#define CPUPOINT_TEXT_2 "data/Texts/CpuPoint2.txt" // チェックポイントテキストのパス
#define POINT_SET_INTERVAL 40                   // ポイントセットインターバール

// チェックポイント配置モード切り替えマクロ
#define SET_MODE
#undef  SET_MODE

//*****************************
// 静的メンバ変数宣言
//*****************************
char *CCpuPoint::m_apPath[ROOT_NUM] = 
{
	{ CPUPOINT_TEXT_1 },
	{ CPUPOINT_TEXT_2 }
};

//******************************
// コンストラクタ
//******************************
CCpuPoint::CCpuPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// 変数のクリア
	memset(&m_anNumPoint, 0, sizeof(m_anNumPoint));
	m_nCntSet = 0;
	m_nCpuPointNum = 0;
	memset(&m_aCpuPoint, 0, sizeof(m_aCpuPoint));
}

//******************************
// デストラクタ
//******************************
CCpuPoint::~CCpuPoint()
{
}

//******************************
// クリエイト
//******************************
CCpuPoint * CCpuPoint::Create(void)
{
	// メモリの確保
	CCpuPoint* pCpuPoint = new CCpuPoint;

	// 初期化
	pCpuPoint->Init();

	return pCpuPoint;
}

//******************************
// 初期化処理
//******************************
HRESULT CCpuPoint::Init(void)
{
	// オブジェクトタイプの設定
	SetPriority(OBJTYPE_CHECKPOINT);

	m_nCntSet = 0;

#ifndef SET_MODE

	// チェックポイントの読み込み
	LoadPoint();

#endif // !SET_MODE

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCpuPoint::Uninit(void)
{
	Release();
}

//******************************
// 更新処理
//******************************
void CCpuPoint::Update(void)
{

#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_NUMPAD5))
	{// コーナーじゃないポイントの設置
		// 座標
		m_aCpuPoint[m_nNumPoint].pos = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		// 向き
		m_aCpuPoint[m_nNumPoint].fAngle = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetRot().y - D3DXToRadian(180.0f);
		// インコース判定
		m_aCpuPoint[m_nNumPoint].inType = POINT_TYPE_NORMAL;
		// モデルの生成
		CModel::Create(m_aCpuPoint[m_nNumPoint].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_aCpuPoint[m_nNumPoint].fAngle, 0.0f));
		// ポイント数の加算
		m_nNumPoint++;
	}

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_NUMPAD4))
	{// 左側がインコースのポイント設置
		// 座標
		m_aCpuPoint[m_nNumPoint].pos = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		// 向き
		m_aCpuPoint[m_nNumPoint].fAngle = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetRot().y - D3DXToRadian(180.0f);
		// インコース判定
		m_aCpuPoint[m_nNumPoint].inType = POINT_TYPE_IN_LEFT;
		// モデルの生成
		CModel::Create(m_aCpuPoint[m_nNumPoint].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_aCpuPoint[m_nNumPoint].fAngle, 0.0f));

		// インコースの目印座標
		D3DXVECTOR3 inPos = m_aCpuPoint[m_nNumPoint].pos;
		inPos.x = inPos.x + cosf(-m_aCpuPoint[m_nNumPoint].fAngle) * 400;
		inPos.y = inPos.y;
		inPos.z = inPos.z + sinf(-m_aCpuPoint[m_nNumPoint].fAngle) * 400;

		// モデルの生成
		CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);

		// ポイント数の加算
		m_nNumPoint++;
	}

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_NUMPAD6))
	{// 右側がインコースのポイント設置
		// 座標
		m_aCpuPoint[m_nNumPoint].pos = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		// 向き
		m_aCpuPoint[m_nNumPoint].fAngle = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetRot().y - D3DXToRadian(180.0f);
		// インコース判定
		m_aCpuPoint[m_nNumPoint].inType = POINT_TYPE_IN_RIGHT;
		// モデルの生成
		CModel::Create(m_aCpuPoint[m_nNumPoint].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_aCpuPoint[m_nNumPoint].fAngle, 0.0f));

		// インコースの目印座標
		D3DXVECTOR3 inPos = m_aCpuPoint[m_nNumPoint].pos;
		inPos.x = inPos.x + cosf(-m_aCpuPoint[m_nNumPoint].fAngle) * -400;
		inPos.y = inPos.y;
		inPos.z = inPos.z + sinf(-m_aCpuPoint[m_nNumPoint].fAngle) * -400;

		// モデルの生成
		CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);

		// ポイント数の加算
		m_nNumPoint++;
	}

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SavePoint();
	}
#else


#endif
}

//******************************
// 描画処理
//******************************
void CCpuPoint::Draw(void)
{
}

//**********************************
// 一番近い座標の取得
//**********************************
D3DXVECTOR3 CCpuPoint::GetNearPos(int nRoot, D3DXVECTOR3 pos)
{
	// 配列0との距離を測る
	float fDistance = D3DXVec3Length(&(m_aCpuPoint[nRoot][0].pos - pos));
	// return用
	D3DXVECTOR3 out = m_aCpuPoint[nRoot][0].pos;

	// 近い地点の検索
	for (int nCnt = 1; nCnt < m_anNumPoint[nRoot]; nCnt++)
	{
		// 保存してある距離より近いとき
		if (fDistance > D3DXVec3Length(&(m_aCpuPoint[nRoot][nCnt].pos - pos)))
		{
			// 距離の更新
			fDistance = D3DXVec3Length(&(m_aCpuPoint[nRoot][nCnt].pos - pos));
			// 配列の保存
			out = m_aCpuPoint[nRoot][nCnt].pos;
		}
	}

	return out;
}

//**********************************
// 一番近い座標のインデックスの取得
//**********************************
int CCpuPoint::GetNearPosIndex(int nRoot, D3DXVECTOR3 pos)
{
	// 配列0との距離を測る
	float fDistance = D3DXVec3Length(&(m_aCpuPoint[nRoot][0].pos - pos));
	// return用配列の保存
	int nIndex = 0;

	// 近い地点の検索
	for (int nCnt = 1; nCnt < m_anNumPoint[nRoot]; nCnt++)
	{
		// 保存してある距離より近いとき
		if (fDistance > D3DXVec3Length(&(m_aCpuPoint[nRoot][nCnt].pos - pos)))
		{
			// 距離の更新
			fDistance = D3DXVec3Length(&(m_aCpuPoint[nRoot][nCnt].pos - pos));
			// 配列の保存
			nIndex = nCnt;
		}
	}

	return nIndex;
}

//******************************
// セーブ
//******************************
void CCpuPoint::SavePoint(void)
{
	// ファイルオープン
	FILE *pFile = NULL;


	// コースのタイプにっよって分ける
	pFile = fopen(m_apPath[0], "w");

	if (pFile != NULL)
	{
		// チェックポイント数の書き込み
		fprintf(pFile, "%d\n", m_anNumPoint[0]);

		// 座標の書き込み
		for (int nCnt = 0; nCnt < m_anNumPoint[0]; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n", m_aCpuPoint[0][nCnt].pos.x, m_aCpuPoint[0][nCnt].pos.y, m_aCpuPoint[0][nCnt].pos.z);
			fprintf(pFile, "%.2f\n", m_aCpuPoint[0][nCnt].fAngle);
			fprintf(pFile, "%d\n", (int)m_aCpuPoint[0][nCnt].inType);
		}
		// ファイルクローズ
		fclose(pFile);
	}
}

//******************************
// ロード
//******************************
void CCpuPoint::LoadPoint(void)
{
	// ファイルオープン
	FILE *pFile = NULL;
	for (int nCntRoot = 0; nCntRoot < ROOT_NUM; nCntRoot++)
	{
		// コースのタイプにっよって分ける
		pFile = fopen(m_apPath[nCntRoot], "r");

		if (pFile != NULL)
		{
			// チェックポイント数の読み込み
			fscanf(pFile, "%d", &m_anNumPoint[nCntRoot]);

			// チェックポイント数分ループ
			for (int nCnt = 0; nCnt < m_anNumPoint[nCntRoot]; nCnt++)
			{
				// 座標の読み込み
				fscanf(pFile, "%f,%f,%f\n", &m_aCpuPoint[nCntRoot][nCnt].pos.x, &m_aCpuPoint[nCntRoot][nCnt].pos.y, &m_aCpuPoint[nCntRoot][nCnt].pos.z);
				fscanf(pFile, "%f\n", &m_aCpuPoint[nCntRoot][nCnt].fAngle);
				fscanf(pFile, "%d\n", &m_aCpuPoint[nCntRoot][nCnt].inType);
#ifdef _DEBUG

				CModel::Create(m_aCpuPoint[nCntRoot][nCnt].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_aCpuPoint[nCntRoot][nCnt].fAngle, 0.0f));

				if (m_aCpuPoint[nCntRoot][nCnt].inType == POINT_TYPE_IN_LEFT)
				{
					// インコースの目印座標
					D3DXVECTOR3 inPos = m_aCpuPoint[nCntRoot][nCnt].pos;
					inPos.x = inPos.x + cosf(-m_aCpuPoint[nCntRoot][nCnt].fAngle) * 400;
					inPos.y = inPos.y;
					inPos.z = inPos.z + sinf(-m_aCpuPoint[nCntRoot][nCnt].fAngle) * 400;

					// モデルの生成
					CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);
				}
				else if (m_aCpuPoint[nCntRoot][nCnt].inType == POINT_TYPE_IN_RIGHT)
				{
					// インコースの目印座標
					D3DXVECTOR3 inPos = m_aCpuPoint[nCntRoot][nCnt].pos;
					inPos.x = inPos.x + cosf(-m_aCpuPoint[nCntRoot][nCnt].fAngle) * -400;
					inPos.y = inPos.y;
					inPos.z = inPos.z + sinf(-m_aCpuPoint[nCntRoot][nCnt].fAngle) * -400;

					// モデルの生成
					CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);
				}
#endif // _DEBUG
			}
		}

		// ファイルクローズ
		fclose(pFile);
	}
}
