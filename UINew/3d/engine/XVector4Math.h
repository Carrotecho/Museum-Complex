//
//  XVector4Math.h
//  
//
//  Created by easy proger on 26.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef _XVector4Math_h
#define _XVector4Math_h


#define X_INLINE	static __inline__


#include "XTypes.h"
#include "XMathTypes.h"

#include <math.h>


AGRESIVE_INLINE void  XVector4Make(XVector4 *in,float x,float y,float z,float w);
AGRESIVE_INLINE float XVector4Dot(const XVector4 *vectorLeft,const XVector4 *vectorRight);
AGRESIVE_INLINE float XVector4Dot(const XVector4 *vectorLeft,const XVector3 *vectorRight,float wRight);

AGRESIVE_INLINE void  XQuaternion4Slerp(XVector4 * res, XVector4 * q, XVector4 * p, float t);
AGRESIVE_INLINE float XQuaternionLength(XVector4 *quaternion);
AGRESIVE_INLINE void  XQuaternionNormalize(XVector4*res, XVector4* quaternion);
AGRESIVE_INLINE void  XQuaternionNormalize(XVector4*res, XVector4* quaternion);
AGRESIVE_INLINE float XQuaternionLength(XVector4 *quaternion);
AGRESIVE_INLINE void  XQuaternionMakeWithMatrix(XVector4*result, XMatrix4 *mat);
AGRESIVE_INLINE void  XQuaternionMakeWithMatrix2(XVector4*result, XMatrix4 *matIn);
AGRESIVE_INLINE void  XQuaternionWithAngleAxis(XVector4*result,float radians,float x, float y, float z);
AGRESIVE_INLINE void  XQuaternionCross(XVector4*result,XVector4*left,XVector4*right);
AGRESIVE_INLINE void  XQuaternionInvert(XVector4*result,XVector4* quaternion);
AGRESIVE_INLINE void  XQuaternionMultiply(XVector4*result,XVector4* quaternionLeft, XVector4* quaternionRight);
AGRESIVE_INLINE void  XQuaternionRotateVector3(XVector3*result,XVector4* quaternion, XVector3* vector);
AGRESIVE_INLINE void  XQuaternionRotateVector3(XVector3*result,XVector4* quaternion, float x,float y,float z);

AGRESIVE_INLINE void XEulerFromQuaternion(XVector3*res,XVector4*quat);
AGRESIVE_INLINE void XQuaternionFromEuler(XVector4*result,XVector3*euler);
/*




*/


AGRESIVE_INLINE void XQuaternionFromEuler(XVector4*result,XVector3*euler){
	float c1 = cos(euler->x/2);
	float s1 = sin(euler->x/2);
	float c2 = cos(euler->y/2);
	float s2 = sin(euler->y/2);
	float c3 = cos(euler->z/2);
	float s3 = sin(euler->z/2);

	result->w = c1*c2*c3-s1*s2*s3;
	result->x = s1*c2*c3+c1*s2*s3;
	result->y = c1*s2*c3-s1*c2*s3;
	result->z = c1*c2*s3+s1*s2*c3; 
}


AGRESIVE_INLINE void XEulerFromQuaternion(XVector3*res,XVector4*quat) {
	res->x = atan2(-2*(quat->y*quat->z-quat->w*quat->x), quat->w*quat->w-quat->x*quat->x-quat->y*quat->y+quat->z*quat->z);
	res->y = asin(2*(quat->x*quat->z + quat->w*quat->y));
	res->z = atan2(-2*(quat->x*quat->y-quat->w*quat->z), quat->w*quat->w+quat->x*quat->x-quat->y*quat->y-quat->z*quat->z);
}


AGRESIVE_INLINE void  XQuaternionRotateVector3(XVector3*result,XVector4* quaternion, float x,float y,float z) {
	XVector4 rotatedQuaternion;
	XVector4Make(&rotatedQuaternion,x, y, z, 0.0f);
	
	XVector4 tmp1,inverted,tmp3;
	XQuaternionMultiply(&tmp1,quaternion, &rotatedQuaternion);
	XQuaternionInvert(&inverted,quaternion);
	XQuaternionMultiply(&tmp3,&tmp1,&inverted);

	result->x = tmp3.x;
	result->y = tmp3.y;
	result->z = tmp3.z;
}


