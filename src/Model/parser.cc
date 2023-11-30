#include "parser.h"

#include <fstream>
#include <sstream>

namespace s21 {

Parser* Parser::p_instance = 0;

Parser* Parser::GetInstance() {
  if (!p_instance) p_instance = new Parser();
  return p_instance;
}

#ifndef _TEST
std::array<double, 784> Parser::GetDataFromImage(QImage image) {
  QTransform rotateTransform;
  rotateTransform.rotate(90);
  image = image.transformed(rotateTransform);
  image = image.mirrored(true, false);
  //  image.save("/Users/steinbrp/Desktop/1.png");
  std::array<double, 784> data;
  QImage normalized_image = image.convertToFormat(QImage::Format_Grayscale8)
                                .scaled(28, 28, Qt::KeepAspectRatio);

  int pixelIndex = 0;
  for (int y = 0; y < 28; ++y) {
    for (int x = 0; x < 28; ++x) {
      double grayValue =
          static_cast<double>(normalized_image.pixelColor(x, y).value()) / 255;
      data[pixelIndex++] = grayValue;
    }
  }

  return data;
}
#endif

std::vector<std::pair<std::array<double, 784>, int>> Parser::LoadDatasetEMNIST(
    std::string path) {
  std::vector<std::pair<std::array<double, 784>, int>> out;
  std::ifstream fin(path);

  if (!fin.is_open()) {
  } else {
    std::string str;
    while (std::getline(fin, str)) {
      std::pair<std::array<double, 784>, int> data;
      std::istringstream ss(str);
      std::string token;

      int i = 0;
      while (std::getline(ss, token, ',')) {
        if (i == 0)
          data.second = std::stoi(token) - 1;
        else
          data.first[i - 1] = std::stod(token) / 255.0;
        i++;
      }

      out.push_back(data);
    }
  }

  fin.close();
  return out;
}

}  // namespace s21
