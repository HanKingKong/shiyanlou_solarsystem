//
// mian.cpp
//
#include <opencv2/opencv.hpp>
bool selectObject = false;  // 用于标记是否有选取目标
int trackObject = 0;        // 1 表示有追踪对象， 0 表示无追踪对象
                            // -1 表示追踪对象尚未计算  Camshift 所需属性
cv::Rect selection;     // 保存鼠标选择的区域
cv::Mat image;          // 用于缓存读取到的视频帧

void onMouse(int event, int x, int y, int, void*){
    static cv::Point origin;
    if(selectObject){
        // 确定鼠标选定区域的左上角以及区域的长和宽
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x-origin.x);
        selection.height= std::abs(y-origin.y);

        selection &= cv::Rect(0,0,image.cols, image.rows);
    }

    switch(event){
        case CV_EVENT_LBUTTONDOWN:
            origin = cv::Point(x, y);
            selection=cv::Rect(x, y, 0,0);
            selectObject = true;
            break;
        case CV_EVENT_LBUTTONUP:
            selectObject = false;
            if(selection.width >0 && selection.height > 0)
                trackObject = -1; // 追踪的目标还未计算 Camshift 所需属性
            break;

    }

}

const char* WIN_NAME = "CamShift at Shiyanlou";

int main() {
    cv::VideoCapture video("video.ogv");
    cv::namedWindow(WIN_NAME);

    cv::setMouseCallback(WIN_NAME, onMouse, NULL);

    cv::Mat frame;
    cv::Mat hsv, hue, mask, hist, backproj;
    cv::Rect trackWindow;           // 追踪到的窗口
    int hsize = 16;
    float hranges[] = {0, 180};
    const float* phranges = hranges;

    while(true){
        video >> frame;
        if(frame.empty()) break;

        frame.copyTo(image);
        // 转换到HSV空间
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        if(trackObject){
            cv::inRange(hsv, cv::Scalar(0, 30, 10), cv::Scalar(180, 256, 10), mask);
            int ch[] ={0,0};
            hue.create(hsv.size(), hsv.depth());
            cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);

            if(trackObject<0){
                // 设置H 通道和mask通道的ROI
                cv::Mat roi(hue, selection), maskroi(mask, selection);
                // 计算ROI所在区域的直方图
                calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
                // 将直方图归一
                normalize(hist, hist, 0, 255, CV_MINMAX);
                // 设置追踪的窗口
                trackWindow = selection;
                // 标记追踪的目标已经计算过直方图属性
                trackObject = 1;

            }

            // 将直方图进行方向投影
            calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
            // 取公共部分
            backproj &= mask;

            // 调用 Camshift算法接口
            cv::RotatedRect trackBox = CamShift(backproj, trackWindow,
                    cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));
            // 处理追踪妙计过小的情况
            if( trackWindow.area() <= 1){
                int cols = backproj.cols, rows = backproj.rows,
                    r = (MIN(cols, rows)+5)/6;
                trackWindow = cv::Rect(trackWindow.x - r, trackWindow.y - r,
                        trackWindow.x + r, trackWindow.y + r) & cv::Rect(0, 0, cols, rows);
        }
        // 绘制追踪区域
        ellipse( image, trackBox, cv::Scalar(0,0,255), 3, CV_AA);
        }
        if(selectObject && selection.width >0 && selection.height >0){
            cv::Mat roi(image, selection);
            bitwise_not(roi, roi);  // 对选择的区域图像反色
        }

        cv::imshow(WIN_NAME, frame);

        int key = cv::waitKey(1000/15);

        if (key == 27) break;
    }

    cv::destroyAllWindows();
    video.release();
    return 0;
}
