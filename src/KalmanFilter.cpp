#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float processNoise, float measurementNoise, float estimatedError, float initialValue)
{
    Q = processNoise;
    R = measurementNoise;
    P = estimatedError;
    X = initialValue;
}

float KalmanFilter::update(float measurement)
{
    // 预测阶段
    P = P + Q;

    // 更新阶段
    K = P / (P + R);
    X = X + K * (measurement - X);
    P = (1 - K) * P;

    return X;
}
