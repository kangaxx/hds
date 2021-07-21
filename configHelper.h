#pragma once
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "commonfunction_c.h"

using namespace std;

namespace commonfunction_c {
	enum eConfigType { CT_JSON = 0, CT_XML = 1 };
    class configHelper {
    public:
        configHelper(string file, eConfigType type) : _file(file), _type(type) {
            if (BaseFunctions::isFolderExist(file))
                _initialed = true;
            else
                _initialed = false;
        }

        template<typename T>
        T findValue(string name, T t) {
            if (!_initialed)
                throw "Error, configHelper not initialed!";
            if (_type == eConfigType::CT_JSON) {
                boost::property_tree::ptree root;
                boost::property_tree::read_json(_file, root);
                return root.get<T>(name);
            }//json end
        }

    private:
        string _file;
        eConfigType _type;
        bool _initialed;
    };
}
