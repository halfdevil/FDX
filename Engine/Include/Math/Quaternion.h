#pragma once

#include "Math/Vector3F.h"
#include "Math/Matrix3F.h"
#include "Math/Matrix4F.h"

namespace FDX
{
	class FDX_EXPORT CQuaternion
	{
	public:

		inline CQuaternion();
		inline CQuaternion(float pfYaw, float pfPitch, float pfRoll);
		inline CQuaternion(float pfX, float pfY, float pfZ, float pfW);

		inline float GetLength();
		inline float Normalize();
		inline float Dot(const CQuaternion &pQ) const;

		inline void GenerateW();
		inline void Conjugate(const CQuaternion &pQ);
		inline void Set(float pfX, float pfY, float pfZ, float pfW = 1.0f);
		inline void SetEulers(float pfYaw, float pfPitch, float pfRoll);
		inline void GetEulers(float &pfYaw, float &pfPitch, float &pfRoll);
		inline void Rotate(const CQuaternion &pA, const CQuaternion &pB);

		inline CMatrix3F GetMatrix3() const;
		inline CMatrix4F GetMatrix4() const;
		
		inline CVector3F Rotate(const CVector3F &pV) const;
		inline CQuaternion Lerp(const CQuaternion &pQ, float pfFactor) const;

		inline CQuaternion operator + (const CQuaternion &pQ) const;
		inline CQuaternion operator * (const CQuaternion &pQ) const;

		inline CQuaternion operator + (const CVector3F &pV) const;
		inline CQuaternion& operator += (const CVector3F &pV);

		inline CQuaternion& operator += (const CQuaternion &pQ);
		inline CQuaternion& operator *= (const CQuaternion &pQ);

		inline CQuaternion operator * (float pfVal) const;
		inline CQuaternion& operator *= (float pfVal);

		inline CQuaternion operator ~() const;
		inline CQuaternion operator *(const CVector3F &pV) const;

	public:

		static CQuaternion FromEulers(float pfYaw, float pfPitch, float pfRoll);
		static CQuaternion FromAxisAngle(const CVector3F &pAxis, float pfAngle);
		static CQuaternion FromMatrix(const CMatrix3F &pMat);
		static CQuaternion FromMatrix(const CMatrix4F &pMat);

	public:

		float X;
		float Y;
		float Z;
		float W;
	};

	inline CQuaternion::CQuaternion()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{
	}

	inline CQuaternion::CQuaternion(float pfYaw, float pfPitch, float pfRoll)
	{
		SetEulers(pfYaw, pfPitch, pfRoll);
	}

	inline CQuaternion::CQuaternion(float pfX, float pfY, float pfZ, float pfW)
		: X(pfX), Y(pfY), Z(pfZ), W(pfW)
	{
	}

	inline float CQuaternion::GetLength()
	{
		return CMathUtil::Sqrt(X * X + Y * Y + Z * Z + W * W);
	}

	inline float CQuaternion::Normalize()
	{
		float lfLen = CMathUtil::Sqrt(X * X + Y * Y + Z * Z + W * W);

		if (lfLen != 0.0f)
		{
			float lfInvLen = 1.0f / lfLen;

			X *= lfInvLen;
			Y *= lfInvLen;
			Z *= lfInvLen;
			W *= lfInvLen;
		}

		return lfLen;
	}

	inline float CQuaternion::Dot(const CQuaternion & pQ) const
	{
		return X * pQ.X + Y * pQ.Y + Z * pQ.Z + W * pQ.W;
	}

	inline void CQuaternion::GenerateW()
	{
		float lfT = 1.0f - (X * X) - (Y * Y) - (Z * Z);

		if (lfT < 0.0f)
			W = 0.0f;
		else
			W = -CMathUtil::Sqrt(lfT);
	}

	inline void CQuaternion::Conjugate(const CQuaternion & pQ)
	{
		X = -pQ.X;
		Y = -pQ.Y;
		Z = -pQ.Z;
		W = pQ.W;
	}

	inline void CQuaternion::SetEulers(float pfYaw, float pfPitch, float pfRoll)
	{
		float lfCx, lfCy, lfCz;
		float lfSx, lfSy, lfSz;
		float lfCyCz, lfSySz, lfCySz, lfSyCz;

		pfPitch *= 0.5f;
		pfYaw *= 0.5f;
		pfRoll *= 0.5f;

		lfCx = CMathUtil::Cos(pfPitch);
		lfCy = CMathUtil::Cos(pfYaw);
		lfCz = CMathUtil::Cos(pfRoll);

		lfSx = CMathUtil::Sin(pfPitch);
		lfSy = CMathUtil::Sin(pfYaw);
		lfSz = CMathUtil::Sin(pfRoll);

		lfCyCz = lfCy * lfCz;
		lfSySz = lfSy * lfSz;
		lfCySz = lfCy * lfSz;
		lfSyCz = lfSy * lfCz;

		W = lfCx * lfCyCz + lfSx * lfSySz;
		X = lfSx * lfCyCz - lfCx * lfSySz;
		Y = lfCx * lfSyCz + lfSx * lfCySz;
		Z = lfCx * lfCySz - lfSx * lfSyCz;
	}

