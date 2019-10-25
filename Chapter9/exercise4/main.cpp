#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;

//push_button's state
bool choose = false;

//button callback
void SwitchTextType_button_callback(int state, void* params);
//mouse callback
void Location_mouse_callback(int event, int x, int y, int flag, void* param);
//type text function
void typeText(cv::Mat& img, cv::QtFont& font, int x, int y);
//receive text
string receive_text(cv::QtFont& font, cv::Mat& img, bool& flag, int x, int y);
int main()
{
    cv::Mat img = cv::imread("../../1.bmp");
    if(img.empty()) return -1;
    cv::namedWindow("Window");

    cv::createButton("SwitchTextType", SwitchTextType_button_callback, nullptr, cv::QT_PUSH_BUTTON);
    cv::setMouseCallback("Window", Location_mouse_callback, static_cast<void*>(&img));

    while(1){
        cv::imshow("Window", img);
        char c = static_cast<char>(cv::waitKey(33));
        if(c == 27)
            break;
    }
    return 0;
}
//button callback
void SwitchTextType_button_callback(int state, void* params)
{
    if(choose){
        choose = false;
        cv::displayOverlay("Window", "TextType OFF", 0);
    }else{
        choose = true;
        cv::displayOverlay("Window", "TextType ON", 0);
    }
}
//mouse callback
void Location_mouse_callback(int event, int x, int y, int flag, void* param)
{
    if(choose && event == cv::EVENT_LBUTTONDOWN){
        cv::Mat& img = *static_cast<cv::Mat*>(param);
        cv::QtFont font = cv::fontQt("Chiller", 20, cv::Scalar(100, 214, 246));
        //当location 太靠上， 发出警告
        if(y < 20){
            cv::displayOverlay("Window", "The location choosed can't type text", 2000);
            return;
        }

        typeText(img, font, x, y);
    }
}
//type text function
void typeText(cv::Mat& img, cv::QtFont& font, int x, int y)
{
    string str;
    bool flag = true;

    while(flag){
        str = receive_text(font, img, flag, x, y);
        cv::addText(img, str, cv::Point(x, y), font);
        cv::imshow("Window", img);
        //if flag is true (whitch means last line is full), move to next line
        if(flag){
            x = 0;
            y += 20;
        }
    }
}
//receive text
string receive_text(cv::QtFont& font, cv::Mat& img, bool& flag, int x, int y)
{
    string str;
    char c;
    bool upperLetter = false;

    while(1){
        c = static_cast<char>(cv::waitKey(0));
        //if c is "BACKSPACE"
        if(c == 8){
            //delete a character if str is not empty
            if(!str.empty()){
                str = str.substr(0, str.size()-1);
            }else{
                cv::displayOverlay("Window", "The text is empty!", 1000);
            }
        }else if(x + static_cast<int>(8 * (str.size()+1) ) >= img.cols){
            //if this line is full, set the flag to true, and return str
            flag = true;
            cv::displayOverlay("Window", "Alright, this line is full, let's move to the next", 1000);
            return str;
        }else if(c == 13){
            //if c is "ENTER", set flag to false(whitch means no next line), and return str
            flag = false;
            return str;
        }else if(c>= 32 && c<= 126){
            //characters that can be shown, just add it to str
            char space = ' ';
            str += (c == 32 ? space : c);
        }else{
            //other characters, do nothing
        }
    }
}
