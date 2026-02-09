#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <caminho>\n";
    return -1;
  }

  cv::Mat image = cv::imread(argv[1]);
  if (image.empty()) {
    std::cerr << "Empty image\n";
    return -1;
  }

  cv::Mat hsvImage;
  cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
  cv::Scalar lowerWhite = cv::Scalar(0, 0, 50);
  cv::Scalar upperWhite = cv::Scalar(180, 30, 180);

  cv::Mat whiteMask;
  cv::inRange(hsvImage, lowerWhite, upperWhite, whiteMask);

  cv::Mat blurredImage;
  // Gauss Filter
  // cv::GaussianBlur(whiteMask, blurredImage, cv::Size(5, 5), 0);
  // Canny
  cv::Mat edges;
  int threshold1 = 100;
  int threshold2 = 150;
  cv::Canny(whiteMask, edges, threshold1, threshold2);

  // cv::imshow("Imagem White", whiteMask);
  cv::imshow("Imagem Edges", edges);
  cv::waitKey(0);

  return 0;
}