AGRESIVE_INLINE void XQuaternionRotateVector3(XVector3*result,XVector4* quaternion, XVector3* vector) {

	XVector4 rotatedQuaternion;
	XVector4Make(&rotatedQuaternion,vector->v[0], vector->v[1], vector->v[2], 0.0f);
	
	XVector4 tmp1,inverted,tmp3;
	XQuaternionMultiply(&tmp1,quaternion, &rotatedQuaternion);
	XQuaternionInvert(&inverted,quaternion);
	XQuaternionMultiply(&tmp3,&tmp1,&inverted);

	result->x = tmp3.x;
	result->y = tmp3.y;
	result->z = tmp3.z;
}

AGRESIVE_INLINE void XQuaternionMultiply(XVector4*result,XVector4* quaternionLeft, XVector4* quaternionRight)
{
#if defined(GLK_SSE3_INTRINSICS)
	const __m128 ql = _mm_load_ps(&quaternionLeft->v[0]);
	const __m128 qr = _mm_load_ps(&quaternionRight->v[0]);

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
				   
	result = (XVector4 *)&r;
#else
	result->x =  quaternionLeft->v[3] * quaternionRight->v[0] +
                        quaternionLeft->v[0] * quaternionRight->v[3] +
                        quaternionLeft->v[1] * quaternionRight->v[2] -
                        quaternionLeft->v[2] * quaternionRight->v[1];
	result->y = quaternionLeft->v[3] * quaternionRight->v[1] +
                        quaternionLeft->v[1] * quaternionRight->v[3] +
                        quaternionLeft->v[2] * quaternionRight->v[0] -
                        quaternionLeft->v[0] * quaternionRight->v[2];
	result->z = quaternionLeft->v[3] * quaternionRight->v[2] +
                        quaternionLeft->v[2] * quaternionRight->v[3] +
                        quaternionLeft->v[0] * quaternionRight->v[1] -
                        quaternionLeft->v[1] * quaternionRight->v[0];
	result->w = quaternionLeft->v[3] * quaternionRight->v[3] -
                        quaternionLeft->v[0] * quaternionRight->v[0] -
                        quaternionLeft->v[1] * quaternionRight->v[1] -
                        quaternionLeft->v[2] * quaternionRight->v[2] ;
#endif
}

AGRESIVE_INLINE void XQuaternionInvert(XVector4*result,XVector4* quaternion)
{
#if defined(__ARM_NEON__)
    float32x4_t *q = (float32x4_t *)quaternion;
    float32x4_t v = vmulq_f32(*q, *q);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    float32_t scale = 1.0f / vget_lane_f32(v2, 0);
    v = vmulq_f32(*q, vdupq_n_f32(scale));
    
    uint32_t signBit = 0x80000000;
    uint32_t zeroBit = 0x0;
    uint32x4_t mask = vdupq_n_u32(signBit);
    mask = vsetq_lane_u32(zeroBit, mask, 3);
    v = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v), mask));
	result->x = v[0];
	result->y = v[1];
	result->z = v[2];
	result->w = v[3];
#elif defined(X_SSE3_INTRINSICS)
	const __m128 q = _mm_load_ps(&quaternion->v[0]);
    const uint32_t signBit = 0x80000000;
    const uint32_t zeroBit = 0x0;
    const uint32_t __attribute__((aligned(16))) mask[4] = {signBit, signBit, signBit, zeroBit};
    const __m128 v_mask = _mm_load_ps((float *)mask);
	const __m128 product = q * q;
	const __m128 halfsum = _mm_hadd_ps(product, product);
	const __m128 v = _mm_xor_ps(q, v_mask) / _mm_hadd_ps(halfsum, halfsum);
    result = (XVector4 *)&v;
#else
    float scale = 1.0f / (quaternion->v[0] * quaternion->v[0] + 
                          quaternion->v[1] * quaternion->v[1] +
                          quaternion->v[2] * quaternion->v[2] +
                          quaternion->v[3] * quaternion->v[3]);

	result->x = -quaternion->v[0] * scale;
	result->y = -quaternion->v[1] * scale;
	result->z = -quaternion->v[2] * scale;
	result->w =  quaternion->v[3] * scale;
#endif
}


AGRESIVE_INLINE void XQuaternionCross(XVector4*result,XVector4*q1,XVector4*q2) {
	result->x = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
	result->y = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
	result->z = q1->w * q2->y + q1->y * q2->w + q1->z * q2->x - q1->x * q2->z;
	result->w = q1->w * q2->z + q1->z * q2->w + q1->x * q2->y - q1->y * q2->x;
}

