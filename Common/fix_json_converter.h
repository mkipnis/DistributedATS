// Copyright (C) Mike Kipnis - DistributedATS
#pragma once

#include <quickfix/Message.h>
#include <quickfix/FieldMap.h>
#include <boost/json.hpp>
#include <string>
#include <sstream>

namespace distributed_ats {
namespace fix_json {

// Convert a FieldMap (Header, Body, or Trailer) to JSON object (recursive)
inline boost::json::object field_map_to_json(const FIX::FieldMap& map)
{
    boost::json::object json_obj;

    // --- Fields ---
    for (FIX::FieldMap::iterator it = const_cast<FIX::FieldMap&>(map).begin();
         it != const_cast<FIX::FieldMap&>(map).end(); ++it)
    {
        const FIX::FieldBase& field = *it;
        int tag = field.getTag();
        json_obj[std::to_string(tag)] = field.getString();
    }

    // --- Groups (repeating groups) ---
    const FIX::FieldMap::Groups& group_maps = map.groups();
    for (auto group_it = group_maps.begin(); group_it != group_maps.end(); ++group_it)
    {
        int group_tag = group_it->first;
        const std::vector<FIX::FieldMap*>& group_vector = group_it->second;

        boost::json::array json_group_array;
        for (auto* group_map : group_vector)
        {
            json_group_array.push_back(field_map_to_json(*group_map));
        }

        json_obj[std::to_string(group_tag)] = std::move(json_group_array);
    }

    return json_obj;
}

// Convert entire FIX message to JSON
inline boost::json::object fix_to_json(const FIX::Message& message)
{
    boost::json::object json_root;

    json_root["Header"]  = field_map_to_json(message.getHeader());
    json_root["Body"]    = field_map_to_json(message);
    json_root["Trailer"] = field_map_to_json(message.getTrailer());

    return json_root;
}

// Helper: convert JSON object to FieldMap (recursive)
inline void json_to_field_map(const boost::json::object& json_obj, FIX::FieldMap& map)
{
    for (auto& kv : json_obj)
    {
        const std::string& key = kv.key();
        const boost::json::value& value = kv.value();
        int tag = std::stoi(key);

        if (value.is_string())
        {
            map.setField(FIX::StringField(tag, value.as_string().c_str()));
        }
        else if (value.is_array())
        {
            const boost::json::array& arr = value.as_array();
            for (auto& item : arr)
            {
                if (!item.is_object()) continue;
                FIX::Group group(tag, 0); // 0 used as placeholder delimiter
                json_to_field_map(item.as_object(), group);
                map.addGroup(tag, group); // FIX: addGroup(tag, group)
            }
        }
        else
        {
            // Convert other JSON types to string
            std::ostringstream oss;
            oss << value;
            map.setField(FIX::StringField(tag, oss.str()));
        }
    }
}

// Convert JSON to FIX::Message
inline FIX::Message json_to_fix(const boost::json::object& json_msg)
{
    FIX::Message message;

    if (json_msg.if_contains("Header"))
        json_to_field_map(json_msg.at("Header").as_object(), message.getHeader());

    if (json_msg.if_contains("Body"))
        json_to_field_map(json_msg.at("Body").as_object(), message);

    if (json_msg.if_contains("Trailer"))
        json_to_field_map(json_msg.at("Trailer").as_object(), message.getTrailer());

    return message;
}

// Helper: JSON string → FIX::Message
inline FIX::Message json_to_fix(const std::string& json_str)
{
    boost::json::value parsed = boost::json::parse(json_str);
    if (!parsed.is_object())
        throw std::runtime_error("Invalid JSON format for FIX message");

    return json_to_fix(parsed.as_object());
}

// Helper: FIX::Message → JSON string
inline std::string fix_to_json_string(const FIX::Message& message)
{
    boost::json::object json_obj = fix_to_json(message);
    return boost::json::serialize(json_obj);
}

} // namespace fix_json
} // namespace distributed_ats
