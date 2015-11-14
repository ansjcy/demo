#ifndef BMP_hpp
#define BMP_hpp

#include <iostream>
using namespace::std;
typedef struct                          //4*2+2*2+2=14
{
    //unsigned short type;
    unsigned int size;
    unsigned short reserve1;
    unsigned short reserve2;
    unsigned int offset;
}head;
typedef struct
{
    unsigned int infoSize;//信息头大小       4*9+2*2 = 40
    unsigned int col;//图像宽度
    unsigned int row;//图像高度
    unsigned short planes;//位平面数，必须为1
    unsigned short bitCount;//每像素位数
    unsigned int  compressionType; //压缩类型
    unsigned int  imageSize; //压缩图像大小字节数
    unsigned int  XPelsPerMeter; //水平分辨率
    unsigned int  YPelsPerMeter; //垂直分辨率
    unsigned int  colorNum; //位图实际用到的色彩数
    unsigned int  colorImportantNum; //本位图中重要的色彩数
}information;
typedef struct
{
    unsigned char rgbB; //该颜色的蓝色分量
    unsigned char rgbG; //该颜色的绿色分量
    unsigned char rgbR; //该颜色的红色分量
    unsigned char rgbRES; //保留值
}palette;
typedef struct
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
}data;
typedef struct
{
    int Y;
    int U;
    int V;
}yuv;

class BMP{
public:
    head thisHead;
    information thisInfo;
    palette thisPalette[256];
    data* thisData;
    yuv* thisYUV;
    
    
    void getHead(FILE* fp);
    void getInfo(FILE* fp);
    void getPalette(FILE* fp);
    void getData(FILE* fp);
    void showHead();
    void showInfo();
    void showPalette();
    void showData();
    void allocateData();
    void writeData(FILE* fpw);
    void toGrew(FILE* fpw);
    void toRGB(FILE* fpw);
    void binarization(FILE* fp);
};

#endif /* BMP_hpp */
