//****************************************************************
//
// �����蔻�菈�� [collision.h]
// Author : Suzuki Mahiro
//
//****************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "main.h"

//****************************************************************
//�v���g�^�C�v�錾
//****************************************************************
bool CollisionRectangleAndRectangle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
bool CollisionCircularAndCircular(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2);
bool CollisionRectangleAndCircular(D3DXVECTOR3 RectanglePos, D3DXVECTOR3 CircularPos, D3DXVECTOR3 RectangleSize, float CircularRadius, float RectangleRadian);

#endif