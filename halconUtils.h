#include "serialization_c11.h"
///////////////////////////////////////////////////////////////////////////////////
//    C++ serializable object and Serialize tool:                                //
//      for send message by socket or zmq easy and quickly                       //
//       20210508 halcon    gxx                                                  //
//    serializationObject                                                        //
//    确保在编译环境中引入的halcon组件否则无法通过编译                           //
//    包含目录中需要 $(HALCONROOT)\include;$(HALCONROOT)\include\halconcpp       //
//    库目录里需要 $(HALCONROOT)\lib\$(HALCONARCH);                              //
//    附加库目录里需要 $(HALCONROOT)\lib\$(HALCONARCH)                           //
///////////////////////////////////////////////////////////////////////////////////
#include <HalconCpp.h>

#include "common.h"
using namespace serialization_c11;
using namespace HalconCpp;

namespace halconUtils {
    //halcon标定功能代码类
    class HalconCalibrationHelper {
    public:
        HalconCalibrationHelper() { ; } //不设置相机参数，可以在运行计算时输入，适合复杂相机组合模式
         //设定相机参数，单个相机模式
        HalconCalibrationHelper(HTuple camera_parameters, HTuple camera_pose, HTuple plate_description) : 
            _camera_parameters(camera_parameters), _camera_pose(camera_pose), _plate_description(plate_description){
            CreateCalibData("calibration_object", 1, 1, &_calib_handle);
            SetCalibDataCamParam(_calib_handle, 0, HTuple(), _camera_parameters);
            SetCalibDataCalibObject(_calib_handle, 0, _plate_description);
        }
        void do_image_to_world(HImage& calib_image, HTuple rows, HTuple columns, HTuple& world_y, HTuple& world_x) {
            ImagePoint2WorldPlane(_camera_parameters, _camera_pose, _plate_description, calib_image,
                rows, columns, world_y, world_x);
        }
        //将Image坐标点转换成世界坐标
        static void ImagePoint2WorldPlane(HTuple camera_parameters, HTuple camera_pose,
            HTuple plate_description, HImage& calib_image, HTuple rows, HTuple columns, HTuple& world_y, HTuple& world_x) {
            Hlong width, height;
            calib_image.GetImageSize(&width, &height);
            HTuple find_cal_obj_par_names, find_cal_obj_par_values;
            find_cal_obj_par_names[0] = "gap_tolerance";
            find_cal_obj_par_names[1] = "alpha";
            find_cal_obj_par_names[2] = "skip_find_caltab";
            find_cal_obj_par_values[0] = 1;
            find_cal_obj_par_values[1] = 1;
            find_cal_obj_par_values[2] = "false";
            HTuple calib_handle;
            CreateCalibData("calibration_object", 1, 1, &calib_handle);
            SetCalibDataCamParam(calib_handle, 0, HTuple(), camera_parameters);
            SetCalibDataCalibObject(calib_handle, 0, plate_description);
            FindCalibObject(calib_image, calib_handle, 0, 0, 0, find_cal_obj_par_names,
                find_cal_obj_par_values);
            HTuple mark_rows, mark_columns, ind;
            GetCalibDataObservPoints(calib_handle, 0, 0, 0, &mark_rows, &mark_columns,
                &ind, &camera_pose);
            SetOriginPose(camera_pose, 0.0, 0.0, 0.001, &camera_pose);
            ImagePointsToWorldPlane(camera_parameters, camera_pose, rows, columns,
                "mm", &world_x, &world_y);
        }
    private:
        HTuple _camera_parameters, _camera_pose; //halcon标定助手相机参数
        HTuple _plate_description; // halcon标定助手desc文件（文件路径名）
        HTuple _calib_handle;
        HTuple _cal_par_names, _cal_par_values;
    };

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