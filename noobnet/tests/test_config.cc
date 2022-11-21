#include "../net/log.h"
#include "../net/config.h"
#include <yaml-cpp/yaml.h>


noobnet::ConfigVar<int>::ptr g_int_value_config = 
  noobnet::Config::lookUp((int)8080, "system.port", "port");

noobnet::ConfigVar<std::vector<int>>::ptr g_vec_int_config = 
  noobnet::Config::lookUp((std::vector<int>){1,2}, "vec", "test vec transform");

noobnet::ConfigVar<std::list<int>>::ptr g_list_int_config = 
  noobnet::Config::lookUp((std::list<int>){2,1}, "list", "list vec transform");

noobnet::ConfigVar<std::set<int>>::ptr g_set_int_config = 
  noobnet::Config::lookUp((std::set<int>){2,1,1,1}, "set", "set vec transform");

noobnet::ConfigVar<std::unordered_set<int>>::ptr g_unordered_set_int_config = 
  noobnet::Config::lookUp((std::unordered_set<int>){2,1,1,1}, "set", "set vec transform");
  
noobnet::ConfigVar<std::map<std::string, int>>::ptr g_map_int_config = 
  noobnet::Config::lookUp((std::map<std::string, int>){{"k", 2}}, "map", "map vec transform");

noobnet::ConfigVar<std::unordered_map<std::string, int>>::ptr g_unordered_map_int_config = 
  noobnet::Config::lookUp((std::unordered_map<std::string, int>){{"k", 2}}, "unordered_map", "unordered_map vec transform");

void test_yaml() {
  YAML::Node root = YAML::LoadFile("/home/hzycpp/Desktop/MyTemplate/noobnet/yaml/logs.yml");
  SYS_LOG_INFO(SYS_LOG_ROOT()) << root;
}

void test_loadyaml(const YAML::Node& root) { 
  noobnet::Config::loadFromYaml(root);
}

#define XX(g_var, name, prefix) \
  { \
    auto &v = g_var->getValue(); \
    for (auto &i : v) { \
      SYS_LOG_INFO(SYS_LOG_ROOT()) << #prefix " " #name ": " << i; \
    } \
    SYS_LOG_INFO(SYS_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
  }

#define XX_M(g_var, name, prefix) \
  { \
    auto &v = g_var->getValue(); \
    for (auto &i : v) { \
      SYS_LOG_INFO(SYS_LOG_ROOT()) << #prefix " " #name ": " \
      << i.first << "--" << i.second; \
    } \
    SYS_LOG_INFO(SYS_LOG_ROOT()) << #prefix " " #name "yaml: " << g_var->toString(); \
  }

int main(int argc, const char** argv) {
    SYS_LOG_INFO(SYS_LOG_ROOT()) << "before" << g_int_value_config->getValue();
    SYS_LOG_INFO(SYS_LOG_ROOT()) << "before" <<g_int_value_config->toString();

    YAML::Node root = YAML::LoadFile("/home/hzycpp/Desktop/MyTemplate/noobnet/yaml/logs.yml");
    noobnet::Config::loadFromYaml(root);
    
    //XX(g_int_value_config, "int", "int test");
    XX(g_vec_int_config, "vec", "vec test")
    XX(g_list_int_config, "list", "list test")
    XX(g_set_int_config, "set", "set test")
    XX(g_unordered_set_int_config, "unordered_set", "unordered_set test")
    XX_M(g_map_int_config, "map", "map test")
    XX_M(g_unordered_map_int_config, "un map", 'un map test')

    SYS_LOG_INFO(SYS_LOG_ROOT()) << "after" << g_int_value_config->getValue();
    SYS_LOG_INFO(SYS_LOG_ROOT()) << "after" << g_int_value_config->toString();

    //SYS_LOG_INFO(SYS_LOG_ROOT()) << root;
    //test_yaml();
    return 0;
}