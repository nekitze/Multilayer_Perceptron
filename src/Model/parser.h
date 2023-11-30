#ifndef PARSER_H
#define PARSER_H

#include <array>
#include <vector>
#ifndef _TEST
#include <QImage>
#endif
#include <string>

namespace s21 {

/**
 * @brief The Parser singleton class
 */
class Parser {
 public:
  /**
   * @brief GetInstance
   * @return Pointer to Parser
   */
  static Parser* GetInstance();

  /**
   * @brief GetDataFromImage
   * @param QImage image
   * @return array of each pixel color in Grayscale from 0 to 255
   */
#ifndef _TEST
  std::array<double, 784> GetDataFromImage(QImage image);
#endif
  std::vector<std::pair<std::array<double, 784>, int>> LoadDatasetEMNIST(
      std::string path);

 private:
  Parser() {}

  static Parser* p_instance;
};

}  // namespace s21

#endif  // PARSER_H
