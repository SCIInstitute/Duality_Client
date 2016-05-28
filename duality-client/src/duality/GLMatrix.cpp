#include "duality/GLMatrix.h"

#include <cmath>
#include <cstring>

GLMatrix::GLMatrix() {
    loadIdentity();
}

void GLMatrix::scale(float sx, float sy, float sz) {
    GLMatrix scaleMat;
    scaleMat.m[0][0] = sx;
    scaleMat.m[1][1] = sy;
    scaleMat.m[2][2] = sz;
    multiply(scaleMat);
}

void GLMatrix::translate(float tx, float ty, float tz) {
    GLMatrix transMat;
    transMat.m[3][0] = tx;
    transMat.m[3][1] = ty;
    transMat.m[3][2] = tz;
    multiply(transMat);
}

void GLMatrix::rotate(float angle, float x, float y, float z) {
    float sinAngle, cosAngle;
    float mag = sqrtf(x * x + y * y + z * z);

    if (mag > 0.0f) {
        sinAngle = sinf(angle * M_PI / 180.0f);
        cosAngle = cosf(angle * M_PI / 180.0f);

        float xx, yy, zz, xy, yz, zx, xs, ys, zs;
        float oneMinusCos;
        GLMatrix rotMat;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
        rotMat.m[0][1] = (oneMinusCos * xy) - zs;
        rotMat.m[0][2] = (oneMinusCos * zx) + ys;
        rotMat.m[0][3] = 0.0f;

        rotMat.m[1][0] = (oneMinusCos * xy) + zs;
        rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
        rotMat.m[1][2] = (oneMinusCos * yz) - xs;
        rotMat.m[1][3] = 0.0f;

        rotMat.m[2][0] = (oneMinusCos * zx) - ys;
        rotMat.m[2][1] = (oneMinusCos * yz) + xs;
        rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
        rotMat.m[2][3] = 0.0f;

        rotMat.m[3][0] = 0.0f;
        rotMat.m[3][1] = 0.0f;
        rotMat.m[3][2] = 0.0f;
        rotMat.m[3][3] = 1.0f;

        multiply(rotMat);
    }
}

void GLMatrix::frustum(float left, float right, float bottom, float top, float nearZ, float farZ) {
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;

    if ((nearZ <= 0.0f) || (farZ <= 0.0f) || (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f))
        return;

    GLMatrix frust;
    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    multiply(frust);
}

