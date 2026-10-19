#include "MyMath.h"
#include <cmath>

using namespace KamataEngine;

// 4x4行列の掛け算
Matrix4x4 MyMath::MatrixMultiply(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result{};

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			for (int k = 0; k < 4; ++k) {
				result.m[row][column] += a.m[row][k] * b.m[k][column];
			}
		}
	}

	return result;
}

// アフィン変換行列の生成
Matrix4x4 MyMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	const float cosX = std::cos(rotate.x);
	const float sinX = std::sin(rotate.x);
	const float cosY = std::cos(rotate.y);
	const float sinY = std::sin(rotate.y);
	const float cosZ = std::cos(rotate.z);
	const float sinZ = std::sin(rotate.z);

	// X軸回転
	Matrix4x4 rotateX = {};
	rotateX.m[0][0] = 1.0f;
	rotateX.m[1][1] = cosX;
	rotateX.m[1][2] = sinX;
	rotateX.m[2][1] = -sinX;
	rotateX.m[2][2] = cosX;
	rotateX.m[3][3] = 1.0f;

	// Y軸回転
	Matrix4x4 rotateY = {};
	rotateY.m[0][0] = cosY;
	rotateY.m[0][2] = -sinY;
	rotateY.m[1][1] = 1.0f;
	rotateY.m[2][0] = sinY;
	rotateY.m[2][2] = cosY;
	rotateY.m[3][3] = 1.0f;

	// Z軸回転
	Matrix4x4 rotateZ = {};
	rotateZ.m[0][0] = cosZ;
	rotateZ.m[0][1] = sinZ;
	rotateZ.m[1][0] = -sinZ;
	rotateZ.m[1][1] = cosZ;
	rotateZ.m[2][2] = 1.0f;
	rotateZ.m[3][3] = 1.0f;

	// 拡大縮小
	Matrix4x4 scaling = {};
	scaling.m[0][0] = scale.x;
	scaling.m[1][1] = scale.y;
	scaling.m[2][2] = scale.z;
	scaling.m[3][3] = 1.0f;

	// 平行移動
	Matrix4x4 translation = {};
	translation.m[0][0] = 1.0f;
	translation.m[1][1] = 1.0f;
	translation.m[2][2] = 1.0f;
	translation.m[3][0] = translate.x;
	translation.m[3][1] = translate.y;
	translation.m[3][2] = translate.z;
	translation.m[3][3] = 1.0f;

	// S × Rx × Ry × Rz × T
	Matrix4x4 result = MatrixMultiply(scaling, rotateX);
	result = MatrixMultiply(result, rotateY);
	result = MatrixMultiply(result, rotateZ);
	result = MatrixMultiply(result, translation);

	return result;
}