	inline void CQuaternion::Set(float pfX, float pfY, float pfZ, float pfW)
	{
		X = pfX;
		Y = pfY;
		Z = pfZ;
		W = pfW;
	}

	inline void CQuaternion::GetEulers(float & pfYaw, float & pfPitch, float & pfRoll)
	{
		double ldR11, ldR21, ldR31, ldR32, ldR33, ldR12, ldR13;
		double ldQ00, ldQ11, ldQ22, ldQ33;
		double ldTmp;

		ldQ00 = W * W;
		ldQ11 = X * X;
		ldQ22 = Y * Y;
		ldQ33 = Z * Z;

		ldR11 = ldQ00 + ldQ11 - ldQ22 - ldQ33;
		ldR21 = 2 * (X * Y + W * Z);
		ldR31 = 2 * (X * Z - W * Y);
		ldR32 = 2 * (Y * Z + W * X);
		ldR33 = ldQ00 - ldQ11 - ldQ22 + ldQ33;

		ldTmp = CMathUtil::Abs(ldR32);
		if (ldTmp > 0.999999)
		{
			ldR12 = 2 * (X * Y - W * Z);
			ldR13 = 2 * (X * Z + W * Y);

			pfPitch = 0.0f;
			pfYaw = static_cast<float>(-(PI / 2 * ldR31 / ldTmp));
			pfRoll = static_cast<float>(CMathUtil::ATan2(-ldR12, -ldR31 * ldR13));
		}
		else
		{
			pfPitch = static_cast<float>(CMathUtil::ATan2(ldR32, ldR33));
			pfYaw = static_cast<float>(CMathUtil::ASin(-ldR31));
			pfRoll = static_cast<float>(CMathUtil::ATan2(ldR21, ldR11));
		}
	}

	inline void CQuaternion::Rotate(const CQuaternion & pA, const CQuaternion & pB)
	{
		CQuaternion lT = pA * pB * (~pA);

		X = lT.X;
		Y = lT.Y;
		Z = lT.Z;
		W = lT.W;
	}

	inline CMatrix3F CQuaternion::GetMatrix3() const
	{
		CMatrix3F lMat = CMatrix3F::ZeroMatrix();

		float lfWx, lfWy, lfWz;
		float lfXx, lfYy, lfZz;
		float lfXy, lfYz, lfXz;
		float lfX2, lfY2, lfZ2;

		lfX2 = X + X;
		lfY2 = Y + Y;
		lfZ2 = Z + Z;

		lfXx = X * lfX2;
		lfXy = X * lfY2;
		lfXz = X * lfZ2;

		lfYy = Y * lfY2;
		lfYz = Y * lfZ2;
		lfZz = Z * lfZ2;

		lfWx = W * lfX2;
		lfWy = W * lfY2;
		lfWz = W * lfZ2;

		lMat.M2[0][0] = 1.0f - (lfYy + lfZz);
		lMat.M2[0][1] = lfXy - lfWz;
		lMat.M2[0][2] = lfXz + lfWy;

		lMat.M2[1][0] = lfXy + lfWz;
		lMat.M2[1][1] = 1.0f - (lfXx + lfZz);
		lMat.M2[1][2] = lfYz - lfWx;

		lMat.M2[2][0] = lfXz - lfWy;
		lMat.M2[2][1] = lfYz + lfWx;
		lMat.M2[2][2] = 1.0f - (lfXx + lfYy);

		return lMat;
	}

	inline CMatrix4F CQuaternion::GetMatrix4() const
	{
		CMatrix4F lMat = CMatrix4F::ZeroMatrix();
		lMat.M2[3][3] = 1.0f;

		float lfWx, lfWy, lfWz;
		float lfXx, lfYy, lfZz;
		float lfXy, lfYz, lfXz;
		float lfX2, lfY2, lfZ2;

		lfX2 = X + X;
		lfY2 = Y + Y;
		lfZ2 = Z + Z;

		lfXx = X * lfX2;
		lfXy = X * lfY2;
		lfXz = X * lfZ2;

		lfYy = Y * lfY2;
		lfYz = Y * lfZ2;
		lfZz = Z * lfZ2;

		lfWx = W * lfX2;
		lfWy = W * lfY2;
		lfWz = W * lfZ2;

		lMat.M2[0][0] = 1.0f - (lfYy + lfZz);
		lMat.M2[0][1] = lfXy - lfWz;
		lMat.M2[0][2] = lfXz + lfWy;

		lMat.M2[1][0] = lfXy + lfWz;
		lMat.M2[1][1] = 1.0f - (lfXx + lfZz);
		lMat.M2[1][2] = lfYz - lfWx;

		lMat.M2[2][0] = lfXz - lfWy;
		lMat.M2[2][1] = lfYz + lfWx;
		lMat.M2[2][2] = 1.0f - (lfXx + lfYy);

		return lMat;
	}

