//
//  XVector3.h
//  
//
//  Created by easy proger on 26.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef _XVector3_h
#define _XVector3_h

#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>



#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#define EPSILON		1.0e-6f
#define DEG_TO_RAD	0.01745329251994329576f
#define RAD_TO_DEG	57.2957795130823209071f

#define COLORHEX(c) ((unsigned long long)((((unsigned char)(c.a*255.0)|((unsigned short)((unsigned char)(c.b*255.0))<<8))|(((unsigned long)(unsigned char)(c.g*255.0))<<16)) | (((unsigned long long)(unsigned char)(c.r*255.0))<<24)  ))
#define HEXCOLOR(c) XVector4(float((c>>24)&0xFF)/255.0, float((c>>16)&0xFF)/255.0, float((c>>8)&0xFF)/255.0, float((c)&0xFF)/255.0)

typedef unsigned int uint;


enum PrticleSortTypes {NO_SORT_PARTICLE, BACK_TO_FRONT_SORT_PARTICLE, FRONT_TO_BACK_SORT_PARTICLE};

struct MatchPathSeparator
{
	bool operator()(char ch) const
	{
		return ch == '/' || ch == '\\';
	}
};

inline std::string
basename(std::string const& pathname)
{
	return std::string(std::find_if(pathname.rbegin(), pathname.rend(),MatchPathSeparator()).base(),pathname.end());
}


template <class T>
class XVector2 {
public:

	union {
		struct { T x, y; };
		struct { T u, v; };
		struct { T s, t; };
	};

	XVector2(T x, T y) : x(x), y(y) {};

};


class XVector4 {
public:
    union {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        struct { float s, t, p, u; };
        float v[4];
    };
    
    
    
    XVector4(XVector4*in) {
        x = in->x;
        y = in->y;
        z = in->z;
        w = in->w;
    };
    XVector4() : x(0), y(0), z(0), w(0) {};
    XVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
    
    
    void makeWithAxisAndRad(float radians, float xangle, float yangle, float zangle)
    {
        float halfAngle = radians * 0.5f;
        float scale = sinf(halfAngle);
        x = scale * xangle;
        y = scale * yangle;
        z = scale * zangle;
        w = cosf(halfAngle);
    }
    
    
    float length()
    {
#if defined(__ARM_NEON__)
        float32x4_t v = vmulq_f32(*(float32x4_t *)&v,
                                  *(float32x4_t *)&v);
        float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
        v2 = vpadd_f32(v2, v2);
        return sqrt(vget_lane_f32(v2, 0));
#elif defined(X_SSE3_INTRINSICS)
        const __m128 q = _mm_load_ps(&v0]);
        const __m128 product = q * q;
        const __m128 halfsum = _mm_hadd_ps(product, product);
        return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(halfsum, halfsum)));
#else
        return sqrt(v[0] * v[0] +
                    v[1] * v[1] +
                    v[2] * v[2] +
                    v[3] * v[3]);
#endif
    }
    
    void normalize()
    {
        float scale = 1.0f / length();
#if defined(__ARM_NEON__)
        float32x4_t v_out = vmulq_f32(*(float32x4_t *)&v, vdupq_n_f32((float32_t)scale));
        v[0] = v_out[0];
        v[1] = v_out[1];
        v[2] = v_out[2];
        v[3] = v_out[3];
#elif defined(X_SSE3_INTRINSICS)
        const __m128 q = _mm_load_ps(&v[0]);
        __m128 v_out = q * _mm_set1_ps(scale);
        v[0] = v_out[0];
        v[1] = v_out[1];
        v[2] = v_out[2];
        v[3] = v_out[3];
#else
        v[0] = v[0] * scale;
        v[1] = v[1] * scale;
        v[2] = v[2] * scale;
        v[3] = v[3] * scale;
#endif
    }
    
    XVector4& operator=(const XVector4*);
    XVector4 operator*(const XVector4&quatRight) const;
	XVector4 operator+(const XVector4&quatRight) const;
    
    
    
    
    
    
};



