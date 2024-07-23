// Creator: Davy (Dawei) Chen
// Created time: July 23rd, 2024

#ifndef RENDER_MODEL
#define RENDER_MODEL

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

#define FLOAT_ZERO 0.000001f
#define SWT_PI 3.14159265358979323846

class RenderModel: public QOpenGLExtraFunctions
{
public:
    RenderModel();
    virtual ~RenderModel();

public:
    QMatrix4x4 setEulerToMatrix(float yaw, float pitch, float roll);
    QVector3D getEulerFromMatrix(QMatrix4x4 mat);
    int judgePointOnWhichSideOfLine(QVector2D lineA, QVector2D lineB, QVector2D pointC);

private:
    QVector3D position;
    float m_pitch = 0.0f;
    float m_yaw = 0.0f;
    float m_roll = 0.0f;

};

#endif
