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
class CameraDevicesBase {
public:
	CameraDevicesBase() {}
	CameraDevicesBase(const char* config) {}
	~CameraDevicesBase() {}
	virtual int get_devices_num() = 0; 
	virtual bool do_capture(int index, HalconCpp::HImage& image) = 0; //������㹦�ܣ����ฺ��ʵ�֣��������sdk��,��������ֵ ��ʾ�����Ƿ�ɹ�
	virtual string get_camera_tag(int index) = 0; //��ȡ���
};

class CameraDevicesUnitTest : public CameraDevicesBase {
public:
	//��Ԫ������
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

//ͼ�����ܻ���
class ImageProcessingBase {
public:
	ImageProcessingBase() {}
	~ImageProcessingBase() {}
	virtual void set_cameras(CameraDevicesBase* camera) = 0;
	virtual void do_image_process(HalconCpp::HImage &result) = 0;
private:
	
};
//������

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