inline XVector4 XVector4::operator+(const XVector4&quatRight) const {

#if defined(__ARM_NEON__)
	float32x4_t v = vaddq_f32(*(float32x4_t *)&v,
		*(float32x4_t *)&quatRight);
	return *(XVector4 *)&v;
#elif defined(GLK_SSE3_INTRINSICS)
	__m128 v = _mm_load_ps(&v[0]) + _mm_load_ps(&quatRight.v[0]);
	return *(GLKQuaternion *)&v;
#else
	XVector4 q = XVector4( v[0] + quatRight.v[0],
		v[1] + quatRight.v[1],
		v[2] + quatRight.v[2],
		v[3] + quatRight.v[3] );
	return q;
#endif
};


inline XVector4 XVector4::operator*(const XVector4&quatRight) const {
    
    
#if defined(X_SSE3_INTRINSICS)
    const __m128 ql = _mm_load_ps(&v[0]);
    const __m128 qr = _mm_load_ps(&quatRight.v[0]);
    
    const __m128 ql3012 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(2, 1, 0, 3));
    const __m128 ql3120 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(0, 2, 1, 3));
    const __m128 ql3201 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(1, 0, 2, 3));
    
    const __m128 qr0321 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(1, 2, 3, 0));
    const __m128 qr1302 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(2, 0, 3, 1));
    const __m128 qr2310 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(0, 1, 3, 2));
    const __m128 qr3012 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(2, 1, 0, 3));
    
    uint32_t signBit = 0x80000000;
    uint32_t zeroBit = 0x0;
    uint32_t __attribute__((aligned(16))) mask0001[4] = {zeroBit, zeroBit, zeroBit, signBit};
    uint32_t __attribute__((aligned(16))) mask0111[4] = {zeroBit, signBit, signBit, signBit};
    const __m128 m0001 = _mm_load_ps((float *)mask0001);
    const __m128 m0111 = _mm_load_ps((float *)mask0111);
    
    const __m128 aline = ql3012 * _mm_xor_ps(qr0321, m0001);
    const __m128 bline = ql3120 * _mm_xor_ps(qr1302, m0001);
    const __m128 cline = ql3201 * _mm_xor_ps(qr2310, m0001);
    const __m128 dline = ql3012 * _mm_xor_ps(qr3012, m0111);
    const __m128 r = _mm_hadd_ps(_mm_hadd_ps(aline, bline), _mm_hadd_ps(cline, dline));
    
    return *(XVector4 *)&r;
#else
    
    XVector4 q = {
        v[3] * quatRight.v[0] +
        v[0] * quatRight.v[3] +
        v[1] * quatRight.v[2] -
        v[2] * quatRight.v[1],
        
        v[3] * quatRight.v[1] +
        v[1] * quatRight.v[3] +
        v[2] * quatRight.v[0] -
        v[0] * quatRight.v[2],
        
        v[3] * quatRight.v[2] +
        v[2] * quatRight.v[3] +
        v[0] * quatRight.v[1] -
        v[1] * quatRight.v[0],
        
        v[3] * quatRight.v[3] -
        v[0] * quatRight.v[0] -
        v[1] * quatRight.v[1] -
        v[2] * quatRight.v[2] };
    return q;
#endif
    
};




inline XVector4& XVector4::operator=(const XVector4* vec) {
    x = vec->x; y = vec->y; z = vec->z, w = vec->w;
    return *this;
};




class XVector3 {
public:
	union {
        struct { float x, y, z; };
        struct { float r, g, b; };
        struct { float s, t, p; };
        float v[3];
    };

	XVector3(XVector4 in) {
		x = in.x;
		y = in.y;
		z = in.z;
	};

	XVector3(XVector4*in) {
		x = in->x;
		y = in->y;
		z = in->z;
	};

	XVector3(XVector3*in) {
		x = in->x;
		y = in->y;
		z = in->z;
	};

	XVector3() : x(0), y(0), z(0) {};
    XVector3(float x, float y, float z) : x(x), y(y), z(z) {};

	XVector3& operator=(const XVector4&);
	XVector3& operator=(const XVector4*);
	XVector3& operator=(const XVector3&); 
	XVector3& operator=(const XVector3*);
    
    XVector3  operator-() const;
	XVector3  operator+(const XVector3& rhs) const;
    XVector3& operator+=(const XVector3& rhs);
    XVector3  operator-(const XVector3& rhs) const;
    
