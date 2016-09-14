#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#include "detect.hh"
#include "window.hh"
#include "classification.hh"

std::vector<bbox> detect(const std::string &img_path,
                         const std::string &classifier_path)
{
  // load image from file
  cv::Mat img = cv::imread(img_path, CV_LOAD_IMAGE_GRAYSCALE);
  int img_width = img.rows;
  int img_height = img.cols;

  // compute integral image
  cv::Mat integral;
  cv::integral(img, integral);

  // load classifier from file
  mblbp_classifier classifier = load_classifier(classifier_path);

  std::vector<window> potential_windows = get_potential_windows(img_width,
                                                                img_height);
  std::vector<window> positive_windows;

  for(const auto &potential_window : potential_windows)
  {
    bool positive = classifier.classify(integral, potential_window);

    if(positive)
      positive_windows.push_back(potential_window);
  }

  return aggregate_windows(img_width, img_height, positive_windows);
}
