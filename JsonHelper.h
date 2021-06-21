#ifndef JSONHELPER_H
#define JSONHELPER_H
//20210616 gxx Taize corp.
/*****************************************************************************************
json string Serialize/Desserialize , json node find/append/remove
base on <boost/property_tree/json_parser.hpp>
*****************************************************************************************/
#include <iostream>

#include "common.h"
#include "commonfunction_c.h"
#include <string.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace commonfunction_c {
	class JsonHelper {
	public:
		JsonHelper(string json): _json_string(json) {
		//do nothing
			_isInitialed = true;
		}

		JsonHelper() {
			// do nothing
		}

		void initial(string json) {
			if (_isInitialed) return;
			_json_string = json;
		}
	private:
		string _json_string;
		wstring _json_w_string;
		bool _isInitialed = false;
	};
}
#endif //JSONHELPER_H
