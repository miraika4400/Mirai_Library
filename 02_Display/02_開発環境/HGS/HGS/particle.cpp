////////////////////////////////////////////////////
//
//    particleクラスの処理[particle.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//*****************************
// マクロ定義
//*****************************
#define PARTICLE_BOX_TEXTURE_PATH "./data/Textures/particle001.png"    //テクスチャのパス
#define PARTICLE_STAR_TEXTURE_PATH "./data/Textures/particle000.png"   //テクスチャのパス
#define PARTICLE_CIRCLE_TEXTURE_PATH "./data/Textures/particle004.png" //テクスチャのパス

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9  CParticle::m_apTexture[PARTICLE_MAX] = {}; // テクスチャポインタ

//******************************
// コンストラクタ
//******************************
CParticle::CParticle()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_type = PARTICLE_BOX;
}

//******************************
// デストラクタ
//******************************
CParticle::~CParticle()
{
}

//******************************
// クリエイト
//******************************
CParticle * CParticle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const D3DCOLOR col,const PARTICLE_TYPE type)
{
	// メモリの確保
	CParticle *pParticle;
	pParticle = new CParticle;

	// タイプの設定
	pParticle->m_type = type;

	// 初期化
	pParticle->Init();

	// 各値の代入・セット
	pParticle->SetPos(pos);                  // 座標
	pParticle->m_move = move;                // 移動量
	pParticle->SetSize(size);                // サイズ
	pParticle->m_nLife = nLife;              // 寿命
	pParticle->SetColor(col);                // カラー
	pParticle->SetPriority(OBJTYPE_PARTICLE); // オブジェクトタイプ
	pParticle->SetAngle((float)(rand() % 360));       // 回転角度をランダム
	pParticle->SetAddMode(true);             // 加算合成
	return pParticle;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CParticle::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PARTICLE_BOX_TEXTURE_PATH, &m_apTexture[PARTICLE_BOX]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_STAR_TEXTURE_PATH, &m_apTexture[PARTICLE_STAR]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_CIRCLE_TEXTURE_PATH, &m_apTexture[PARTICLE_CIRCLE]);
	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CParticle::Unload(void)
{
	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		// テクスチャの解放処理
		if (m_apTexture[nCntParticle] != NULL)
		{
			m_apTexture[nCntParticle]->Release();
			m_apTexture[nCntParticle] = NULL;
		}
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CParticle::Init(void)
{
	if (FAILED(CScene2d::Init()))
	{
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(m_apTexture[m_type]);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CParticle::Uninit(void)
{

	CScene2d::Uninit();
}

//******************************
// 更新処理
//******************************
void CParticle::Update(void)
{
	// 移動
	SetPos(GetPos() + m_move);
	D3DXCOLOR col = GetColor();
	
	col.a -= 0.02f;
	SetColor(col);

	// 寿命
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//******************************
// 描画処理
//******************************
void CParticle::Draw(void)
{
	CScene2d::Draw();
}