    XVector3  operator*(const float scale) const;
	XVector3& operator*=(const float scale);
    
    XVector3  operator/(const float scale) const;
    XVector3& operator/=(const float scale);
    
    bool        operator==(const XVector3& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const XVector3& rhs) const;   // exact compare, no epsilon
    
    float       length() const;                         //
    float       distance(const XVector3& vec) const;     // distance between two vectors
    float       dot(const XVector3& vec) const;          // dot product
	XVector3 cross(const XVector3& rhs) const;
	XVector3& normalize();
};

inline bool XVector3::operator==(const XVector3& rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

inline bool XVector3::operator!=(const XVector3& rhs) const {
    return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

inline XVector3& XVector3::operator+=(const XVector3& rhs) {
    x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline XVector3 XVector3::operator-() const {
    return XVector3(-x, -y, -z);
}


inline XVector3 XVector3::operator*(const float a) const {
    return XVector3(x*a, y*a, z*a);
}

inline XVector3 XVector3::operator/(const float a) const {
    return XVector3(x/a, y/a, z/a);
}

inline XVector3& XVector3::operator/=(const float a) {
    x /= a; y /= a; z /= a; return *this;
}

inline float XVector3::distance(const XVector3& vec) const {
    return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y) + (vec.z-z)*(vec.z-z));
}

inline float XVector3::length() const {
    return sqrtf(x*x + y*y + z*z);
}
inline float XVector3::dot(const XVector3& rhs) const {
    return (x*rhs.x + y*rhs.y + z*rhs.z);
}

inline XVector3 XVector3::operator-(const XVector3& rhs) const {
    return XVector3(x-rhs.x, y-rhs.y, z-rhs.z);
}

inline float XinvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;          // get bits for floating value
	i = 0x5f3759df - (i >> 1);    // gives initial guess
	x = *(float*)&i;            // convert bits back to float
	x = x * (1.5f - xhalf*x*x); // Newton step
	return x;
}

inline XVector3& XVector3::normalize() {
	float invLength = XinvSqrt(x*x + y*y + z*z);
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return *this;
}

