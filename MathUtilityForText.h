#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

//Vector3同士の加算
Vector3& operator+=(Vector3& lhs, const Vector3& rhv);

//行列同士の掛け算
Matrix4x4 MatrixMultply(Matrix4x4& m1, Matrix4x4& m2);

//アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

