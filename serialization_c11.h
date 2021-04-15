#ifndef SERIALIZATION_C11_H
#define SERIALIZATION_C11_H

#ifndef INT_SERIALIZABLE_OBJECT_SIZE
#define INT_SERIALIZABLE_OBJECT_SIZE 512
#endif //INT_SERIALIZABLE_OBJECT_SIZE

#define INT_PAINTTER_SERIAL_POINT_SIZE 10
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
///////////////////////////////////////////////////////////////////////////////////
//    C++ serializable object and Serialize tool:                                //
//      for send message by socket or zmq easy and quickly                       //
//  20210414 最近时间比较紧，类型检查等功能先放放 gxx                            //
//     数据类应该是SerializationOjbect的子类而不是直接使用serializationObject    //
///////////////////////////////////////////////////////////////////////////////////
namespace serialization_c11 {

	class SerializationOjbect {
	public:
		string getVersion() { return string(version); }
		void setVersion(char* v) { strcpy(version, v); }
		int getMemberNum() { return memberNum; }
		void setMemberNum(int num) { memberNum = num; }
	private:
		char version[10];
		int memberNum;
	};



	class SerializationFactory{
	public:
		static void Serialize(const SerializationOjbect* o, char** result) {  
			memcpy(*result, (char*)o, INT_SERIALIZABLE_OBJECT_SIZE);
			int i = 1;
		}
		static SerializationOjbect* Deserialize(char* d) { return (SerializationOjbect*)d; }
		void initial(SerializationOjbect* o) { memcpy((char*)data[0], (char*)o, INT_SERIALIZABLE_OBJECT_SIZE); initialed = true; }
		char* getChars() { return (char*)data; }
	private:
		bool initialed = false;
		char data[INT_SERIALIZABLE_OBJECT_SIZE];
	};

	class BurrsPainter :public SerializationOjbect {
	public:
		float getPointerX(int index) { return pointX[index]; }
		void setPointerX(int index, float value) { pointX[index] = value; }

		float getPointerY(int index) { return pointY[index]; }
		void setPointerY(int index, float value) { pointY[index] = value; }

		float getDistance(int index) { return distance[index]; }
		void setDistance(int index, float value) { distance[index] = value; }

		string getFileName() { return string(fileName); }
		void setFileName(char* v) { strcpy(fileName, v); }
	private:
		float pointX[INT_PAINTTER_SERIAL_POINT_SIZE];
		float pointY[INT_PAINTTER_SERIAL_POINT_SIZE];
		float distance[INT_PAINTTER_SERIAL_POINT_SIZE];
		char fileName[40];
	};
}

#endif //SERIALIZATION_C11_H