inline XVector3 XVector3::cross(const XVector3& rhs) const {
	return XVector3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

inline XVector3& XVector3::operator=(const XVector4* vec) {
	x = vec->x; y = vec->y; z = vec->z;
	return *this;
};

inline XVector3& XVector3::operator=(const XVector4& vec) {
	x = vec.x; y = vec.y; z = vec.z;
	return *this;
};


inline XVector3& XVector3::operator=(const XVector3* vec) {
    x = vec->x; y = vec->y; z = vec->z;
    return *this;
};

inline XVector3& XVector3::operator=(const XVector3& vec) {
    x = vec.x; y = vec.y; z = vec.z;
    return *this;
};


inline XVector3 XVector3::operator+(const XVector3& rhs) const {
    return XVector3(x+rhs.x, y+rhs.y, z+rhs.z);
}

inline XVector3& XVector3::operator*=(const float a) {
    x *= a; y *= a; z *= a; return *this;
}




inline XVector3 Xnormalize(const XVector3 &v);
inline float Xdot(const XVector3 &a, const XVector3 &b);
inline XVector3 Xcross(const XVector3 &a, const XVector3 &b);
inline float Xarccos(float x);

inline XVector3 Xnormalize(const XVector3 &v) {

	float len = v.length();
	if (fabsf(len) > EPSILON) {
		float inv = 1.0f / len;
		return XVector3(v.x * inv, v.y * inv, v.z * inv);
	}
	else {
		return v;
	}
}

inline float Xdot(const XVector3 &a, const XVector3 &b) {
	return
		a.x * b.x +
		a.y * b.y +
		a.z * b.z;
}

inline XVector3 Xcross(const XVector3 &a, const XVector3 &b) {

	return XVector3(
		a.y * b.z - b.y * a.z,
		a.z * b.x - b.z * a.x,
		a.x * b.y - b.x * a.y
		);
}
inline float Xarccos(float x) {
	double omxx = 1.0 - double(x) * double(x);
	if (omxx < 0.0) omxx = 0.0;
	return atan2f((float)sqrt(omxx), x);
}






class XMatrix4 {
public:
	union {
    	struct {
        	float m00, m01, m02, m03;
        	float m10, m11, m12, m13;
        	float m20, m21, m22, m23;
        	float m30, m31, m32, m33;
    	};
    	float m[16];
	} 
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) 
	
#else
	__attribute__((aligned(16)))
#endif
	;

	void makeWithQuat(XVector4* quat) {
		float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

		x2 = quat->x + quat->x; y2 = quat->y + quat->y; z2 = quat->z + quat->z;
		xx = quat->x * x2;     xy = quat->x * y2;     xz = quat->x * z2;
		yy = quat->y * y2;     yz = quat->y * z2;     zz = quat->z * z2;
		wx = quat->w * x2;     wy = quat->w * y2;     wz = quat->w * z2;


		m[0] = 1 - (yy + zz);  m[4] = xy + wz;        m[8] = xz - wy;
		m[1] = xy - wz;        m[5] = 1 - (xx + zz);  m[9] = yz + wx;
		m[2] = xz + wy;        m[6] = yz - wx;        m[10] = 1 - (xx + yy);

		m[3] = 0.0f;
		m[7] = 0.0f;
		m[11] = 0.0f;

		m[12] = 0.0;
		m[13] = 0.0;
		m[14] = 0.0;
		m[15] = 1.0f;
	}
   

	XMatrix4() :
		m00(1.0), m01(0.0), m02(0.0),m03(0.0),
		m10(0.0), m11(1.0), m12(0.0),m13(0.0),
		m20(0.0), m21(0.0), m22(1.0),m23(0.0),
		m30(0.0), m31(0.0), m32(0.0),m33(1.0)
	{};
    XMatrix4(float m1,float m2,float m3,float m4,float m5,float m6,float m7,float m8,float m9,float m10,float m11,float m12,float m13,float m14,float m15,float m16) : 
		m00(m1),  m01(m2),  m02(m3), m03(m4),
		m10(m5),  m11(m6),  m12(m7), m13(m8),
		m20(m9),  m21(m10), m22(m11),m23(m12),
		m30(m13), m31(m14), m32(m15),m33(m16) {};

	void identity() {
		m00 = 1.0; m01 = 0.0; m02 = 0.0; m03 = 0.0;
		m10 = 0.0; m11 = 1.0; m12 = 0.0; m13 = 0.0;
		m20 = 0.0; m21 = 0.0; m22 = 1.0; m23 = 0.0;
		m30 = 0.0; m31 = 0.0; m32 = 0.0; m33 = 1.0;
	}

	XVector4 operator*(const XVector4& vectorRight) const;
	XMatrix4 operator*(const XMatrix4& matrixRight) const; 
};

inline XVector4 XMatrix4::operator*(const XVector4& vectorRight) const {
#if defined(__ARM_NEON__)
	float32x4x4_t iMatrix = *(float32x4x4_t *)&this->m[0];
	float32x4_t v;

	iMatrix.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)vectorRight.v[0]);
	iMatrix.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)vectorRight.v[1]);
	iMatrix.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)vectorRight.v[2]);
	iMatrix.val[3] = vmulq_n_f32(iMatrix.val[3], (float32_t)vectorRight.v[3]);

	iMatrix.val[0] = vaddq_f32(iMatrix.val[0], iMatrix.val[1]);
	iMatrix.val[2] = vaddq_f32(iMatrix.val[2], iMatrix.val[3]);

	v = vaddq_f32(iMatrix.val[0], iMatrix.val[2]);

	return *(XVector4 *)&v;
#elif defined(X_SSE3_INTRINSICS)
	const __m128 v = _mm_load_ps(&vectorRight.v[0]);

	const __m128 r = _mm_load_ps(&m[0])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0))
		+ _mm_load_ps(&m[4])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))
		+ _mm_load_ps(&m[8])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))
		+ _mm_load_ps(&m[12]) * _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));

	XVector4 ret;
	*(__m128*)&ret = r;
	return ret;