void GLMatrix::perspective(float fovy, float aspect, float nearZ, float farZ) {
    float frustumW, frustumH;
    frustumH = tanf(fovy / 360.0f * M_PI) * nearZ;
    frustumW = frustumH * aspect;
    frustum(-frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

void GLMatrix::ortho(float left, float right, float bottom, float top, float nearZ, float farZ) {
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;

    if ((deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f))
        return;

    GLMatrix ortho;
    ortho.m[0][0] = 2.0f / deltaX;
    ortho.m[3][0] = -(right + left) / deltaX;
    ortho.m[1][1] = 2.0f / deltaY;
    ortho.m[3][1] = -(top + bottom) / deltaY;
    ortho.m[2][2] = -2.0f / deltaZ;
    ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

    multiply(ortho);
}

GLMatrix::GLMatrix(const float* rows) {
    memcpy(m, rows, sizeof(float) * 16);
}

GLMatrix::GLMatrix(const GLMatrix& matrix) {
    loadMatrix(matrix);
}

void GLMatrix::loadMatrix(const GLMatrix& matrix) {
    memcpy(m, matrix, sizeof(float) * 16);
}

void GLMatrix::multiply(const GLMatrix& matrix) {
    float tmp[4][4];
    for (int i = 0; i < 4; ++i) {
        tmp[i][0] = (m[i][0] * matrix.m[0][0]) + (m[i][1] * matrix.m[1][0]) + (m[i][2] * matrix.m[2][0]) + (m[i][3] * matrix.m[3][0]);

        tmp[i][1] = (m[i][0] * matrix.m[0][1]) + (m[i][1] * matrix.m[1][1]) + (m[i][2] * matrix.m[2][1]) + (m[i][3] * matrix.m[3][1]);

        tmp[i][2] = (m[i][0] * matrix.m[0][2]) + (m[i][1] * matrix.m[1][2]) + (m[i][2] * matrix.m[2][2]) + (m[i][3] * matrix.m[3][2]);

        tmp[i][3] = (m[i][0] * matrix.m[0][3]) + (m[i][1] * matrix.m[1][3]) + (m[i][2] * matrix.m[2][3]) + (m[i][3] * matrix.m[3][3]);
    }
    memcpy(m, tmp, sizeof(float) * 16);
}

void GLMatrix::multiplyLeft(const GLMatrix& matrix) {
    float tmp[4][4];
    for (int i = 0; i < 4; ++i) {
        tmp[i][0] = (matrix.m[i][0] * m[0][0]) + (matrix.m[i][1] * m[1][0]) + (matrix.m[i][2] * m[2][0]) + (matrix.m[i][3] * m[3][0]);

        tmp[i][1] = (matrix.m[i][0] * m[0][1]) + (matrix.m[i][1] * m[1][1]) + (matrix.m[i][2] * m[2][1]) + (matrix.m[i][3] * m[3][1]);

        tmp[i][2] = (matrix.m[i][0] * m[0][2]) + (matrix.m[i][1] * m[1][2]) + (matrix.m[i][2] * m[2][2]) + (matrix.m[i][3] * m[3][2]);

        tmp[i][3] = (matrix.m[i][0] * m[0][3]) + (matrix.m[i][1] * m[1][3]) + (matrix.m[i][2] * m[2][3]) + (matrix.m[i][3] * m[3][3]);
    }
    memcpy(m, tmp, sizeof(float) * 16);
}

void GLMatrix::loadIdentity() {
    memset(m, 0, sizeof(float) * 16);
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
}

GLMatrix GLMatrix::inverse() const {

    GLMatrix result;

    float Q = m[1][0] * (m[2][3] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) + m[0][3] * (-m[2][1] * m[3][2] + m[3][1] * m[2][2]) +
                         m[3][3] * (m[0][2] * m[2][1] - m[0][1] * m[2][2])) +
              m[1][3] * (m[0][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) + m[0][2] * (-m[3][0] * m[2][1] + m[2][0] * m[3][1]) +
                         m[0][1] * (-m[2][0] * m[3][2] + m[3][0] * m[2][2])) +
              m[3][3] * (m[1][1] * (-m[2][0] * m[0][2] + m[0][0] * m[2][2]) + m[1][2] * (-m[0][0] * m[2][1] + m[0][1] * m[2][0])) +
              m[2][3] * (m[0][0] * (-m[1][1] * m[3][2] + m[1][2] * m[3][1]) + m[3][0] * (m[0][2] * m[1][1] - m[1][2] * m[0][1])) +
              m[0][3] * (m[1][2] * (m[2][1] * m[3][0] - m[3][1] * m[2][0]) + m[1][1] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]));

    result.m[0][0] = (m[1][3] * m[2][1] * m[3][2] + m[3][3] * m[1][1] * m[2][2] - m[3][3] * m[1][2] * m[2][1] -
                      m[2][3] * m[1][1] * m[3][2] - m[1][3] * m[3][1] * m[2][2] + m[2][3] * m[1][2] * m[3][1]) /
                     Q;
    result.m[1][0] = -(m[1][0] * m[3][3] * m[2][2] - m[1][0] * m[2][3] * m[3][2] - m[3][3] * m[1][2] * m[2][0] +
                       m[2][3] * m[1][2] * m[3][0] + m[1][3] * m[2][0] * m[3][2] - m[1][3] * m[3][0] * m[2][2]) /
                     Q;
    result.m[2][0] = (-m[1][0] * m[2][3] * m[3][1] + m[1][0] * m[3][3] * m[2][1] - m[3][3] * m[2][0] * m[1][1] -
                      m[1][3] * m[3][0] * m[2][1] + m[2][3] * m[3][0] * m[1][1] + m[1][3] * m[2][0] * m[3][1]) /
                     Q;
    result.m[3][0] = -(m[1][0] * m[2][1] * m[3][2] - m[1][0] * m[3][1] * m[2][2] + m[3][0] * m[1][1] * m[2][2] -
                       m[2][1] * m[1][2] * m[3][0] - m[2][0] * m[1][1] * m[3][2] + m[3][1] * m[1][2] * m[2][0]) /
                     Q;
    /// 2
    result.m[0][1] = (-m[0][1] * m[3][3] * m[2][2] + m[0][1] * m[2][3] * m[3][2] - m[2][3] * m[0][2] * m[3][1] -
                      m[0][3] * m[2][1] * m[3][2] + m[3][3] * m[0][2] * m[2][1] + m[0][3] * m[3][1] * m[2][2]) /
                     Q;

    result.m[1][1] = (-m[3][3] * m[0][2] * m[2][0] + m[3][3] * m[0][0] * m[2][2] - m[2][3] * m[0][0] * m[3][2] -
                      m[0][3] * m[3][0] * m[2][2] + m[2][3] * m[0][2] * m[3][0] + m[0][3] * m[2][0] * m[3][2]) /
                     Q;

    result.m[2][1] = -(-m[0][1] * m[3][3] * m[2][0] + m[0][1] * m[2][3] * m[3][0] + m[3][3] * m[0][0] * m[2][1] -
                       m[0][3] * m[2][1] * m[3][0] + m[0][3] * m[3][1] * m[2][0] - m[2][3] * m[0][0] * m[3][1]) /
                     Q;

    result.m[3][1] = (-m[0][1] * m[2][0] * m[3][2] + m[0][1] * m[3][0] * m[2][2] + m[0][0] * m[2][1] * m[3][2] -
                      m[0][0] * m[3][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1] + m[2][0] * m[0][2] * m[3][1]) /
                     Q;
    /// 3
    result.m[0][2] = -(m[3][3] * m[0][2] * m[1][1] - m[1][3] * m[0][2] * m[3][1] - m[0][3] * m[1][1] * m[3][2] +
                       m[0][1] * m[1][3] * m[3][2] - m[0][1] * m[3][3] * m[1][2] + m[0][3] * m[3][1] * m[1][2]) /
                     Q;

    result.m[1][2] = (-m[1][0] * m[0][3] * m[3][2] + m[1][0] * m[3][3] * m[0][2] + m[1][3] * m[0][0] * m[3][2] -
                      m[3][3] * m[1][2] * m[0][0] - m[1][3] * m[3][0] * m[0][2] + m[0][3] * m[1][2] * m[3][0]) /
                     Q;

    result.m[2][2] = -(-m[3][3] * m[0][0] * m[1][1] + m[3][3] * m[0][1] * m[1][0] + m[0][3] * m[3][0] * m[1][1] +
                       m[1][3] * m[0][0] * m[3][1] - m[1][3] * m[0][1] * m[3][0] - m[0][3] * m[1][0] * m[3][1]) /
                     Q;

    result.m[3][2] = -(m[3][2] * m[0][0] * m[1][1] - m[3][2] * m[0][1] * m[1][0] - m[0][2] * m[3][0] * m[1][1] -
                       m[1][2] * m[0][0] * m[3][1] + m[1][2] * m[0][1] * m[3][0] + m[0][2] * m[1][0] * m[3][1]) /
                     Q;
    /// 4
    result.m[0][3] = (-m[0][1] * m[2][3] * m[1][2] + m[0][1] * m[1][3] * m[2][2] - m[1][3] * m[0][2] * m[2][1] -
                      m[0][3] * m[1][1] * m[2][2] + m[2][3] * m[0][2] * m[1][1] + m[0][3] * m[2][1] * m[1][2]) /
                     Q;

    result.m[1][3] = -(-m[1][0] * m[0][3] * m[2][2] + m[1][0] * m[2][3] * m[0][2] + m[1][3] * m[0][0] * m[2][2] -
                       m[2][3] * m[1][2] * m[0][0] + m[0][3] * m[1][2] * m[2][0] - m[1][3] * m[2][0] * m[0][2]) /
                     Q;

    result.m[2][3] = (-m[2][3] * m[0][0] * m[1][1] + m[2][3] * m[0][1] * m[1][0] + m[0][3] * m[2][0] * m[1][1] +
                      m[1][3] * m[0][0] * m[2][1] - m[1][3] * m[0][1] * m[2][0] - m[0][3] * m[1][0] * m[2][1]) /
                     Q;

    result.m[3][3] = (m[2][2] * m[0][0] * m[1][1] - m[2][2] * m[0][1] * m[1][0] - m[0][2] * m[2][0] * m[1][1] -
                      m[1][2] * m[0][0] * m[2][1] + m[1][2] * m[0][1] * m[2][0] + m[0][2] * m[1][0] * m[2][1]) /
                     Q;
    return result;
}

/*
 The MIT License

 Copyright (c) 2012 Interactive Visualization and Data Analysis Group
 Saarland University, DFKI, Intel VCI, MMCI

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
