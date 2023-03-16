/*************************************************************************
    > File Name: JsonConfig.h
    > Author: hsz
    > Brief:
    > Created Time: Thu 29 Dec 2022 10:36:22 AM CST
 ************************************************************************/

#ifndef __CONFIG_JSON_CONFIG_H__
#define __CONFIG_JSON_CONFIG_H__

#include <json/json.h>

namespace eular {

using JsonValue = Json::Value;
using JsonMembers = Json::Value::Members;       // std::vector<String>
using JsonValueType = Json::ValueType;
using JsonFastWriter = Json::FastWriter;
using JsonStyledWriter = Json::StyledWriter;
using JsonReader = Json::Reader;

} // namespace eular

#endif