AGRESIVE_INLINE void XQuaternionWithAngleAxis(XVector4*result,float radians,float x, float y, float z) {

	float halfAngle = radians * 0.5f;
    float scale = sinf(halfAngle);

	result->x = scale * x;
	result->y = scale * y;
	result->z = scale * z;
	result->w = cosf(halfAngle);
}

AGRESIVE_INLINE void XQuaternionMakeWithMatrix(XVector4*result, XMatrix4 *mat)
{
    
    double trace = 1 + mat->m[0] + mat->m[5] + mat->m[10];
    double S = 0;
    double X = 0;
    double Y = 0;
    double Z = 0;
    double W = 0;
    
    if (trace > 0.0000001)
    {
        S = sqrt(trace) * 2;
        X = (mat->m[9] - mat->m[6]) / S;
        Y = (mat->m[2] - mat->m[8]) / S;
        Z = (mat->m[4] - mat->m[1]) / S;
        W = 0.25 * S;
    }
    else
    {
        if (mat->m[0] > mat->m[5] && mat->m[0] > mat->m[10])
        {
            // Column 0:
            S = sqrt(1.0 + mat->m[0] - mat->m[5] - mat->m[10]) * 2;
            X = 0.25 * S;
            Y = (mat->m[4] + mat->m[1]) / S;
            Z = (mat->m[2] + mat->m[8]) / S;
            W = (mat->m[9] - mat->m[6]) / S;
        }
        else if (mat->m[5] > mat->m[10])
        {
            // Column 1:
            S = sqrt(1.0 + mat->m[5] - mat->m[0] - mat->m[10]) * 2;
            X = (mat->m[4] + mat->m[1]) / S;
            Y = 0.25 * S;
            Z = (mat->m[9] + mat->m[6]) / S;
            W = (mat->m[2] - mat->m[8]) / S;
        }
        else
        {
            // Column 2:
            S = sqrt(1.0 + mat->m[10] - mat->m[0] - mat->m[5]) * 2;
            X = (mat->m[2] + mat->m[8]) / S;
            Y = (mat->m[9] + mat->m[6]) / S;
            Z = 0.25 * S;
            W = (mat->m[4] - mat->m[1]) / S;
        }
    }
    
    
    result->x = (float)-X;
    result->y = (float)-Y;
    result->z = (float)-Z;
    result->w = (float)W;
    
    
}



AGRESIVE_INLINE void XQuaternionMakeWithMatrix2(XVector4*result, XMatrix4 *matIn)
{
    
    double trace = 1 + matIn->m[0] + matIn->m[5] + matIn->m[10];
    double S = 0;
    double X = 0;
    double Y = 0;
    double Z = 0;
    double W = 0;
    
    if (trace > 0.001)
    {
        S = sqrt(trace) * 2;
        X = (matIn->m[6] - matIn->m[9]) / S;
        Y = (matIn->m[8] - matIn->m[2]) / S;
        Z = (matIn->m[1] - matIn->m[4]) / S;
        W = 0.25 * S;
    }
    else
    {
        if (matIn->m[0] > matIn->m[5] && matIn->m[0] > matIn->m[10])
        {
            // Column 0:
            S = sqrt(1.0 + matIn->m[0] - matIn->m[5] - matIn->m[10]) * 2;
            X = 0.25 * S;
            Y = (matIn->m[1] + matIn->m[4]) / S;
            Z = (matIn->m[8] + matIn->m[2]) / S;
            W = (matIn->m[6] - matIn->m[9]) / S;
        }
        else if (matIn->m[5] > matIn->m[10])
        {
            // Column 1:
            S = sqrt(1.0 + matIn->m[5] - matIn->m[0] - matIn->m[10]) * 2;
            X = (matIn->m[1] + matIn->m[4]) / S;
            Y = 0.25 * S;
            Z = (matIn->m[6] + matIn->m[9]) / S;
            W = (matIn->m[8] - matIn->m[2]) / S;
        }
        else
        {
            // Column 2:
            S = sqrt(1.0 + matIn->m[10] - matIn->m[0] - matIn->m[5]) * 2;
            X = (matIn->m[8] + matIn->m[2]) / S;
            Y = (matIn->m[6] + matIn->m[9]) / S;
            Z = 0.25 * S;
            W = (matIn->m[1] - matIn->m[4]) / S;
        }
    }
    
    
    result->x = (float)-X;
    result->y = (float)-Y;
    result->z = (float)-Z;
    result->w = (float)W;
    
    
}








