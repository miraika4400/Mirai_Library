//=============================================================================
//
// gameヘッダ [game.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _GAME_H_
#define _GAME_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//=============================
// 前方宣言
//=============================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CCamera;
class CLight;
class CCharacter;
class CCpuPoint;
class CCheckPoint;
class CItemPoint;
class CCourse;

//*****************************
//マクロ定義
//*****************************
#define MAX_PLAYER_NUM 1                               // プレイヤーの最大数
#define MAX_NPC_NUM 7                                  // NPCの最大数
#define MAX_CHARACTER_NUM MAX_PLAYER_NUM + MAX_NPC_NUM // キャラクターの最大数(プレイヤー最大数 + NPC最大数)

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CGame : public CScene
{
public:
	//*****************************
	// 列挙定義
	//*****************************
	typedef enum
	{
		GAME_NORMAL,
		GAME_RESULT,
		GAME_MAX
	}GAME_STATE;

	//*****************************
	// メンバ関数
	//*****************************
	CGame();
	~CGame();

	static CGame *Create(void); // クラス生成

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	// 取得処理
	static CCamera     *GetCamera(int nIndex) { return m_apCamera[nIndex]; }       // カメラポインタ
	static CCharacter  *GetCharacter(int nIndex) { return m_apCharacter[nIndex]; } // キャラクターポインタ(引数：配列番号)
	static CCpuPoint   *GetCpuPoint(void) { return m_pCpuPoint; }                  // CPU用コースポイント
	static CCheckPoint *GetCheckPoint(void) { return m_pCheckPoint; }              // チェックポイント
	static CCourse     *GetCourse(void) { return m_pCourse; }                      // チェックポイント
	static int          GetCharacterNum(void) { return m_nNumCaracter; }           // キャラクター数
	static int          GetPlayerNum(void) { return m_nNumPlayer; }                // プレイヤー数
	static GAME_STATE   GetState(void) { return m_gameState; }

private:
	void CheckGoal(void); // プレイヤーがゴールかチェックしてリザルトフラグを立てる
	//*****************************
	// メンバ変数
	//*****************************
	static CCamera*m_apCamera[MAX_PLAYER_NUM];            // カメラ
	static CLight*m_pLight;                               // ライトポインタ
	static CCharacter* m_apCharacter[MAX_CHARACTER_NUM];  // キャラクターポインタ(最大キャラ数分配列確保)
	static int m_nNumCaracter;                            // 生成したキャラクター数
	static int m_nNumPlayer;                              // プレイヤー数
	static CCpuPoint *m_pCpuPoint;                        // CPUコースデータ
	static CCheckPoint*m_pCheckPoint;                     // チェックポイント
	static CItemPoint *m_pItemPoint;                      // アイテム座標の管理
	static CCourse    *m_pCourse;                         // コースポインタ
	static GAME_STATE m_gameState;                        // ゲームの状態
	int  m_nCntResult;                                    // リザルト遷移までのカウント
	bool m_bResult;                                       // リザルト遷移フラグ
};

#endif