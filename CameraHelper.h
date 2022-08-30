/******************************************************/
//create by gxx 20220616
//工业相机通用管理类
/******************************************************/
#pragma once
#include <iostream>
using namespace std;
namespace commonfunction_c {
	typedef enum CAMERA_POSITION {
		CAMERA_POSITION_LEFT = 1,
		CAMERA_POSITION_MID = 2,
		CAMERA_POSITION_RIGHT = 3,
		CAMERA_POSITION_TOP = 4,
		CAMERA_POSITION_BOTTOM = 5,
	}CAMERA_POSITION;
//工厂模式产品类,任意品牌相机的基类
class CameraDevicesBase {
public:
	CameraDevicesBase() {}
	CameraDevicesBase(const char* config) {}
	~CameraDevicesBase() {}
	virtual int get_devices_num() = 0; 
	virtual bool do_capture(int index, HalconCpp::HImage& image) = 0; //相机拍摄功能，子类负责实现（调用相机sdk）,函数返回值 表示拍摄是否成功
	virtual string get_camera_tag(int index) = 0; //获取相机
};

class CameraDevicesUnitTest : public CameraDevicesBase {
public:
	//单元测试用
	CameraDevicesUnitTest() {
		//do nothing
	}
	~CameraDevicesUnitTest() {}
	bool do_capture(int index, HalconCpp::HImage& image) {
		try {
			return true;
		}
		catch (...) {
			return false;
		}
	}

	string get_camera_tag(int index) {
		return "";
	}
};

//图像处理功能基类
class ImageProcessingBase {
public:
	ImageProcessingBase() {}
	~ImageProcessingBase() {}
	virtual void set_cameras(CameraDevicesBase* camera) = 0;
	virtual void do_image_process(HalconCpp::HImage &result) = 0;
private:
	
};
//工厂类

class CameraHelper {
public:
	static CameraHelper& get_instance() { 
		static CameraHelper instance;
		return instance; 
	}

	void initial(CameraDevicesBase* camera) {
		_camera = camera;
	}
	void do_capture(int index, HalconCpp::HImage image) {
		if (_camera != nullptr)
			_camera->do_capture(index, image);
	}
private:
	CameraHelper() {

	}
	~CameraHelper() {}
	CameraHelper(const CameraHelper&);
	CameraHelper& operator=(CameraHelper&);
	int _camera_num = 0;
	CameraDevicesBase* _camera = nullptr;
};

}