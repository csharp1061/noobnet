#include "../net/log.h"
#include "../net/config.h"
#include <yaml-cpp/yaml.h>


noobnet::ConfigVar<int>::ptr g_int_value_config = 
  noobnet::Config::lookUp((int)8080, "system.port", "port");

void test_yaml() {
  YAML::Node root = YAML::LoadFile("/home/hzycpp/Desktop/MyTemplate/noobnet/yaml/logs.yml");
  SYS_LOG_INFO(SYS_LOG_ROOT()) << root;
}

void test_loadyaml(const YAML::Node& root) { 
  noobnet::Config::loadFromYaml(root);
}

int main(int argc, const char** argv) {
    SYS_LOG_INFO(SYS_LOG_ROOT()) << "before" << g_int_value_config->getValue();
    SYS_LOG_INFO(SYS_LOG_ROOT()) << "before" <<g_int_value_config->toString();

    YAML::Node root = YAML::LoadFile("/home/hzycpp/Desktop/MyTemplate/noobnet/yaml/logs.yml");
    noobnet::Config::loadFromYaml(root);
    
    SYS_LOG_INFO(SYS_LOG_ROOT()) << "after" << g_int_value_config->getValue();
    SYS_LOG_INFO(SYS_LOG_ROOT()) << "after" << g_int_value_config->toString();

    SYS_LOG_INFO(SYS_LOG_ROOT()) << root;
    //test_yaml();
    return 0;
}