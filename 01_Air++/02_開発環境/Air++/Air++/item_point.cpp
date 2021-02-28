////////////////////////////////////////////////////
//
//    item_pointクラスの処理[item_point.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "item_point.h"
#include "manager.h"
#include "keyboard.h"
#include "debug_log.h"
#include "collision.h"
#include "game.h"
#include "character.h"
#include "player.h"
#include "item.h"

//*****************************
// マクロ定義
//*****************************
#define ITEMPOINT_TEXT "data/Texts/ItemPoint.txt" // チェックポイントテキストのパス

// チェックポイント配置モード切り替えマクロ
#define SET_MODE
#undef  SET_MODE

//*****************************
// 静的メンバ変数宣言
//*****************************

//******************************
// コンストラクタ
//******************************
CItemPoint::CItemPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// 変数のクリア
	m_nItemNum = 0;
	memset(&m_aItemPos, 0, sizeof(m_aItemPos));
}

//******************************
// デストラクタ
//******************************
CItemPoint::~CItemPoint()
{
}

//******************************
// クリエイト
//******************************
CItemPoint * CItemPoint::Create(void)
{
	// メモリの確保
	CItemPoint* pCpuPoint = new CItemPoint;

	// 初期化
	pCpuPoint->Init();

	return pCpuPoint;
}

//******************************
// 初期化処理
//******************************
HRESULT CItemPoint::Init(void)
{
	// オブジェクトタイプの設定
	SetPriority(OBJTYPE_CHECKPOINT);

	m_nItemNum = 0;
#ifndef SET_MODE

	// チェックポイントの読み込み
	LoadText();

#endif // !SET_MODE

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CItemPoint::Uninit(void)
{
	Release();
}

//******************************
// 更新処理
//******************************
void CItemPoint::Update(void)
{

#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{	
		// 座標
		m_aItemPos[m_nItemNum] = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		
		// アイテムの生成
		CItem::Create(m_aItemPos[m_nItemNum]);
		// ポイント数の加算
		m_nItemNum++;
	}


	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SaveText();
	}

#endif
}

//******************************
// 描画処理
//******************************
void CItemPoint::Draw(void)
{
}

//******************************
// セーブ
//******************************
void CItemPoint::SaveText(void)
{
	// ファイルオープン
	FILE *pFile = NULL;

	// コースのタイプにっよって分ける
	pFile = fopen(ITEMPOINT_TEXT, "w");

	if (pFile != NULL)
	{
		// チェックポイント数の書き込み
		fprintf(pFile, "%d\n", m_nItemNum);

		// 座標の書き込み
		for (int nCnt = 0; nCnt < m_nItemNum; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n", m_aItemPos[nCnt].x, m_aItemPos[nCnt].y, m_aItemPos[nCnt].z);
		}
		// ファイルクローズ
		fclose(pFile);
	}
}

//******************************
// ロード
//******************************
void CItemPoint::LoadText(void)
{
	// ファイルオープン
	FILE *pFile = NULL;

	// コースのタイプにっよって分ける
	pFile = fopen(ITEMPOINT_TEXT, "r");

	if (pFile != NULL)
	{
		// チェックポイント数の読み込み
		fscanf(pFile, "%d", &m_nItemNum);

		// チェックポイント数分ループ
		for (int nCnt = 0; nCnt < m_nItemNum; nCnt++)
		{
			// 座標の読み込み
			fscanf(pFile, "%f,%f,%f\n", &m_aItemPos[nCnt].x, &m_aItemPos[nCnt].y, &m_aItemPos[nCnt].z);

			// アイテムの生成
			CItem::Create(m_aItemPos[nCnt]);
		}

		// ファイルクローズ
		fclose(pFile);
	}
}
