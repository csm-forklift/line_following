#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv)
{
    //===== Read in an image for testing =====//
    cv::Mat input_img;
    input_img = cv::imread("test_images/yellow_line_15.jpg");

    // Convert image to HSV (Maximums, Hue: 179, Saturation: 255, Value: 255)
    cv::Mat input_hsv;
    cv::cvtColor(input_img, input_hsv, CV_BGR2HSV);

    // Thresholding Values
    int low[] = {20, 0, 0};
    int high[] = {40, 255, 255};
    //===== Read in an image for testing =====//


    //===== Create test image for check HSV values =====//
    // Create test image, convert to HSV
    //cv::Mat test_hsv(300,300,CV_8UC3);
    cv::Mat test_hsv = input_hsv.clone();
    cv::namedWindow("HSV Testing", CV_WINDOW_AUTOSIZE);

    // Create trackbars
    int hue = 125;
    int saturation = 125;
    int value = 255;
    int h_max = 179;
    int s_max = 255;
    int v_max = 255;

    /*
    // Single value testing
    cv::createTrackbar("Hue", "HSV Testing", &hue, h_max, NULL);
    cv::createTrackbar("Saturation", "HSV Testing", &saturation, s_max, NULL);
    cv::createTrackbar("Value", "HSV Testing", &value, v_max, NULL);

    while (1) {
        // Change image HSV values
        for (int row = 0; row < test_hsv.rows; row++) {
            for (int col = 0; col < test_hsv.cols; col++) {
                cv::Vec3b intensity(hue, saturation, value);
                test_hsv.at<cv::Vec3b>(row, col) = intensity;
            }
        }

        // Show image
        cv::Mat test_bgr;
        cv::cvtColor(test_hsv, test_bgr, CV_HSV2BGR);
        cv::imshow("HSV Testing", test_bgr);
        if (cv::waitKey(10) == 32) {
            break;
        }
    }
    */

    // Threshold testing
    cv::createTrackbar("Hue Low", "HSV Testing", &low[0], h_max, NULL);
    cv::createTrackbar("Hue High", "HSV Testing", &high[0], h_max, NULL);
    cv::createTrackbar("Saturation Low", "HSV Testing", &low[1], s_max, NULL);
    cv::createTrackbar("Saturation High", "HSV Testing", &high[1], s_max, NULL);
    cv::createTrackbar("Value Low", "HSV Testing", &low[2], v_max, NULL);
    cv::createTrackbar("Value High", "HSV Testing", &high[2], v_max, NULL);

    while (1) {
        // Separate HSV into individual channeals
        cv::Mat test_planes[3];
        split(test_hsv, test_planes);

        // Threshold the image based on high and low HSV values
        for (int i = 0; i < 3; i++) {
            cv::Mat imageThreshLow, imageThreshHigh;

            // Cut values outside low and high
            threshold(test_planes[i], imageThreshLow, low[i], 255, cv::THRESH_BINARY);
            threshold(test_planes[i], imageThreshHigh, high[i], 255, cv::THRESH_BINARY_INV);

            // Store threshold values back into image plane
            bitwise_and(imageThreshLow, imageThreshHigh, test_planes[i]);
        }

        // Merge all of the planes together
        cv::Mat image_result(cv::Size(test_hsv.cols, test_hsv.rows), CV_8UC1, cv::Scalar(255));
        for (int i = 0; i < 3; i++) {
            bitwise_and(image_result, test_planes[i], image_result);
        }

        // Show image
        cv::imshow("HSV Testing", image_result);
        if (cv::waitKey(10) == 32) {
            break;
        }
    }
    //===== Create test image for check HSV values =====//


    //===== Isolate yellow from the image =====//
    // Separate HSV into individual channeals
    cv::Mat planes[3];
    split(input_hsv, planes);

    // Threshold the image based on high and low HSV values
    for (int i = 0; i < 3; i++) {
        cv::Mat imageThreshLow, imageThreshHigh;

        // Cut values outside low and high
        threshold(planes[i], imageThreshLow, low[i], 255, cv::THRESH_BINARY);
        threshold(planes[i], imageThreshHigh, high[i], 255, cv::THRESH_BINARY_INV);

        // Store threshold values back into image plane
        bitwise_and(imageThreshLow, imageThreshHigh, planes[i]);
    }

    // Merge all of the planes together
    cv::Mat image_result(cv::Size(input_hsv.cols, input_hsv.rows), CV_8UC1, cv::Scalar(255));
    for (int i = 0; i < 3; i++) {
        bitwise_and(image_result, planes[i], image_result);
    }
    //===== Isolate yellow from the image =====//

    //===== Testing Window =====//
    char w_name[] = "Testing View";
    cv::namedWindow(w_name, CV_WINDOW_AUTOSIZE);
    cv::imshow(w_name, input_img);
    cv::imshow("Threshold", input_hsv);

    //===== Wait for Key =====//
    cv::waitKey(0);

    return 0;
}
