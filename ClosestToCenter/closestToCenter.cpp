#include <Eigen/LU>
#include <iostream>

int main(int argc, char *argv[]) {

  Eigen::MatrixXd map, obstacles;
  map.resize(11, 11);
  map << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
      1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0,
      0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
  obstacles = map;
  std::cout << "Map:\n" << map << "\n";

  int MAX_NON_OBSTACLE = 1;

  unsigned int x = map.cols(), y = map.rows();

  for (unsigned int xi = 0; xi < x; xi++) {
    for (unsigned int yi = 0; yi < y; yi++) {
      while (yi < y && map(yi, xi) >= MAX_NON_OBSTACLE) {
        int directionX = xi <= x / 2 ? 1 : -1,
            directionY = yi <= y / 2 ? 1 : -1;

        if (map(yi, xi + directionX) < MAX_NON_OBSTACLE) {
          std::cout << "Adicionando (xi, yi) = (" << xi << ", " << yi << ")\n";
          obstacles(yi, xi) = 2;
        } else if (yi + directionY >= 0 && yi + directionY < y &&
                   map(yi + directionY, xi) < MAX_NON_OBSTACLE) {
          std::cout << "Adicionando (xi, yi) = (" << xi << ", " << yi << ")\n";
          obstacles(yi, xi) = 2;
        }
        yi++;
      }
    }
  }
  std::cout << "Obstacles:\n" << obstacles << "\n";

  Eigen::MatrixXd data;
  Eigen::Vector2d xy;
  // xy.resize(1, 2);
  xy << 0, 0;
  data.resize(18, 2);
  data << 0, 5, 1, 4, 2, 4, 2, 7, 3, 1, 3, 2, 3, 3, 3, 8, 4, 9, 5, 9, 6, 8, 7,
      8, 8, 3, 8, 4, 8, 5, 8, 8, 9, 6, 10, 6;

  std::cout << (data.rowwise() - xy.transpose()).rowwise().norm().minCoeff();
  return 0;
}
