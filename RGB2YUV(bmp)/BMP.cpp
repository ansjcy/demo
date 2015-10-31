#include "BMP.hpp"
void BMP::getHead(FILE* fp)
{
    fread(&thisHead,1,sizeof(head),fp);
}
void BMP::showHead()
{
    cout<<"size:"<<thisHead.size<<endl;
    cout<<"reserve1:"<<thisHead.reserve1<<endl;
    cout<<"reserve2:"<<thisHead.reserve2<<endl;
    cout<<"offset:"<<thisHead.offset<<endl;
}
void BMP::getInfo(FILE* fp)
{
    fread(&thisInfo,1,sizeof(information),fp);
}
void BMP::showInfo()
{
    cout<<"size:"<<thisInfo.infoSize<<endl;
    cout<<"width:"<<thisInfo.width<<endl;
    cout<<"height:"<<thisInfo.height<<endl;
    cout<<"planes:"<<thisInfo.planes<<endl;
    cout<<"bitCounts:"<<thisInfo.bitCount<<endl;
    cout<<"compressType:"<<thisInfo.compressionType<<endl;
    cout<<"imageSize:"<<thisInfo.imageSize<<endl;
    cout<<"xPelsPerMeter:"<<thisInfo.XPelsPerMeter<<endl;
    cout<<"yPelsPerMeter:"<<thisInfo.YPelsPerMeter<<endl;
    cout<<"colorNum:"<<thisInfo.colorNum<<endl;
    cout<<"colorImportant:"<<thisInfo.colorImportantNum<<endl;
}
//void BMP::getPalette(FILE* fp)
//{
//    for(unsigned int i=0;i<thisInfo.colorNum;i++)
//    {
//        fread((char *)&(thisPalette[i].rgbB),1,sizeof(unsigned char),fp);
//        fread((char *)&(thisPalette[i].rgbG),1,sizeof(unsigned char),fp);
//        fread((char *)&(thisPalette[i].rgbR),1,sizeof(unsigned char),fp);
//        fread((char *)&(thisPalette[i].rgbRES),1,sizeof(unsigned char),fp);
//    }
//}
//void BMP::showPalette()
//{
//    //cout<<thisPalette<<endl;
//}
void BMP::getData(FILE* fp)
{
    fread(thisData,thisInfo.imageSize * sizeof(data)  / 3, 1,fp);
}
void BMP::showData()
{
    for (int i = 0; i < thisInfo.imageSize / 3; i++) {
        cout<<"B:"<<(int)thisData[i].blue<<" "
            <<"G:"<<(int)thisData[i].green<<" "
            <<"R:"<<(int)thisData[i].red<<endl;
    }
}
void BMP::allocateData()
{
    //cout<<"-------------------------"<<sizeof(data);
    thisData = (data*)malloc(thisInfo.imageSize*sizeof(data)/3);
    for (int i = 0; i < thisInfo.imageSize / 3; i++) {
        thisData[i].blue = 0;
        thisData[i].green = 0;
        thisData[i].red = 0;
    }
}
void BMP::writeData(FILE* fpw)
{
    unsigned short BMPIndicator= 0x4d42;
    fwrite(&BMPIndicator,1,sizeof(unsigned short),fpw);
    fwrite(&thisHead,1,sizeof(head),fpw);
    fwrite(&thisInfo,1,sizeof(information),fpw);
    
    int i = 0, j = 0;
    while(true){
        if(j >= thisInfo.imageSize)
            break;
        
        fwrite(&(thisData[i].blue), 1, sizeof(unsigned char), fpw);
        ++j;
        
        if(j >= thisInfo.imageSize)
            break;
        fwrite(&(thisData[i].green), 1, sizeof(unsigned char), fpw);
        ++j;
        
        if(j >= thisInfo.imageSize)
            break;
        fwrite(&(thisData[i].red), 1, sizeof(unsigned char), fpw);
        ++j;
        
        ++i;
    }
}

