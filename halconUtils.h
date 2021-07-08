#include "serialization_c11.h"
///////////////////////////////////////////////////////////////////////////////////
//    C++ serializable object and Serialize tool:                                //
//      for send message by socket or zmq easy and quickly                       //
//  20210508 halcon    gxx                                        //
//    serializationObject    //
///////////////////////////////////////////////////////////////////////////////////
#include <HalconCpp.h>

#include "common.h"
using namespace serialization_c11;
using namespace HalconCpp;

namespace halconUtils {
    class HalconUtils {
    public:
        static unsigned char* HImage2BitMap(HImage& source, unsigned char** result) {
            unsigned char* image = *result;
            Hlong width;
            Hlong height;
            source.GetImageSize(&width, &height);
            HTuple channels = source.CountChannels();
            HTuple type = source.GetImageType();
            if (strcmp(type[0].S(), "byte")) 
                return NULL;
            HString Type;
            if (channels[0].I() == 1)
            {
                image = reinterpret_cast<unsigned char*>(source.GetImagePointer1(&Type, &width, &height));
                return image;
            }
            else if (channels[0].I() == 3)
            {
                unsigned char* R, * G, * B;
                source.GetImagePointer3(reinterpret_cast<void**>(&R),
                    reinterpret_cast<void**>(&G),
                    reinterpret_cast<void**>(&B), &Type, &width, &height);
                image = new unsigned char[width * height * 3]; 
                for (int j = 0; j < height; j++)
                {
                    for (int i = 0; i < width; i++)
                    {
                        int halconIdx = j * width + i;
                        int vcIdx = (height - j - 1) * width + i;
                        image[vcIdx * 3] = R[halconIdx];
                        image[vcIdx * 3 + 1] = G[halconIdx];
                        image[vcIdx * 3 + 2] = B[halconIdx];
                    }
                }
                return image;
            }
            return NULL;
        }
    };

    class HalconResult : public SerializationOjbect {
    public:
        int getImageWidth() { return this->imageWidth; }
        void setImageWidth(int w) { this->imageWidth = w; }
        int getImageHeight() { return this->imageHeight; }
        void setImageHeight(int w) { this->imageHeight = w; }
        unsigned char* getImage() { return this->image; }
        void setImage(const unsigned char* img, unsigned int size) { memcpy_s(this->image, size, img, size); }
        void setImage(HImage& source) {
            unsigned char* p = &(image[0]);
            HalconUtils::HImage2BitMap(source, &p);
        }
    private:
        int imageWidth, imageHeight;
        unsigned char image[INT_HALCON_IMAGE_SIZE];
    };

    class HalconBurrResult : public HalconResult {
    public:
        float getPointerX(int index) { return pointX[index]; }
        void setPointerX(int index, float value) { pointX[index] = value; }

        float getPointerY(int index) { return pointY[index]; }
        void setPointerY(int index, float value) { pointY[index] = value; }

        float getDistance(int index) { return distance[index]; }
        void setDistance(int index, float value) { distance[index] = value; }

        int getMaxBurrX() { return maxBurrX; }
        void setMaxBurrX(int value) { maxBurrX = value; }

        int getMaxBurrY() { return maxBurrY; }
        void setMaxBurrY(int value) { maxBurrY = value; }

        string getFileName() { return string(fileName); }
        void setFileName(const char* v) { strncpy_s(fileName, v, INT_FILE_NAME_SIZE); }

        string getFileTime() { return string(fileTime); }
        void setFileTime(const char* v) { strncpy_s(fileTime, v, INT_FILE_TIME_SIZE); }

        int getBurrsNum() { return burrsNum; }
        void setBurrsNum(int num) { burrsNum = num; }

        int getType() { return type; }
        void setType(int t) { type = t; }
    private:
        float pointX[INT_PAINTTER_SERIAL_POINT_SIZE];
        float pointY[INT_PAINTTER_SERIAL_POINT_SIZE];
        float distance[INT_PAINTTER_SERIAL_POINT_SIZE];

        int maxBurrX;
        int maxBurrY;
        char fileName[INT_FILE_NAME_SIZE];
        char fileTime[INT_FILE_TIME_SIZE];
        int burrsNum;
        int type;
    };

}