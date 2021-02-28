//=============================================================================
//
// cpupointヘッダ [cpupoint.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CPUPOINT_H_
#define _CPUPOINT_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"
#include "course.h"
//*****************************
// マクロ定義
//*****************************
#define MAX_CPUPOINT_NUM 256 // CPUポイント最大数
#define ROOT_NUM 2           // ルート分岐数

//*****************************
// 前方宣言
//*****************************


//*****************************
// クラス定義
//*****************************

// CPU用コースデータクラス
class CCpuPoint : public CScene
{
public:
	//============
	// 状態列挙
	//============
	// ポイントのタイプ*インコース側判定用
	typedef enum
	{
		POINT_TYPE_NORMAL = 0, // ノーマル 
		POINT_TYPE_IN_LEFT,    // 左がインコース側
		POINT_TYPE_IN_RIGHT,   // 右がインコース側
		POINT_TYPE_MAX,        // タイプ数
	}POINTTYPE;

	//============
	// メンバ関数
	//============
	// CPU用コースデータ構造体
	typedef struct
	{
		D3DXVECTOR3 pos;        // 座標
		float fAngle;           // 角度
		POINTTYPE inType;       // インコース側判定用
	}CpuPointData;

	//============
	// メンバ関数
	//============
	CCpuPoint();
	~CCpuPoint();
	static CCpuPoint *Create(void); // クラス生成

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	D3DXVECTOR3 GetNearPos(int nRoot, D3DXVECTOR3 pos); // 引き数の座標から最も近い座標の取得
	int GetNearPosIndex(int nRoot, D3DXVECTOR3 pos); // 引き数の座標から最も近い座標のインデックスの取得
	// ポイント数の取得
	int GetPointNum(int nRoot) { return m_anNumPoint[nRoot]; }

	// ポイント数の取得
	CpuPointData GetPointData(int nRoot,int nIndex) { return m_aCpuPoint[nRoot][nIndex]; }
private:
	void SavePoint(void); // テキストファイルにポイントを書き込む
	void LoadPoint(void); // テキストファイルのポイントを読み込む

	//============
	// メンバ変数
	//============
	static char *m_apPath[ROOT_NUM];           // テキストファイルのパス
	int m_anNumPoint[ROOT_NUM];                           // ポイントの数
	int m_nCntSet;                             // 設置時のカウント
	CpuPointData m_aCpuPoint[ROOT_NUM][MAX_CPUPOINT_NUM]; // CPU用コースデータ構造体
	int m_nCpuPointNum;                        // コースデータ配列管理用
};

#endif