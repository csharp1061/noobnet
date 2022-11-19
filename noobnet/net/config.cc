#include "log.h"
#include "config.h"

namespace noobnet {

Config::ConfigVarMap Config::s_datas;

ConfigVarBase::ptr Config::lookUpBase(const std::string& name) {
  auto it = s_datas.find(name);
  return it == s_datas.end() ? nullptr : it->second;
}
/*
yaml文件格式
"A.B", 10
A:
  B: 10
*/
static void ListAllMember(const std::string& prefix,
                          const YAML::Node& node,
                          std::list<std::pair<std::string, const YAML::Node>>& output) {
  if (prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678")
  != std::string::npos) {
    SYS_LOG_ERROR(SYS_LOG_ROOT()) << "Invalid config:" << prefix << ":" << node;
    return;
  }
  output.push_back(std::make_pair(prefix, node));
  if (node.IsMap()) {
    for (auto i = node.begin(); i != node.end(); ++i) {
      ListAllMember(prefix.empty() ? i->first.Scalar() : 
      prefix + "." + i->first.Scalar(), i->second, output);
    }
  }
}
//
void Config::loadFromYaml(const YAML::Node& root) {
  std::list<std::pair<std::string, const YAML::Node>> all_nodes;
  ListAllMember("", root, all_nodes);

  for (auto &it : all_nodes) {
    std::string key = it.first;
    if (key.empty()) {
      continue;
    }

    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    ConfigVarBase::ptr var = lookUpBase(key);

    if (var) {
      if (it.second.IsScalar()) {
        var->fromString(it.second.Scalar());
      } else {
        std::stringstream ss;
        ss << it.second;
        var->fromString(ss.str());
      }
    }
  }
}

} //noobnet