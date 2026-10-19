#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class MyMath {
public:
	// 4x4行列の掛け算
	static Matrix4x4 MatrixMultiply(const Matrix4x4& a, const Matrix4x4& b);

	// アフィン変換行列の生成
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
};