#else
	XVector4 v = XVector4(
        m[0] * vectorRight.v[0] + m[4] * vectorRight.v[1] + m[8] * vectorRight.v[2] + m[12] * vectorRight.v[3],
		m[1] * vectorRight.v[0] + m[5] * vectorRight.v[1] + m[9] * vectorRight.v[2] + m[13] * vectorRight.v[3],
		m[2] * vectorRight.v[0] + m[6] * vectorRight.v[1] + m[10] * vectorRight.v[2] + m[14] * vectorRight.v[3],
		m[3] * vectorRight.v[0] + m[7] * vectorRight.v[1] + m[11] * vectorRight.v[2] + m[15] * vectorRight.v[3] );
	return v;
#endif
}

inline XMatrix4 XMatrix4::operator*(const XMatrix4& matrixRight) const {
	XMatrix4 result;
#if defined(__ARM_NEON__)
    
    const float32x4x4_t* __restrict__ m1 = (float32x4x4_t*)this;
    const float32x4x4_t* __restrict__ m2 = (float32x4x4_t*)&matrixRight;
    float32x4x4_t* __restrict__ r = (float32x4x4_t*)&result;
    
    (*r).val[0] = vmulq_n_f32((*m1).val[0], vgetq_lane_f32((*m2).val[0], 0));
    (*r).val[1] = vmulq_n_f32((*m1).val[0], vgetq_lane_f32((*m2).val[1], 0));
    (*r).val[2] = vmulq_n_f32((*m1).val[0], vgetq_lane_f32((*m2).val[2], 0));
    (*r).val[3] = vmulq_n_f32((*m1).val[0], vgetq_lane_f32((*m2).val[3], 0));
    
    (*r).val[0] = vmlaq_n_f32((*r).val[0], (*m1).val[1], vgetq_lane_f32((*m2).val[0], 1));
    (*r).val[1] = vmlaq_n_f32((*r).val[1], (*m1).val[1], vgetq_lane_f32((*m2).val[1], 1));
    (*r).val[2] = vmlaq_n_f32((*r).val[2], (*m1).val[1], vgetq_lane_f32((*m2).val[2], 1));
    (*r).val[3] = vmlaq_n_f32((*r).val[3], (*m1).val[1], vgetq_lane_f32((*m2).val[3], 1));
    
    (*r).val[0] = vmlaq_n_f32((*r).val[0], (*m1).val[2], vgetq_lane_f32((*m2).val[0], 2));
    (*r).val[1] = vmlaq_n_f32((*r).val[1], (*m1).val[2], vgetq_lane_f32((*m2).val[1], 2));
    (*r).val[2] = vmlaq_n_f32((*r).val[2], (*m1).val[2], vgetq_lane_f32((*m2).val[2], 2));
    (*r).val[3] = vmlaq_n_f32((*r).val[3], (*m1).val[2], vgetq_lane_f32((*m2).val[3], 2));
    
    (*r).val[0] = vmlaq_n_f32((*r).val[0], (*m1).val[3], vgetq_lane_f32((*m2).val[0], 3));
    (*r).val[1] = vmlaq_n_f32((*r).val[1], (*m1).val[3], vgetq_lane_f32((*m2).val[1], 3));
    (*r).val[2] = vmlaq_n_f32((*r).val[2], (*m1).val[3], vgetq_lane_f32((*m2).val[2], 3));
    (*r).val[3] = vmlaq_n_f32((*r).val[3], (*m1).val[3], vgetq_lane_f32((*m2).val[3], 3));
	return *(XMatrix4*)r;
#else
    
    result.m[0]  = m[0] * matrixRight.m[0]  + m[4] * matrixRight.m[1]  + m[8] * matrixRight.m[2]   + m[12] * matrixRight.m[3];
    result.m[4]  = m[0] * matrixRight.m[4]  + m[4] * matrixRight.m[5]  + m[8] * matrixRight.m[6]   + m[12] * matrixRight.m[7];
    result.m[8]  = m[0] * matrixRight.m[8]  + m[4] * matrixRight.m[9]  + m[8] * matrixRight.m[10]  + m[12] * matrixRight.m[11];
    result.m[12] = m[0] * matrixRight.m[12] + m[4] * matrixRight.m[13] + m[8] * matrixRight.m[14]  + m[12] * matrixRight.m[15];
    
    result.m[1]  = m[1] * matrixRight.m[0]  + m[5] * matrixRight.m[1]  + m[9] * matrixRight.m[2]   + m[13] * matrixRight.m[3];
    result.m[5]  = m[1] * matrixRight.m[4]  + m[5] * matrixRight.m[5]  + m[9] * matrixRight.m[6]   + m[13] * matrixRight.m[7];
    result.m[9]  = m[1] * matrixRight.m[8]  + m[5] * matrixRight.m[9]  + m[9] * matrixRight.m[10]  + m[13] * matrixRight.m[11];
    result.m[13] = m[1] * matrixRight.m[12] + m[5] * matrixRight.m[13] + m[9] * matrixRight.m[14]  + m[13] * matrixRight.m[15];
    
    result.m[2]  = m[2] * matrixRight.m[0]  + m[6] * matrixRight.m[1]  + m[10] * matrixRight.m[2]  + m[14] * matrixRight.m[3];
    result.m[6]  = m[2] * matrixRight.m[4]  + m[6] * matrixRight.m[5]  + m[10] * matrixRight.m[6]  + m[14] * matrixRight.m[7];
    result.m[10] = m[2] * matrixRight.m[8]  + m[6] * matrixRight.m[9]  + m[10] * matrixRight.m[10] + m[14] * matrixRight.m[11];
    result.m[14] = m[2] * matrixRight.m[12] + m[6] * matrixRight.m[13] + m[10] * matrixRight.m[14] + m[14] * matrixRight.m[15];
    
    result.m[3]  = m[3] * matrixRight.m[0]  + m[7] * matrixRight.m[1]  + m[11] * matrixRight.m[2]  + m[15] * matrixRight.m[3];
    result.m[7]  = m[3] * matrixRight.m[4]  + m[7] * matrixRight.m[5]  + m[11] * matrixRight.m[6]  + m[15] * matrixRight.m[7];
    result.m[11] = m[3] * matrixRight.m[8]  + m[7] * matrixRight.m[9]  + m[11] * matrixRight.m[10] + m[15] * matrixRight.m[11];
    result.m[15] = m[3] * matrixRight.m[12] + m[7] * matrixRight.m[13] + m[11] * matrixRight.m[14] + m[15] * matrixRight.m[15];
    
	return result;
#endif
}