void BMP::toGrew(FILE* fpw)
{
    unsigned short BMPIndicator= 0x4d42;
    fwrite(&BMPIndicator,1,sizeof(unsigned short),fpw);
    fwrite(&thisHead,1,sizeof(head),fpw);
    fwrite(&thisInfo,1,sizeof(information),fpw);
    
    thisYUV = (yuv*)malloc(sizeof(yuv)*thisInfo.imageSize/3);
    
    int i = 0, j = 0;
    cout<<"here!!!";
    while(true){
        if(j >= thisInfo.imageSize)
            break;
        
        //0.256788 * R + 0.504129 * G + 0.097906 * B
        //-0.148223 * R - 0.290993 * G + 0.439216 * B
        //0.439216 * R - 0.367788 * G - 0.071427 * B
        
//        thisYUV[i].Y = (0.256788 * thisData[i].red + 0.504129*thisData[i].green + 0.097906*thisData[i].blue)+16;
//        thisYUV[i].U = (-0.148223 * thisData[i].red -0.290993*thisData[i].green + 0.439216*thisData[i].blue)+128;
//        thisYUV[i].V = (0.439216 * thisData[i].red -0.367788*thisData[i].green -0.071427*thisData[i].blue)+128;
        
    
        thisYUV[i].Y = (((66 * thisData[i].red + 129*thisData[i].green + 25*thisData[i].blue+128)>>8)+16);
        thisYUV[i].U = ((-38 * thisData[i].red -74*thisData[i].green + 112*thisData[i].blue+128)>>8)+128;
        thisYUV[i].V = ((112 * thisData[i].red -94*thisData[i].green -18*thisData[i].blue+128)>>8)+128;
        
        
        if (thisYUV[i].Y <= 0) {
            thisYUV[i].Y = 0;
        }
        else if(thisYUV[i].Y>255)
        {
            thisYUV[i].Y = 255;
        }
        
        fwrite(&(thisYUV[i].Y), 1, sizeof(unsigned char), fpw);
        ++j;
        
        if(j >= thisInfo.imageSize)
            break;
        fwrite(&(thisYUV[i].Y), 1, sizeof(unsigned char), fpw);
        ++j;
        
        if(j >= thisInfo.imageSize)
            break;
        fwrite(&(thisYUV[i].Y), 1, sizeof(unsigned char), fpw);
        ++j;
        
        ++i;
    }
   
}

void BMP::toRGB(FILE* fpw)
{
    unsigned short BMPIndicator= 0x4d42;
    fwrite(&BMPIndicator,1,sizeof(unsigned short),fpw);
    fwrite(&thisHead,1,sizeof(head),fpw);
    fwrite(&thisInfo,1,sizeof(information),fpw);
    
    
    int i = 0, j = 0;
    while(true){
        if(j >= thisInfo.imageSize)
            break;
        
        //C = Y - 16
        //D = U - 128
        //E = V - 128
        //R = clip(( 298 * (Y - 16)           + 409 * (V - 128) + 128) >> 8)
        //G = clip(( 298 * (Y - 16) - 100 * (U - 128) - 208 * (V - 128) + 128) >> 8)
        //B = clip(( 298 * (Y - 16) + 516 * (U - 128)           + 128) >> 8)
        int C = thisYUV[i].Y - 16;
        int D = thisYUV[i].U - 128;
        int E = thisYUV[i].V - 128;
        int tmpR, tmpG, tmpB;
//        tmpR = 1.164383 * C                   + 1.596027 * E;//( 298 * C + 409 * E + 128) >> 8;
//        tmpG = 1.164383 * C - (0.391762 * D) - (0.812968 * E);//( 298 * C - 100 * D - 208 * E + 128) >> 8;
//        tmpB = 1.164383 * C +  2.017232 * D;//( 298 * C + 516 * D + 128) >> 8;
        
        tmpR = ( 298 * C + 409 * E + 128) >> 8;
        tmpG = ( 298 * C - 100 * D - 208 * E + 128) >> 8;
        tmpB = ( 298 * C + 516 * D + 128) >> 8;
        
        if(tmpR <= 0)
            tmpR = 0;
        if (tmpR >= 255)
            tmpR =255;
        if(tmpG <= 0)
            tmpG = 0;
        if (tmpG >= 255)
            tmpG =255;
        if(tmpB <= 0)
            tmpB = 0;
        if (tmpB >= 255)
            tmpB =255;
        
        fwrite(&(tmpB), 1, sizeof(unsigned char), fpw);
        ++j;
        
        if(j >= thisInfo.imageSize)
            break;
        fwrite(&(tmpG), 1, sizeof(unsigned char), fpw);
        ++j;
        
        if(j >= thisInfo.imageSize)
            break;
        fwrite(&(tmpR), 1, sizeof(unsigned char), fpw);
        ++j;
        
        ++i;
    }
}
