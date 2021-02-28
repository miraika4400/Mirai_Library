////////////////////////////////////////////////////
//
//    カメラクラスの処理[camera.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "mouse.h"
#include "player.h"
#include "cpu.h"
#include "destination.h"
#include "joypad.h"

//******************************
// マクロ定義
//******************************
#define CAMERA_DISTANCE 850.0f                               // カメラと対象の距離
#define CAMERA_VIEW_MAX 100000.0f                            // カメラの描画距離(遠)
#define CAMERA_VIEW_MIN 10.0f                                // カメラの描画距離(近)
#define CAMERA_FOV_BASE D3DXToRadian(45.0f)                  // 視野角
#define CAMERA_FOV_ACCELERATION D3DXToRadian(60.0f)          // 視野角*加速時
#define CAMERA_FOV_STAN D3DXToRadian(35.0f)                  // 視野角*スタン時
#define CAMERA_FOV_RATE 0.05f                                // 視野角変化時の係数
#define CAMERA_BACK_BUFFE D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f) // カメラの背景色
#define CAMERA_MOVE_SPEED_MAX 0.04f                          // カメラ操作感度最大値
#define DAMERA_THETA_RATE 0.05f                              // カメラのシータ調整時の係数
#define CAMERA_THETA_BASE D3DXToRadian(75.0f)                // カメラのファイ(高さ)の基本角度
#define DAMERA_PHI_RATE 0.01f                                // カメラのファイ(高さ)調整時の係数
#define THETA_DIFFERENCE D3DXToRadian(60)                    // シータとシータの目標値の差の最大
#define SHAKE_VEC D3DXVECTOR3(60.0f,70.0f,30.0f)             // ブレの方向
#define SHAKE_COUNT 6                                        // ブレの方向転換時インターバル
#define SHAKE_RATE 0.05f                                     // ブレの係数

// CPUに追従替えるに切り替えるマクロ
#define CPU_CAMERA
#undef  CPU_CAMERA

//******************************
// 静的メンバ変数宣言
//******************************
CCamera*CCamera::m_pCamera = NULL;

//******************************
// コンストラクタ
//******************************
CCamera::CCamera()
{
	// 変数のクリア
	m_posV = { 0.0f,0.0f,0.0f };
	m_posR = { 0.0f,0.0f,0.0f };
	m_vecU = { 0.0f,0.0f,0.0f };
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	m_nCntShake = 0;
	m_shake = VEC3_ZERO;
	m_shakeDist = VEC3_ZERO;
	m_nPlayerNum = 0;
	m_fRad   = 0.0f;              // 目標からの距離
	m_fTheta = 0.0f;              // 角度シータ
	m_fPhi   = 0.0f;              // 角度ファイ
	m_fFov = 0.0f;                // 視野角
}

//******************************
// デストラクタ
//******************************
CCamera::~CCamera()
{
}

//******************************
// クリエイト
//******************************
CCamera * CCamera::Create(int nPlayerNum)
{

	m_pCamera = new CCamera;
	m_pCamera->Init();

	// プレイヤー番号のセット
	m_pCamera->m_nPlayerNum = nPlayerNum;

	return m_pCamera;
}


//******************************
// 初期化処理
//******************************
HRESULT CCamera::Init(void)
{
	// 変数の初期化

	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // 上方向ベクトル

	m_fRad = CAMERA_DISTANCE;     // 距離
	m_fTheta = D3DXToRadian(90);  // 角度シータ
	m_fPhi = CAMERA_THETA_BASE;   // 角度ファイ

	// 球面座標の設定
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

	m_fFov = CAMERA_FOV_BASE; //視野角

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCamera::Uninit(void)
{
	delete this;
}

//******************************
// 更新処理
//******************************
void CCamera::Update(void)
{
#ifdef CPU_CAMERA
	// CPUに追従するカメラ（デバッグ用）*ちょっと雑

	static CCpu* pCpu = (CCpu*)CScene::GetTop(CScene::OBJTYPE_CPU);

	// 見ているCPUの切り替え
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RIGHT))
	{
		if (pCpu->GetNext() != NULL)
		{
			pCpu = (CCpu*)pCpu->GetNext();
		}
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_LEFT))
	{
		if (pCpu->GetPrev() != NULL)
		{
			pCpu = (CCpu*)pCpu->GetPrev();
		}
	}

	// 注視点をCPUにする
	if (pCpu != NULL)
	{
		m_posR = pCpu->GetPos();
	}

	// 角度シータの目標値
	float fThetaDist = (-pCpu->GetRot().y - D3DXToRadian(90));

	// グルんと回転しないよう調整
	while (m_fTheta - fThetaDist > D3DXToRadian(180))
	{
		fThetaDist += D3DXToRadian(360);
	}
	while (m_fTheta - fThetaDist < D3DXToRadian(-180))
	{
		fThetaDist -= D3DXToRadian(360);
	}

	// シータを目標値に近づける
	m_fTheta += (fThetaDist - m_fTheta)*0.05f;
	
	//高さをCPUのロットXに対応させる
	m_fPhi += ((-pCpu->GetRot().x) + CAMERA_THETA_BASE - m_fPhi)*DAMERA_PHI_RATE;

	// 球面座標の設定
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

