## This chapter request a Qt backend OpenCV, 
so you should first compile opencv source with MinGW (since Qt only support MinGW-compiled opencv version)

For me, I've tried many times to compile opencv by cmake,
but all failed, every time I type "mingw32-make"(now I change it to "make"),
it would stop at 41% or so, and prompted that something wrong with libopencv_core412.dll.

So, I have no choice to download a MinGW-compiled opencv from the internet,
but it didn't work neither.

Finally, I *compile it by QtCreator*(the method is extremely similar to the first, just using different tool, but still work with cmake)
and succeeded.
Step by step as [this](https://blog.csdn.net/scien2011/article/details/52830794)

### To use the compiled opencv library, every .pro file in a project created by QtCreator should contain the follows:
```
INCLUDEPATH += D:\opencv\opencv-build-QtCreator\include \
               D:\opencv\opencv-build-QtCreator\include\opencv2
LIBS += D:\opencv\opencv-build-QtCreator\x64\mingw\lib\libopencv_*
```
*So every project consists of main.cpp and .pro*

#Just for me
