/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

/**
  \file    ArcBall.cpp
  \author  Jens Krueger
           SCI Institute
           University of Utah
           Based on the NeHe Tutorial 48
  \date    October 2008
*/

#include "ArcBall.h"

namespace IVDA
{
	float ArcBall::ms_fEpsilon = 1.0e-5f;

	ArcBall::ArcBall(uint32_t iWinWidth, uint32_t iWinHeight, int iWinOffsetX, int iWinOffsetY, bool bUseTranslation) :
		m_vStartDrag(),
		m_iWinDim(iWinWidth, iWinHeight),
		m_iWinOffsets(iWinOffsetX, iWinOffsetY),
		m_fRadius(1.0f),
		m_bUseTranslation(bUseTranslation)
	{
	}

	void ArcBall::SetWindowSize(uint32_t iWinWidth, uint32_t iWinHeight) {
		m_iWinDim = Vec2ui(iWinWidth, iWinHeight);
	}

	void ArcBall::SetWindowOffset(int iWinOffsetX, int iWinOffsetY) {
	  m_iWinOffsets = Vec2i(iWinOffsetX, iWinOffsetY);
	}

	void ArcBall::Click(Vec2ui vPosition) {
	  m_vStartDrag = MapToSphere(vPosition);
	}

	FLOATQUATERNION4 ArcBall::Drag(Vec2ui vPosition) {
	  FLOATQUATERNION4 qRotation;

	  // Map the point to the sphere
	  Vec3f vCurrent = MapToSphere(vPosition);

	  // Compute the vector perpendicular to the begin and end vectors
	  Vec3f vCross(vCurrent % m_vStartDrag);
	  float        fDot(vCurrent ^ m_vStartDrag);

	  if (vCross.length() > ms_fEpsilon)    //if its non-zero
		  return FLOATQUATERNION4(vCross.x, vCross.y, vCross.z, fDot);
	  else
		  return FLOATQUATERNION4(0,0,0,0);
	}

	Vec3f ArcBall::MapToSphere(Vec2ui vPosition) const {
	  Vec3f vResult;

	  // normalize position to [-1 ... 1]
	  Vec2f vNormPosition;
	  vNormPosition.x =  -(((vPosition.x-m_iWinOffsets.x) / (float(m_iWinDim.x - 1) / 2.0f)) - 1.0f);
	  vNormPosition.y =  ((vPosition.y-m_iWinOffsets.y) / (float(m_iWinDim.y - 1) / 2.0f)) - 1.0f;

	  if (m_bUseTranslation) {
		Mat4f mTranslation(m_mTranslation);
		mTranslation.m43 = 0;
		vNormPosition = (Vec4f(vNormPosition,0.0f,1.0f) * mTranslation).xy();
	  }

	  // Compute the length of the vector to the point from the center
	  float length = vNormPosition.length();

	  // If the point is mapped outside of the sphere... (length > radius)
	  if (length > m_fRadius) {
		  // Compute a normalizing factor (radius / length)
		  float norm = float(m_fRadius / length);

		  // Return the "normalized" vector, a point on the sphere
		  vResult.x = vNormPosition.x * norm;
		  vResult.y = vNormPosition.y * norm;
		  vResult.z = 0.0f;
	  } else    // Else it's on the inside
	  {
		  // Return a vector to a point mapped inside the sphere
		  vResult.x = vNormPosition.x;
		  vResult.y = vNormPosition.y;
		  vResult.z = length-m_fRadius;
	  }

	  return vResult;
	}
}