class XMatrix3 {
public:

	union {
    	struct {
        	float m00, m01, m02;
        	float m10, m11, m12;
        	float m20, m21, m22;
    	};
    	float m[9];
	};

	XMatrix3() : 
		m00(1.0), m01(0.0), m02(0.0),
		m10(0.0), m11(1.0), m12(0.0),
		m20(0.0), m21(0.0), m22(1.0)
	{};
    XMatrix3(float m1,float m2,float m3,float m5,float m6,float m7,float m9,float m10,float m11) : 
		m00(m1),  m01(m2),  m02(m3), 
		m10(m5),  m11(m6),  m12(m7),  
		m20(m9),  m21(m10), m22(m11) {};

	void identity() {
		m00 = 1.0; m01 = 0.0; m02 = 0.0;
		m10 = 0.0; m11 = 1.0; m12 = 0.0; 
		m20 = 0.0; m21 = 0.0; m22 = 1.0; 
	}
};



enum TypeGenericObjects {
	GenericObject_PARTICLE
};

class GenericObjects {
public:
	TypeGenericObjects typeGeneric;
};



class ParticleInfo:public GenericObjects {
public:

	PrticleSortTypes typeSort;
	uint      blendIn;
	uint      blendOut;

	bool      blend;

	XVector3  nodePosition;
	XVector3  nodeScale;
	XVector3  position;

	bool      colorInterpolate;

	float	  azimuth;
	float	  pitch;
	float	  azimuthVariance;
	float	  pitchVariance;
	XVector4  startColor;
	XVector4  startColorVariance;
	XVector4  finishColor;
	XVector4  finishColorVariance;
	float	  speed;
	float	  speedVariance;
	XVector3  force;
	XVector3  forceVariance;


	float	  particleSize;
	float	  particleSizeVariance;
	float	  particlesEmittedPerSecond;
	float	  particleEmitVariance;
	float	  particleLifespan;
	float	  particleLifespanVariance;