#else
	// プレイヤーに追従するカメラ
	CPlayer* pPlayer = (CPlayer*)CScene::GetTop(CScene::OBJTYPE_PLAYER);

	while (pPlayer != NULL)
	{
		if (pPlayer != NULL &&pPlayer->GetPlayerNum() == m_nPlayerNum)
		{
			// 注視点をプレイヤーにする
			if (pPlayer != NULL)
			{
				// ブレの値の処理
				Shake();

				m_posR = pPlayer->GetDest() ->GetPos() + m_shake;

				float fThetaDist = -pPlayer->GetDest()->GetRot().y+D3DXToRadian(90);

				// グルんと回転しないように調整
				while (m_fTheta - fThetaDist > D3DXToRadian(180))  fThetaDist += D3DXToRadian(360);
				while (m_fTheta - fThetaDist < D3DXToRadian(-180)) fThetaDist -= D3DXToRadian(360);

				// シータの目標値の差の上限、下限
				if (fThetaDist - m_fTheta >= THETA_DIFFERENCE)
				{
					m_fTheta = fThetaDist + -THETA_DIFFERENCE;
				}
				if (fThetaDist - m_fTheta <= -THETA_DIFFERENCE)
				{
					m_fTheta = fThetaDist + THETA_DIFFERENCE;
				}
				m_fTheta += (fThetaDist - m_fTheta)*DAMERA_THETA_RATE;
			}

			//// マウスで視点操作
			//m_fTheta -= min(max(CManager::GetMouse()->GetMouseMove().x / 100, -CAMERA_MOVE_SPEED_MAX), CAMERA_MOVE_SPEED_MAX);

			
			//高さをプレイヤーのロットXに対応させる
			m_fPhi += ((-pPlayer->GetRot().x) + CAMERA_THETA_BASE - m_fPhi)*DAMERA_PHI_RATE;

			if (pPlayer->GetActiveAcceleration())
			{// 加速時
				m_fFov += (CAMERA_FOV_ACCELERATION - m_fFov)*CAMERA_FOV_RATE;
			}
			else if (pPlayer->GetStan())
			{// スタン時
				m_fFov += (CAMERA_FOV_STAN - m_fFov)*CAMERA_FOV_RATE;
			}
			else
			{// 通常
				m_fFov += (CAMERA_FOV_BASE - m_fFov)*CAMERA_FOV_RATE;
			}

		
				
			if (CManager::GetMouse()->GetMousePress(1)|| CManager::GetJoypad()->GetJoystickPress(1, 0))
			{// バックミラー

				// 球面座標の設定
				m_posV.x = pPlayer->GetPos().x + (m_fRad)* sinf((pPlayer->GetRot().x) + CAMERA_THETA_BASE)*cosf(m_fTheta + D3DXToRadian(180));
				m_posV.y = pPlayer->GetPos().y + (m_fRad)* cosf((pPlayer->GetRot().x) + CAMERA_THETA_BASE);
				m_posV.z = pPlayer->GetPos().z + (m_fRad)* sinf((pPlayer->GetRot().x) + CAMERA_THETA_BASE)*sinf(m_fTheta + D3DXToRadian(180));
			}
			else
			{
				// 球面座標の設定
				m_posV.x = pPlayer->GetPos().x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
				m_posV.y = pPlayer->GetPos().y + (m_fRad)* cosf(m_fPhi);
				m_posV.z = pPlayer->GetPos().z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);
			}

			break;
		}
		pPlayer = (CPlayer*)pPlayer->GetNext();
	}

#endif // CPU_CAMERA
}

//******************************
// カメラのセット
//******************************
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIXA16 matWorld;

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera->m_mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_pCamera->m_mtxView, &m_pCamera->m_posV, &m_pCamera->m_posR, &m_pCamera->m_vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera->m_mtxView);

	//プロジェクトマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera->m_mtxProjection);

	D3DXMatrixPerspectiveFovLH(&m_pCamera->m_mtxProjection,
		m_fFov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, CAMERA_VIEW_MIN, CAMERA_VIEW_MAX);
	
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera->m_mtxProjection);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CAMERA_BACK_BUFFE, 1.0f, 0);
}

//******************************
// ブレの処理
//******************************
void CCamera::Shake(bool bRand)
{
	if (bRand)
	{// 引数がtrueだったとき

		m_shakeDist = SHAKE_VEC;
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)CScene::GetTop(CScene::OBJTYPE_PLAYER);

		while (pPlayer != NULL)
		{
			if (pPlayer != NULL &&pPlayer->GetPlayerNum() == m_nPlayerNum)
			{
				// プレイヤーが障害物にぶつかったときにカメラをぶらす
				if (pPlayer->GetStan())
				{
					// カウントを進める
					m_nCntShake++;
					// 一定のカウントで
					if (m_nCntShake % SHAKE_COUNT == 0)
					{// 反対方向にぶらしつつちょっとずつぶれをちいさくする
						m_shakeDist *= -0.9f;
					}
				}
				else
				{
					m_nCntShake = 0;
					m_shakeDist = VEC3_ZERO;
				}
				break;
			}

			pPlayer = (CPlayer*)pPlayer->GetNext();
		}

		// ブレの値を目標値に近づける
		m_shake += (m_shakeDist - m_shake)*SHAKE_RATE;
	}
}