	inline CVector3F CQuaternion::Rotate(const CVector3F & pV) const
	{
		CQuaternion lQ(X, Y, Z, W);
		CQuaternion lR = lQ * pV * (~lQ);

		return CVector3F(lR.X, lR.Y, lR.Z);
	}

	inline CQuaternion CQuaternion::Lerp(const CQuaternion & pQ, float pfFactor) const
	{
		CQuaternion lResult;
		float lfInvFactor = 1.0f - pfFactor;

		if (Dot(pQ) < 0)
		{
			lResult.W = lfInvFactor * W + pfFactor * -pQ.W;
			lResult.X = lfInvFactor * X + pfFactor * -pQ.X;
			lResult.Y = lfInvFactor * Y + pfFactor * -pQ.Y;
			lResult.Z = lfInvFactor * Z + pfFactor * -pQ.Z;
		}
		else
		{
			lResult.W = lfInvFactor * W + pfFactor * pQ.W;
			lResult.X = lfInvFactor * X + pfFactor * pQ.X;
			lResult.Y = lfInvFactor * Y + pfFactor * pQ.Y;
			lResult.Z = lfInvFactor * Z + pfFactor * pQ.Z;
		}

		return lResult;
	}

	inline CQuaternion CQuaternion::operator+(const CQuaternion & pQ) const
	{
		return CQuaternion(X + pQ.X, Y + pQ.Y, Z + pQ.Z, W + pQ.W);
	}

	inline CQuaternion CQuaternion::operator*(const CQuaternion & pQ) const
	{
		return CQuaternion(
			W * pQ.X + X * pQ.W + Y * pQ.Z - Z * pQ.Y,
			W * pQ.Y + Y * pQ.W + Z * pQ.X - X * pQ.Z,
			W * pQ.Z + Z * pQ.W + X * pQ.Y - Y * pQ.X,
			W * pQ.W - X * pQ.X - Y * pQ.Y - Z * pQ.Z
		);
	}

	inline CQuaternion CQuaternion::operator+(const CVector3F & pV) const
	{
		CQuaternion lQ(pV.X, pV.Y, pV.Z, 0.0f);
		lQ *= *this;

		lQ.X = X + lQ.X * 0.5f;
		lQ.Y = Y + lQ.Y * 0.5f;
		lQ.Z = Z + lQ.Z * 0.5f;
		lQ.W = W + lQ.W * 0.5f;

		return lQ;
	}

	inline CQuaternion & CQuaternion::operator+=(const CVector3F & pV)
	{
		CQuaternion lQ(pV.X, pV.Y, pV.Z, 0.0f);
		lQ *= *this;

		X += lQ.X * 0.5f;
		Y += lQ.Y * 0.5f;
		Z += lQ.Z * 0.5f;
		W += lQ.W * 0.5f;

		return *this;
	}

	inline CQuaternion & CQuaternion::operator+=(const CQuaternion & pQ)
	{
		X += pQ.X;
		Y += pQ.Y;
		Z += pQ.Z;
		W += pQ.W;

		return *this;
	}

	inline CQuaternion & CQuaternion::operator*=(const CQuaternion & pQ)
	{
		float lfX, lfY, lfZ, lfW;

		lfX = W * pQ.X + X * pQ.W + Y * pQ.Z - Z * pQ.Y;
		lfY = W * pQ.Y + Y * pQ.W + Z * pQ.X - X * pQ.Z;
		lfZ = W * pQ.Z + Z * pQ.W + X * pQ.Y - Y * pQ.X;
		lfW = W * pQ.W - X * pQ.X - Y * pQ.Y - Z * pQ.Z;

		X = lfX;
		Y = lfY;
		Z = lfZ;
		W = lfW;

		return *this;
	}

	inline CQuaternion CQuaternion::operator*(float pfVal) const
	{
		return CQuaternion(X * pfVal, Y * pfVal, Z * pfVal, W * pfVal);
	}

	inline CQuaternion & CQuaternion::operator*=(float pfVal)
	{
		X *= pfVal;
		Y *= pfVal;
		Z *= pfVal;
		W *= pfVal;

		return *this;
	}
	inline CQuaternion CQuaternion::operator~() const
	{
		return CQuaternion(-X, -Y, -Z, W);
	}

	inline CQuaternion CQuaternion::operator*(const CVector3F & pV) const
	{
		return CQuaternion(
			W * pV.X + Y * pV.Z - Z * pV.Y,
			W * pV.Y + Z * pV.X - X * pV.Z,
			W * pV.Z + X * pV.Y - Y * pV.X,
			-(X * pV.X + Y * pV.Y + Z * pV.Z)
		);
	}
}