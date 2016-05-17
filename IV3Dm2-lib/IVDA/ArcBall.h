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

#pragma once

#ifndef IVDA_ARCBALL
#define IVDA_ARCBALL

#include "StdDefines.h"
#include "Vectors.h"

namespace IVDA
{
	/** \class ArcBall */
	class ArcBall
	{
	public:
	  ArcBall(uint32_t iWinWidth=0, uint32_t iWinHeight=0, int iWinOffsetX=0, int iWinOffsetY=0, bool bUseTranslation = false);

	  void SetRadius(float fRadius) {m_fRadius = fRadius;}
	  void SetTranslation(const Mat4f& mTranslation) {m_mTranslation = mTranslation;}
	  const Mat4f& GetTranslation() const {return m_mTranslation;}
	  void SetUseTranslation(bool bUseTranslation) {m_bUseTranslation = bUseTranslation;}
	  bool GetUseTranslation() {return m_bUseTranslation;}
	  void SetWindowSize(uint32_t iWinWidth, uint32_t iWinHeight);
	  void SetWindowOffset(int iWinOffsetX, int iWinOffsetY);
	  void Click(Vec2ui vPosition);
	  FLOATQUATERNION4 Drag(Vec2ui vPosition);

	protected:
	  static float ms_fEpsilon;

	  Vec3f m_vStartDrag;   ///< Saved click vector
	  Vec2ui  m_iWinDim;      ///< window dimensions
	  Vec2i   m_iWinOffsets;  ///< Horizontal/Vertical window offset
	  float        m_fRadius;      ///< radius of the ball
	  Mat4f m_mTranslation; ///< translation of the ball
	  bool         m_bUseTranslation; ///< whether to the translation of the ball


	  Vec3f MapToSphere(Vec2ui vPosition) const;
	};
}

#endif // ARCBALL_H
