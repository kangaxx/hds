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
class CameraDevicesParent {
public:
	CameraDevicesParent() {}
	CameraDevicesParent(const char* config) {}
	~CameraDevicesParent() {}
	virtual bool do_capture(int index, char** result) = 0; //相机拍摄功能，子类负责实现（调用相机sdk）,函数返回值 表示拍摄是否成功
};

class CameraDevicesUnitTest : public CameraDevicesParent {
public:
	//单元测试用
	CameraDevicesUnitTest() {
		//do nothing
	}
	~CameraDevicesUnitTest() {}
	bool do_capture(int index, char** result) {
		try {
			strcpy_s(*result, 4, "cam");
			return true;
		}
		catch (...) {
			return false;
		}
	}
};
//工厂类
class CameraHelper {
public:
	static CameraHelper& get_instance() { 
		static CameraHelper instance;
		return instance; 
	}

	void initial(CameraDevicesParent* camera) {
		_camera = camera;
	}
	void do_capture(int index, char** result) {
		if (_camera != nullptr)
			_camera->do_capture(index, result);
	}
private:
	CameraHelper() {

	}
	~CameraHelper() {}
	CameraHelper(const CameraHelper&);
	CameraHelper& operator=(CameraHelper&);
	int _camera_num = 0;
	CameraDevicesParent* _camera = nullptr;
};

}