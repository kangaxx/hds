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
        enum Rectangle_Angle_Pos
        {
            UPPER_LINE = 0, //取出组成上方的线（的角点）
            LEFT_LINE = 1, //取出组成左侧的线（的角点）
            RIGHT_LINE = 2, //取出组成右侧的线（的角点）
            LOWER_LINE = 3 //取出组成下方的线（的角点）
        };
        /// <summary>
        /// 根据参数取出仿射矩形的边，正常情况下，上下左右只有一条边，但是特定情况下（45度正方形）会有两条
        /// </summary>
        /// <param name="pos"></param>
        /// <param name="row"></param>
        /// <param name="column"></param>
        /// <param name="phi"></param>
        /// <param name="l1"></param>
        /// <param name="l2"></param>
        /// <param name="points"></param>
        /// <returns>变得数量， 当数量为2 时需要注意</returns>
        static int get_rectangle_angle_point(Rectangle_Angle_Pos pos, HTuple row, HTuple column, HTuple phi, HTuple l1, HTuple l2, vector<HTuple>& points_row, vector<HTuple>& points_col) {
            HTuple sin, cos;
            HTuple a, b, c, d, e, f, g, h;
            TupleSin(phi, &sin);
            TupleCos(phi, &cos);
            a = -1.0 * l1 * cos - l2 * sin;
            b = -1.0 * l1 * sin + l2 * cos;
            c = l1 * cos - l2 * sin;
            d = l1 * sin + l2 * cos;
            e = -l1 * cos + l2 * sin;
            f = -l1 * sin - l2 * cos;
            g = l1 * cos + l2 * sin;
            h = l1 * sin - l2 * cos;
            HTuple row1, row2, row3, row4, col1, col2, col3, col4;
            row1 = row - b;
            col1 = column + a;
            row2 = row - d;
            col2 = column + c;
            row3 = row - f;
            col3 = column + e;
            row4 = row - h;
            col4 = column + g; 
            int result_num = 0;
            if (Rectangle_Angle_Pos::UPPER_LINE == pos) {
                if (row1 <= row) {
                    points_row.push_back(row1);
                    points_col.push_back(col1);
                    result_num++;
                }
                if (row2 <= row) {
                    points_row.push_back(row2);
                    points_col.push_back(col2);
                    result_num++;
                }
                if (row3 <= row) {
                    points_row.push_back(row3);
                    points_col.push_back(col3);
                    result_num++;
                }
                if (row4 <= row) {
                    points_row.push_back(row4);
                    points_col.push_back(col4);
                    result_num++;
                }
            }
            else if (Rectangle_Angle_Pos::LEFT_LINE == pos) {
                if (col1 <= column) {
                    points_row.push_back(row1);
                    points_col.push_back(col1);
                    result_num++;
                }
                if (col2 <= column) {
                    points_row.push_back(row2);
                    points_col.push_back(col2);
                    result_num++;
                }
                if (col3 <= column) {
                    points_row.push_back(row3);
                    points_col.push_back(col3);
                    result_num++;
                }
                if (col4 <= column) {
                    points_row.push_back(row4);
                    points_col.push_back(col4);
                    result_num++;
                }
            }
            else if (Rectangle_Angle_Pos::RIGHT_LINE == pos) {
                if (col1 >= column) {
                    points_row.push_back(row1);
                    points_col.push_back(col1);
                    result_num++;
                }
                if (col2 >= column) {
                    points_row.push_back(row2);
                    points_col.push_back(col2);
                    result_num++;
                }
                if (col3 >= column) {
                    points_row.push_back(row3);
                    points_col.push_back(col3);
                    result_num++;
                }
                if (col4 >= column) {
                    points_row.push_back(row4);
                    points_col.push_back(col4);
                    result_num++;
                }
            }
            else if (Rectangle_Angle_Pos::LOWER_LINE == pos) {
                if (row1 >= row) {
                    points_row.push_back(row1);
                    points_col.push_back(col1);
                    result_num++;
                }
                if (row2 >= row) {
                    points_row.push_back(row2);
                    points_col.push_back(col2);
                    result_num++;
                }
                if (row3 >= row) {
                    points_row.push_back(row3);
                    points_col.push_back(col3);
                    result_num++;
                }
                if (row4 >= row) {
                    points_row.push_back(row4);
                    points_col.push_back(col4);
                    result_num++;
                }
            }
            if (result_num != 2)
                return true;
            else
                return false;
        }
        static HObject char_to_halcon_image(char* data, int width, int height) {
            HObject  ho_Image;
            GenImage1Extern(&ho_Image, "byte", width, height, (Hlong)data, NULL); //由相机传入
            return ho_Image;
        }

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

    //图片的额外信息
    class HImageExtraInfo {
    public:
        HImageExtraInfo(HImage& other) {
            _image = other;
        }
        void set_job_id(int id) {
            _job_id = id;
        }
        int get_job_id() { return _job_id; }
        void set_camera_id(int id) { _camera_id = id; }
        int get_camera_id() { return _camera_id; }
        HImage& get_image() { return _image; }
        void set_total_count(int v) { _total_camera_count = v; }
        int get_total_count() { return _total_camera_count; }
    private:
        HImage _image;
        HImage* _temp_image; //特定情况下只有指针
        int _job_id;
        int _camera_id;
        int _total_camera_count;
        string _camera_tag;
    };
}