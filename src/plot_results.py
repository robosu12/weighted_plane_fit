import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 读取数据
points = np.loadtxt('../data/points.csv', delimiter=',')
weights = [0.5, 1.0, 2.0]

fig = plt.figure(figsize=(15,5))
for i, w in enumerate(weights):
    ax = fig.add_subplot(1, 3, i+1, projection='3d')
    
    # 绘制点
    ax.scatter(points[:-1,0], points[:-1,1], points[:-1,2], c='b', label='inlier')
    ax.scatter(points[-1,0], points[-1,1], points[-1,2], c='r', s=100, label='outlier')
    
    # 读取平面参数
    A, B, C, D = np.loadtxt(f'../data/plane_{i+1}.csv', delimiter=',')
    xx, yy = np.meshgrid(np.linspace(-6,6,10), np.linspace(-6,6,10))
    zz = (-A*xx - B*yy - D)/C  # 平面方程: Ax + By + Cz + 1 = 0
    
    ax.plot_surface(xx, yy, zz, alpha=0.5, color='orange')
    ax.set_title(f'weight = {w}')
    ax.set_zlim(-5, 15)
    ax.legend()

plt.tight_layout()
plt.savefig('result.png')
plt.show()