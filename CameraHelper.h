/******************************************************/
//create by gxx 20220616
//��ҵ���ͨ�ù�����
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
//����ģʽ��Ʒ��,����Ʒ������Ļ���
class CameraDevicesParent {
public:
	CameraDevicesParent() {}
	CameraDevicesParent(const char* config) {}
	~CameraDevicesParent() {}
	virtual bool do_capture(int index, char** result) = 0; //������㹦�ܣ����ฺ��ʵ�֣��������sdk��,��������ֵ ��ʾ�����Ƿ�ɹ�
};

class CameraDevicesUnitTest : public CameraDevicesParent {
public:
	//��Ԫ������
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
//������
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