	ParticleInfo() : nodeScale(1,1,1)
	{};
	ParticleInfo& operator=(const ParticleInfo&);
	ParticleInfo& operator=(const ParticleInfo*);
};

inline ParticleInfo& ParticleInfo::operator=(const ParticleInfo& vec) {
	blendOut                  = vec.blendOut;
	blendIn                   = vec.blendIn;
	blend                     = vec.blend;
	typeSort                  = vec.typeSort;
	pitch                     = vec.pitch;
	azimuth                   = vec.azimuth;
	nodePosition              = vec.nodePosition;
	nodeScale                 = vec.nodeScale;
	position                  = vec.position;
	colorInterpolate          = vec.colorInterpolate;
	azimuthVariance           = vec.azimuthVariance;
	pitchVariance             = vec.pitchVariance;
	speed                     = vec.speed;
	speedVariance             = vec.speedVariance;
	startColor                = vec.startColor;
	startColorVariance        = vec.startColorVariance;
	finishColor               = vec.finishColor;
	finishColorVariance       = vec.finishColorVariance;
	force                     = vec.force;
	forceVariance             = vec.forceVariance;
	particleSize              = vec.particleSize;
	particleSizeVariance      = vec.particleSizeVariance;
	particlesEmittedPerSecond = vec.particlesEmittedPerSecond;
	particleEmitVariance      = vec.particleEmitVariance;
	particleLifespan          = vec.particleLifespan;
    particleLifespanVariance  = vec.particleLifespanVariance;
	return *this;
};
inline ParticleInfo& ParticleInfo::operator=(const ParticleInfo* vec) {
	*this = *vec;
	return *this;
};

class CameraInfo {
public:
	int typeCamera;
	XVector3 lookAtPos;
	XVector3 lookAtView;
	XVector3 lookAtUp;
	float unused[20];

	CameraInfo& operator=(const CameraInfo&);
	CameraInfo& operator=(const CameraInfo*);
};


inline CameraInfo& CameraInfo::operator=(const CameraInfo& vec) {
	typeCamera = vec.typeCamera;
	lookAtPos = vec.lookAtPos;
	lookAtView = vec.lookAtView;
	lookAtUp = vec.lookAtUp;
	return *this;
};

inline CameraInfo& CameraInfo::operator=(const CameraInfo* vec) {
	typeCamera = vec->typeCamera;
	lookAtPos = vec->lookAtPos;
	lookAtView = vec->lookAtView;
	lookAtUp = vec->lookAtUp;
	return *this;
};



class LightInfo {
public:
	int typeLight;
	XVector4 quaternion;
	XVector3 translate;
	int typeShadow;
	float unused[20];
	LightInfo& operator=(const LightInfo&);
	LightInfo& operator=(const LightInfo*);
};


inline LightInfo& LightInfo::operator=(const LightInfo& vec) {
	typeLight = vec.typeLight;
	quaternion = vec.quaternion;
	translate = vec.translate;
	typeShadow = vec.typeShadow;
	return *this;
};

inline LightInfo& LightInfo::operator=(const LightInfo* vec) {
	typeLight = vec->typeLight;
	quaternion = vec->quaternion;
	translate = vec->translate;
	typeShadow = vec->typeShadow;
	return *this;
};


struct XTracePoint {

	XVector3 point;
	XVector3 normal;
	XVector3 texcoord;

};


class XLine {

public:

	XVector3 src;
	XVector3 dst;

	XLine() { }
	XLine(const XVector3 &s, const XVector3 &d) : src(s), dst(d) { }

	XVector3 getDirection() const { return (dst - src); }
};


class  XPlane {
    
public:
    XVector3 normal;
    float dist;
    
    XPlane() : normal(0.0f, 0.0f, 1.0f), dist(0.0f) {}
    
    XPlane(const XVector3 &n, float d) : normal(n), dist(d) {}
    XPlane(const XVector3 &, const XVector3 &);
    
    XVector3 getOrigin();
    void setOrigin(const XVector3 &);
    
    void normalize();
    friend XPlane normalize(const XPlane &);
    bool trace(const XLine &l, XTracePoint &i, bool fixSrc, bool fixDst) const;
};



#endif
