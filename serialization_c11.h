#ifndef SERIALIZATION_C11_H
#define SERIALIZATION_C11_H

#ifndef INT_SERIALIZABLE_OBJECT_SIZE
#define INT_SERIALIZABLE_OBJECT_SIZE 512
#endif //INT_SERIALIZABLE_OBJECT_SIZE

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
///////////////////////////////////////////////////////////////////////////////////
//    C++ serializable object and Serialize tool:                                //
//      for send message by socket or zmq easy and quickly                       //
//  20210414 最近时间比较紧，类型检查等功能先放放 gxx                            //
///////////////////////////////////////////////////////////////////////////////////
namespace serialization_c11 {

	class SerializationOjbect {
	public:
		string getVersion() { return string(version); }
		void setVersion(char* v) { strcpy(version, v); }
		int getPropNum() { return propNum; }
		void setPropNum(int num) { propNum = num; }
	private:
		char version[10];
		int propNum;

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
}

#endif //SERIALIZATION_C11_H