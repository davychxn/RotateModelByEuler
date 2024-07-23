#include "./RenderModel.h"

// Set Eulers into a Matrix
// When rotate a model, rotation order of the 3 axes matters. Use this to avoid the bothering
QMatrix4x4 RenderModel::setEulerToMatrix(float yaw, float pitch, float roll)
{
    QMatrix4x4 m1;
    m1.setToIdentity();

    float pitch2 = -pitch * 2.0f * SWT_PI / 360.0f;
    float yaw2 = -yaw * 2.0f * SWT_PI / 360.0f;
    float roll2 = -roll * 2.0f * SWT_PI / 360.0f;

    float cosY = cosf(yaw2);       // Yaw
    float sinY = sinf(yaw2);

    float cosP = cosf(pitch2);     // Pitch
    float sinP = sinf(pitch2);

    float cosR = cosf(roll2);      // Roll
    float sinR = sinf(roll2);

    float e11 = cosY * cosR + sinY * sinP * sinR;
    float e21 = cosR * sinY * sinP - sinR * cosY;
    float e31 = cosP * sinY;

    float e12 = cosP * sinR;
    float e22 = cosR * cosP;
    float e32 = -sinP;

    float e13 = sinR * cosY * sinP - sinY * cosR;
    float e23 = sinY * sinR + cosR * cosY * sinP;
    float e33 = cosP * cosY;

    QVector4D row1 = m1.row(0);
    QVector4D row2 = m1.row(1);
    QVector4D row3 = m1.row(2);
    row1.setX(e11);
    row1.setY(e12);
    row1.setZ(e13);
    row2.setX(e21);
    row2.setY(e22);
    row2.setZ(e23);
    row3.setX(e31);
    row3.setY(e32);
    row3.setZ(e33);
    m1.setRow(0, row1);
    m1.setRow(1, row2);
    m1.setRow(2, row3);

    return m1;
}

// Extract Eulers from a Matrix
QVector3D RenderModel::getEulerFromMatrix(QMatrix4x4 mat)
{
    float e32 = mat.row(2).y();
    float e31 = mat.row(2).x();
    float e33 = mat.row(2).z();
    float e12 = mat.row(0).y();
    float e22 = mat.row(1).y();
    float e21 = mat.row(1).x();
    float e11 = mat.row(0).x();

    float pitch = -asinf(-e32);        // Pitch
    float yaw = 0.0f;
    float roll = 0.0f;
    if (cosf(pitch) > 0.0001)          // Not at poles
    {
        yaw = -atan2f(e31, e33);       // Yaw
        roll = -atan2f(e12, e22);      // Roll
    }
    else
    {
        yaw = 0.0f;                    // Yaw
        roll = -atan2f(-e21, e11);     // Roll
    }

    // From Radians to Angles
    yaw = yaw * 360.0f * 0.5f / SWT_PI;
    pitch = pitch * 360.0f * 0.5f / SWT_PI;
    roll = roll * 360.0f * 0.5f / SWT_PI;

    return QVector3D(yaw, pitch, roll);
}

// Tell a point lies on which side of a line
// Point A and B make a line. C is the point to be judged
int RenderModel::judgePointOnWhichSideOfLine(QVector2D lineA, QVector2D lineB, QVector2D pointC)
{
    QVector2D va = lineA;
    QVector2D vb = lineB;
    QVector2D vc = pointC;
    float sign = (vb.x() - va.x()) * (vc.y() - va.y()) - (vb.y() - va.y()) * (vc.x() - va.x());
    return sign < 0.0f ? -1.0f : 1.0f;
}

// Called by rendering funtions to give Model Matrix
QMatrix4x4 RenderModel::calcModelMatrix()
{
    // Get scene stage offset in the world
    QMatrix4x4 m1 = RDR->getAccessories()->getModelMatrix();
    QVector3D eye = RDR->getAccessories()->getEye();
    // Model bottom center offset
    QVector3D v2 = QVector3D(m_center.x(), m_lower.y(), m_center.z());
    // Model center offset
    QVector3D v3 = QVector3D(m_center.x(), m_center.y(), m_center.z());
    QVector3D v4 = QVector3D(0.0f, -m_center.y() + m_lower.y(), 0.0f);

    m1.translate(-v4 + position - eye);
    // Set Eulers to Model Matrix
    QMatrix4x4 m2 = setEulerToMatrix(m_yaw, m_pitch, m_roll);
    // Combine rotation and translation
    m1 = m1 * m2;

    m1.translate(-v3 + eye);

    return m1;
}

// Called by mouse event listener to apply new rotations to model on stage
void RenderModel::setRotate(int axis, float delta)
{
    if (axis < 0 || axis >= 3)
    {
        return;
    }

    QMatrix4x4 m3;
    m3.setToIdentity();
    // Current rotation
    switch (axis)
    {
    case 0:
    {
        m3.rotate(delta, QVector3D(1.0f, 0.0f, 0.0f));
        break;
    }
    case 1:
    {
        m3.rotate(delta,  QVector3D(0.0f, 1.0f, 0.0f));
        break;
    }
    case 2:
    {
        m3.rotate(delta,  QVector3D(0.0f, 0.0f, 1.0f));
        break;
    }
    }

    // Get Rotation Matrix from former Eulers
    QMatrix4x4 m2 = setEulerToMatrix(m_yaw, m_pitch, m_roll);
    // Combine current and former rotations
    m2 = m3 * m2;
    // Keep rotations by Eulers for rendering later
    QVector3D v1 = getEulerFromMatrix(m2);
    m_yaw = v1.x();
    m_pitch = v1.y();
    m_roll = v1.z();

}
