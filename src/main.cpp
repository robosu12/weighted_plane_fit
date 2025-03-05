#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

using namespace Eigen;

// 生成带噪声的平面数据
MatrixXd generate_points(int num_points)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-3.0, 3.0);
    std::normal_distribution<> gauss(4.0, 0.1);

    MatrixXd points(num_points, 3);

    // 生成平面点 z = 2.0*x + 3.0*y + 噪声
    for (int i = 0; i < num_points - 1; ++i)
    {
        double x = dis(gen);
        double y = dis(gen);
        points(i, 0) = x;
        points(i, 1) = y;
        points(i, 2) = 2.0 * x + 4.0 * y + gauss(gen);
    }

    // 添加噪声点 (0,0,10)
    points(num_points - 1, 0) = 0;
    points(num_points - 1, 1) = 0;
    points(num_points - 1, 2) = 10;

    return points;
}

// 加权平面拟合
Vector4d weighted_plane_fit(const MatrixXd &points, double noise_weight)
{
    int N = points.rows();
    VectorXd weights = VectorXd::Ones(N);
    weights(N - 1) = noise_weight; // 最后一个点是噪声点

    // 构造加权矩阵
    MatrixXd M = points.array().colwise() * weights.array();
    VectorXd b = -weights;

    // QR分解求解
    ColPivHouseholderQR<MatrixXd> qr(M);
    Vector3d ABC = qr.solve(b);

    double norm = ABC.norm();

    Vector4d pabcd;
    pabcd(0) = ABC(0) / norm;
    pabcd(1) = ABC(1) / norm;
    pabcd(2) = ABC(2) / norm;
    pabcd(3) = 1.0 / norm;

    // 归一化法向量
    return pabcd;
}

int main()
{
    const int num_points = 10;
    MatrixXd points = generate_points(num_points);

    // 保存数据点
    std::ofstream data_file("../data/points.csv");
    const IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n"); // 新增格式定义
    data_file << points.format(CSVFormat);                                // 使用自定义格式
    data_file.close();

    // 三种权重情况
    const std::vector<double> weights = {0.5, 1.0, 2.0};

    int i = 0;
    for (double w : weights)
    {
        Vector4d pabcd = weighted_plane_fit(points, w);

        // 保存平面参数
        i++;
        std::string filename = "../data/plane_" + std::to_string(i) + ".csv";
        std::ofstream plane_file(filename);
        plane_file << pabcd[0] << "," << pabcd[1] << "," << pabcd[2] << "," << pabcd[3] << std::endl;
        plane_file.close();

        std::cout << pabcd[0] << "," << pabcd[1] << "," << pabcd[2] << "," << pabcd[3] << std::endl;
    }

    std::cout << "数据已保存至 data/ 目录" << std::endl;
    return 0;
}