#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

#define WINDOW_X                         450     //显示整张图片的window的左上角坐标
#define WINDOW_Y                         350

#define MAGNIFICATION_WINDOW_EDGE_LENGTH 200     //magWindow的边长

#define ADDLENGTH                        100     //生成img_blackBorder时扩充的长度，这里取和 MAGNIFICANT_TIMES_1一样的值，就够用了

#define MAGNIFICATION_TIMES_1            100     //1X 时所选取的原图边长
#define MAGNIFICATION_TIMES_2            50      //2X
#define MAGNIFICATION_TIMES_3            25      //3X
#define MAGNIFICATION_TIMES_4            12      //4X

bool magnification_on = false;        //控制放大的开关
int magnification_times = 0;          //控制放大的倍数

//button_callback
void controlMagnification_button_callback(int state, void* params);
//trackbar_callback
void setMagnificationTimes_trackbar_callback(int pos, void* param);
//mouse_callback
void showMagnification_mouse_callback(int event, int x, int y, int flag, void* param);
//help mouse_callback
void showTheMagnification(cv::Mat& img_blackBorder, cv::Mat& mag_img, int x, int y, int selectedRigionEdgeLength, int times);

int main()
{
    cv::Mat img = cv::imread("../../1.bmp");
    if(img.empty()) return -1;

    //设置一个加了黑边框的image, 其内部和原image一样， 方便到时候对边界放大问题进行处理
    cv::Mat img_blackBorder = cv::Mat::zeros(img.rows+ADDLENGTH,
                                             img.cols+ADDLENGTH,
                                             CV_8UC3);
    cv::Mat img_inside(img_blackBorder(cv::Range(ADDLENGTH/2, img_blackBorder.rows-ADDLENGTH/2),
                                       cv::Range(ADDLENGTH/2, img_blackBorder.cols-ADDLENGTH/2)));
    img.copyTo(img_inside);

    //把显示整张图片的Window的位置设定，方便等会显示magWindow
    cv::namedWindow("Window");
    cv::moveWindow("Window", WINDOW_X, WINDOW_Y);

    //create push_botton
    cv::createButton("ControlMagnification", controlMagnification_button_callback, nullptr, cv::QT_PUSH_BUTTON, false);
    //create trackbar
    cv::createTrackbar("SetMagnificationTimes", "", &magnification_times, 3, setMagnificationTimes_trackbar_callback);
    //setMouseCallback
    cv::setMouseCallback("Window", showMagnification_mouse_callback, static_cast<void*>(&img_blackBorder));

    cv::displayOverlay("Window", "Magnification OFF", 0);
    cv::imshow("Window", img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
//button_callback
void controlMagnification_button_callback(int state, void* params)
{
    if(magnification_on){
        magnification_on = false;
        cv::displayOverlay("Window", "Magnification OFF", 0);
        cv::destroyWindow("magWindow");
    }else{
        magnification_on = true;
        cv::displayOverlay("Window", "Magnification ON", 0);

        //显示push botton后才创建 用于display magnification 的 window
        cv::namedWindow("magWindow", cv::WINDOW_NORMAL | cv::WINDOW_GUI_NORMAL);
        cv::resizeWindow("magWindow", MAGNIFICATION_WINDOW_EDGE_LENGTH, MAGNIFICATION_WINDOW_EDGE_LENGTH);
        cv::moveWindow("magWindow", WINDOW_X, WINDOW_Y - MAGNIFICATION_WINDOW_EDGE_LENGTH - 40);
    }
}
//trackbar_callback
void setMagnificationTimes_trackbar_callback(int pos, void* param)
{

}
//mouse_callback
void showMagnification_mouse_callback(int event, int x, int y, int flag, void* param)
{
    //if push_botton is on and event is mouse_move
    if(magnification_on && event == cv::EVENT_MOUSEMOVE){
        cv::Mat& img_blackBorder = *static_cast<cv::Mat*>(param);
        cv::Mat mag_img;             //mag_img为被放大的那部分图片
        //根据trackbar所设的放大倍数来进行不同操作
        switch (magnification_times) {
        case 0 : {
            showTheMagnification(img_blackBorder, mag_img, x, y, MAGNIFICATION_TIMES_1, 1);
            break;
        }
        case 1 : {
            showTheMagnification(img_blackBorder, mag_img, x, y, MAGNIFICATION_TIMES_2, 2);
            break;
        }
        case 2 : {
            showTheMagnification(img_blackBorder, mag_img, x, y, MAGNIFICATION_TIMES_3, 3);
            break;
        }
        case 3 : {
            showTheMagnification(img_blackBorder, mag_img, x, y, MAGNIFICATION_TIMES_4, 4);
            break;
        }
        default :
            break;
        }
    }
}
//help mouse_callback
void showTheMagnification(cv::Mat& img_blackBorder, cv::Mat& mag_img, int x, int y, int selectedRigionEdgeLength, int times)
{
    //这里是先把x，y + ADDLENGTH/2 ，使他们在img_blackBorder中指向和img一样的位置，再进行选框
    mag_img = img_blackBorder(cv::Range(y + ADDLENGTH/2 - selectedRigionEdgeLength/2, y + ADDLENGTH/2 + selectedRigionEdgeLength/2),
                              cv::Range(x + ADDLENGTH/2 - selectedRigionEdgeLength/2, x + ADDLENGTH/2 + selectedRigionEdgeLength/2));
    //move the magnification window
    cv::moveWindow("magWindow",
                   x + WINDOW_X - MAGNIFICATION_WINDOW_EDGE_LENGTH/2,
                   WINDOW_Y - MAGNIFICATION_WINDOW_EDGE_LENGTH - 40);
    string str = to_string(times) + "X";
    cv::displayOverlay("magWindow", str, 0);
    cv::imshow("magWindow", mag_img);
}
