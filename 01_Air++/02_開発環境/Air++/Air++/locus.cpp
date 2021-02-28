////////////////////////////////////////////////////
//
//    LOCUSクラスの処理[LOCUS.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "locus.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "keyboard.h"
#include "joypad.h"
#include "model.h"

//******************************
// マクロ定義
//******************************
#define TEX_PATH "./data/Textures/locus.png"
#define LIFE_MAX 60 // 寿命
#define MAINUS_ALPHA 0.05f // アルファ値を減らす

//******************************
// 前方宣言
//******************************
LPDIRECT3DTEXTURE9 CLocus::m_pTexture = NULL;

//===================================
// コンストラクタ
//===================================
CLocus::CLocus(int nPriority) :CScene(OBJTYPE_SYSTEM)
{
	m_pVtxBuff = NULL;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_bAddMode = false;
	m_nLife = 0;
	D3DXMatrixIdentity(&m_mtxWorld);
}

//===================================
// デストラクタ
//===================================
CLocus::~CLocus()
{
}

//===================================
// クリエイト関数
//===================================
CLocus * CLocus::Create(D3DXVECTOR3 aPos[NUM_VERTEX /2], D3DXVECTOR3 aPosOld[NUM_VERTEX/2] , D3DXCOLOR col)
{
	// メモリの確保
	CLocus *pLocus;
	pLocus = new CLocus;
	// 初期化
	pLocus->Init();
	pLocus->SetPos(aPos, aPosOld);
	pLocus->SetColor(col);

	return pLocus;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CLocus::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEX_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CLocus::Unload(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//===================================
// 初期化処理
//===================================
HRESULT CLocus::Init(void)
{
	VERTEX_3D *pVtx;// 頂点情報ポインタ

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// メンバ変数の初期化
	m_col = { 1.0f,1.0f,1.0f,1.0f };

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = VEC3_ZERO;
	pVtx[1].pos = VEC3_ZERO;
	pVtx[2].pos = VEC3_ZERO;
	pVtx[3].pos = VEC3_ZERO;

	// テクスチャUV座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCnt = 0; nCnt < NUM_VERTEX; nCnt++)
	{
		// 色の設定
		pVtx[nCnt].col = m_col;
		// 法線
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// アンロック
	m_pVtxBuff->Unlock();

	// 加算合成モードにする
	m_bAddMode = true;

	return S_OK;
}

//===================================
// 終了処理
//===================================
void CLocus::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//===================================
// 更新処理
//===================================
void CLocus::Update(void)
{
	// ライフをカウント
	m_nLife++;

	m_col.a -= MAINUS_ALPHA;
	SetColor(m_col);
	if (m_nLife > LIFE_MAX)
	{// 寿命を超えたら
		// 消す
		Uninit();
	}
}

//===================================
// 描画処理
//===================================
void CLocus::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 
	// ライトを消す
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// 加算モードの時
	if (m_bAddMode)
	{
		// 加算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	// テクスチャ初期化
	pDevice->SetTexture(0, NULL);

	// 加算モードの時
	if (m_bAddMode)
	{
		// 通常合成に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// ライトをつける
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//===================================
// 座標のセット
//===================================
void CLocus::SetPos(const D3DXVECTOR3 aPos[NUM_VERTEX / 2], const D3DXVECTOR3 aPosOld[NUM_VERTEX / 2])
{
	VERTEX_3D *pVtx;// 頂点情報ポインタ

	// ロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = aPos[0];
	pVtx[1].pos = aPos[1];
	pVtx[2].pos = aPosOld[0];
	pVtx[3].pos = aPosOld[1];

	// アンロック
	m_pVtxBuff->Unlock();
}

//===================================
// 色のセット
//===================================
void CLocus::SetColor(const D3DXCOLOR col)
{
	VERTEX_3D *pVtx;// 頂点情報ポインタ

	m_col = col;

	// ロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// アンロック
	m_pVtxBuff->Unlock();
}