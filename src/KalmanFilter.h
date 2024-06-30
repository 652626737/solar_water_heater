#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter
{
public:
    KalmanFilter(float processNoise, float measurementNoise, float estimatedError, float initialValue);
    float update(float measurement);

private:
    float Q; // 过程噪声协方差
    float R; // 测量噪声协方差
    float P; // 估计误差协方差
    float K; // 卡尔曼增益
    float X; // 滤波后的值
};

#endif // KALMANFILTER_H
