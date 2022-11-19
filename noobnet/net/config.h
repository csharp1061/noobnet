#ifndef __CONFIG_
#define __CONFIG_

#include "log.h"
#include <memory>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>


namespace noobnet
{
  
//基类 维护设置的名字和描述
class ConfigVarBase {
public:
  typedef std::shared_ptr<ConfigVarBase> ptr;

  ConfigVarBase(const std::string& name, const std::string& des = "") :
        m_name(name), //强制转小写
        m_description(des) {
    std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
  }

  virtual ~ConfigVarBase() {};

  virtual std::string toString() = 0;
  virtual bool fromString(const std::string& val) = 0;

  const std::string& getName() const { return m_name; }
  const std::string& getDes() const { return m_description; }
private:
  std::string m_name;
  std::string m_description;
};

//对类型转换做一个统一的处理，使用偏特化对复杂类型（容器）提供类型转换支持
template<class F, class T>
class LexicalCast {
public:
  T operator() (const F& from, const T& target) {
    return boost::lexical_cast<T>(from);
  }
};

//vector to string
template<class T>
class LexicalCast<std::vector<T>, std::string> {
public:
  std::string operator() (const std::vector<T>& vec) {
    std::stringstream ss;
    for (auto i = vec.begin(); i != vec.end(); ++i) {
      
    }
  }
};


template<class T>
class ConfigVar : public ConfigVarBase {
public:
  //使用智能指针管理自己
  typedef std::shared_ptr<ConfigVar> ptr;

  ConfigVar(const T& val, const std::string& name, const std::string& des = "")
      :ConfigVarBase(name, des)
      ,m_val(val) {
  }

  //使用boost库的格式转换函数lexical将T类型的值转换为字符串类型
  std::string toString() override {
    try {
      return boost::lexical_cast<std::string>(m_val); 
    }
    catch (const std::exception& e) {
      SYS_LOG_ERROR(SYS_LOG_ROOT()) << "ConfigVar::toString exception"
        << e.what() << "convert" << typeid(m_val).name() << "to stirng";
    }    
    return "";
  }
  //将字符串类型的值转换为T类型
  bool fromString(const std::string& val) {
    try {
      m_val = boost::lexical_cast<T>(val);
    }
    catch (const std::exception& e) {
      SYS_LOG_ERROR(SYS_LOG_ROOT()) << "ConfigVar::toString exception"
        << e.what() << "convert string to" << typeid(m_val).name();
    }
    return false;
  } 

  const T getValue() { return m_val; }
  void setValue(const T& val) { m_val = val; } 
private:
  T m_val;
};

//config var 的管理类
class Config {
public:
  typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

  //根据三元素查找并返回一个对应的configvar ptr 如果map中没有就会自动创建一个
  template<class T>
  static typename ConfigVar<T>::ptr lookUp(const T& default_val, const std::string& name
    , const std::string& des="") {
      auto tmp = lookUp<T>(name);
      if (tmp) {
        SYS_LOG_INFO(SYS_LOG_ROOT()) << "Lookup name=" << name << "exists";
        return tmp;
      }
      //限定名字首字母 npos是string的最大值，find未找到返回值为最大值
      if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678")
        != std::string::npos) {
        SYS_LOG_ERROR(SYS_LOG_ROOT()) << "Lookup name invalid" << name;
        throw std::invalid_argument(name);
      }
      //没有找到 返回一个新的对象
      typename ConfigVar<T>::ptr v(new ConfigVar<T>(default_val, name, des));
      s_datas[name] = v;
      return v;
  }

  //根据名字在map中找到对应的config
  template<class T>
  static typename ConfigVar<T>::ptr lookUp(const std::string& name) {
    auto tmp = s_datas.find(name);
    if (tmp == s_datas.end()) {
      return nullptr;
    }
    return std::dynamic_pointer_cast<ConfigVar<T>>(tmp->second);
  }

  static ConfigVarBase::ptr lookUpBase(const std::string& name); 
    
  static void loadFromYaml(const YAML::Node& root);
private:
  static ConfigVarMap s_datas;
};
}

#endif // !__CONFIG_

