cmake_minimum_required(VERSION 3.10)
project(WeightedPlaneFit)

set(CMAKE_CXX_STANDARD 17)

# Eigen3 路径配置（根据实际安装路径修改）
include_directories(/usr/include/eigen3)

add_executable(plane_fit src/main.cpp)