AGRESIVE_INLINE float XQuaternionLength(XVector4 *quaternion)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vmulq_f32(*(float32x4_t *)quaternion,
                              *(float32x4_t *)quaternion);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    return sqrt(vget_lane_f32(v2, 0));
#elif defined(X_SSE3_INTRINSICS)
	const __m128 q = _mm_load_ps(quaternion->q[0]);
	const __m128 product = q * q;
	const __m128 halfsum = _mm_hadd_ps(product, product);
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(halfsum, halfsum)));
#else
    return sqrt(quaternion->v[0] * quaternion->v[0] +
                quaternion->v[1] * quaternion->v[1] +
                quaternion->v[2] * quaternion->v[2] +
                quaternion->v[3] * quaternion->v[3]);
#endif
}

AGRESIVE_INLINE void XQuaternionNormalize(XVector4*res, XVector4* quaternion)
{
    float scale = 1.0f / XQuaternionLength(quaternion);
#if defined(__ARM_NEON__)
    float32x4_t v = vmulq_f32(*(float32x4_t *)quaternion,
                              vdupq_n_f32((float32_t)scale));
    res = (XVector4 *)&v;
#elif defined(X_SSE3_INTRINSICS)
	const __m128 q = _mm_load_ps(quaternion->q[0]);
    __m128 v = q * _mm_set1_ps(scale);
    res = (XVector4 *)&v;
#else
    res->x = quaternion->v[0] * scale;
    res->y = quaternion->v[1] * scale;
    res->z = quaternion->v[2] * scale;
    res->w = quaternion->v[3] * scale;
#endif
}

#define DELTA 0.0005f
AGRESIVE_INLINE void XQuaternion4Slerp(XVector4 * res, XVector4 * q, XVector4 * p, float t)
{

	if (t >= 1.0) return;

	float p1[4];
	double omega, cosom, sinom, scale0, scale1;
	
	// косинус угла
	cosom = q->x*p->x + q->y*p->y + q->z*p->z + q->w*p->w;
	
	if ( cosom <0.0 )
	{
		cosom = -cosom;
		p1[0] = - p->x;  p1[1] = - p->y;
		p1[2] = - p->z;  p1[3] = - p->w;
	}
	else
	{
		p1[0] = p->x;    p1[1] = p->y;
		p1[2] = p->z;    p1[3] = p->w;
	}
	
	if ( (1.0 - cosom) > DELTA )
	{
		// стандартный случай (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	}
	else
	{
		// если маленький угол - линейная интерполяция
		scale0 = 1.0 - t;
		scale1 = t;
	}
	
	res->x =(float) (scale0 * q->x + scale1 * p1[0]);
	res->y =(float) (scale0 * q->y + scale1 * p1[1]);
	res->z =(float) (scale0 * q->z + scale1 * p1[2]);
	res->w =(float) (scale0 * q->w + scale1 * p1[3]);
}



AGRESIVE_INLINE void XVector4Make(XVector4 *in,float x,float y,float z,float w) {
	in->x = x;
	in->y = y;
	in->z = z;
	in->w = w;
}




AGRESIVE_INLINE float XVector4Dot(const XVector4 *vectorLeft,const XVector4 *vectorRight)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vmulq_f32(*(float32x4_t *)vectorLeft,
                              *(float32x4_t *)vectorRight);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    return vget_lane_f32(v2, 0);
#else
    return vectorLeft->v[0] * vectorRight->v[0] +
           vectorLeft->v[1] * vectorRight->v[1] +
           vectorLeft->v[2] * vectorRight->v[2] +
           vectorLeft->v[3] * vectorRight->v[3];
#endif
}


AGRESIVE_INLINE float XVector4Dot(const XVector4 *vectorLeft,const XVector3 *vectorRight,float wRight)
{
    
#if defined(__ARM_NEON__)
    XVector4 vectorRight4 = {vectorRight->x,vectorRight->y,vectorRight->z,wRight};
    float32x4_t v = vmulq_f32(*(float32x4_t *)vectorLeft,
                              *(float32x4_t *)&vectorRight4);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    return vget_lane_f32(v2, 0);
#else
    return vectorLeft->v[0] * vectorRight->v[0] +
           vectorLeft->v[1] * vectorRight->v[1] +
           vectorLeft->v[2] * vectorRight->v[2] +
           vectorLeft->v[3] * wRight;
#endif
}



#endif
