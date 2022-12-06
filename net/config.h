#ifndef __CONFIG_
#define __CONFIG_

#include "log.h"
#include "utils.h"
#include <string>
#include <sstream>
#include <memory>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>

//TODO 配置的事件机制，当配置项发生修改时，要通知响应的代码，函数回调

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
  //virtual std::string getTypename() const = 0;

  const std::string& getName() const { return m_name; }
  const std::string& getDes() const { return m_description; }

  virtual std::string getTypeName() const = 0;
private:
  std::string m_name;
  std::string m_description;
};

//对类型转换做一个统一的处理，使用偏特化对复杂类型（容器）提供类型转换支持
template<class F, class T>
class LexicalCast {
public:
  T operator() (const F& from) {
    return boost::lexical_cast<T>(from);
  }
};

//vector to string
template<class T>
class LexicalCast<std::vector<T>, std::string> {
public:
  std::string operator() (const std::vector<T>& vec) {
    std::stringstream ss;
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : vec) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    ss << node;
    return ss.str();
  }
};

//string to vector
template<class T>
class LexicalCast<std::string, std::vector<T>> {
public:
  std::vector<T> operator() (const std::string& str) {
    YAML::Node node = YAML::Load(str);
    typename std::vector<T> vec;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      vec.push_back(LexicalCast<std::string, T>()(ss.str()));
    }
    return vec;
  }
};

//list to string
template<class T>
class LexicalCast<std::list<T>, std::string> {
public:
  std::string operator() (const std::list<T>& vec) {
    std::stringstream ss;
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : vec) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    ss << node;
    return ss.str();
  }
};

//string to list
template<class T>
class LexicalCast<std::string, std::list<T>> {
public:
  std::list<T> operator() (const std::string& str) {
    YAML::Node node = YAML::Load(str);
    typename std::list<T> vec;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      vec.push_back(LexicalCast<std::string, T>()(ss.str()));
    }
    return vec;
  }
};

//set to string
template<class T>
class LexicalCast<std::set<T>, std::string> {
public:
  std::string operator() (const std::set<T>& vec) {
    std::stringstream ss;
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : vec) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    ss << node;
    return ss.str();
  }
};

//string to set
template<class T>
class LexicalCast<std::string, std::set<T>> {
public:
  std::set<T> operator() (const std::string& str) {
    YAML::Node node = YAML::Load(str);
    typename std::set<T> vec;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      vec.insert(LexicalCast<std::string, T>()(ss.str()));
    }
    return vec;
  }
};

//unordered_set to string
template<class T>
class LexicalCast<std::unordered_set<T>, std::string> {
public:
  std::string operator() (const std::unordered_set<T>& vec) {
    std::stringstream ss;
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : vec) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    ss << node;
    return ss.str();
  }
};

//string to unordered_set
template<class T>
class LexicalCast<std::string, std::unordered_set<T>> {
public:
  std::unordered_set<T> operator() (const std::string& str) {
    YAML::Node node = YAML::Load(str);
    typename std::unordered_set<T> vec;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      vec.insert(LexicalCast<std::string, T>()(ss.str()));
    }
    return vec;
  }
};

//map to string
template<class T>
class LexicalCast<std::map<std::string, T>, std::string> {
public:
  std::string operator() (const std::map<std::string, T>& v) {
     YAML::Node node(YAML::NodeType::Map);
      for(auto& i : v) {
          node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
      }
      std::stringstream ss;
      ss << node;
      return ss.str();
  }
};

//string to map
template<class T>
class LexicalCast<std::string, std::map<std::string, T>> {
public:
  std::map<std::string, T> operator() (const std::string& str) {
    YAML::Node node = YAML::Load(str);
    typename std::map<std::string, T> vec;
    std::stringstream ss;
    for (auto i = node.begin(); i != node.end(); ++i) {
      ss.str("");
      ss << i->second;
      vec.insert(std::make_pair(i->first.Scalar(), 
                  LexicalCast<std::string, T>()(ss.str())));
    }
    return vec;
  }
};

//unordered_map to string
template<class T>
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
  std::string operator() (const std::unordered_map<std::string, T>& v) {
     YAML::Node node(YAML::NodeType::Map);
      for(auto& i : v) {
          node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
      }
      std::stringstream ss;
      ss << node;
      return ss.str();
  }
};

//string to unordered_map
template<class T>
class LexicalCast<std::string, std::unordered_map<std::string, T>> {
public:
  std::unordered_map<std::string, T> operator() (const std::string& str) {
    YAML::Node node = YAML::Load(str);
    typename std::unordered_map<std::string, T> vec;
    std::stringstream ss;
    for (auto i = node.begin(); i != node.end(); ++i) {
      ss.str("");
      ss << i->second;
      vec.insert(std::make_pair(i->first.Scalar(), 
                  LexicalCast<std::string, T>()(ss.str())));
    }
    return vec;
  }
};

//TODO 增加用户自定义类型的解析与转换
template<class T, class FromStr = LexicalCast<std::string, T>
                , class ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:

  typedef std::shared_ptr<ConfigVar> ptr;
  typedef std::function<void (const T& old_conf,const T& new_conf)> on_change_cb; 

  ConfigVar(const T& val, const std::string& name, const std::string& des = "")
      :ConfigVarBase(name, des)
      ,m_val(val) {
  }

  std::string getTypeName() const override { return TypeToName<T>(); }

  //使用boost库的格式转换函数lexical将T类型的值转换为字符串类型
  std::string toString() override {
    try {
      //return boost::lexical_cast<std::string>(m_val); 
      return ToStr()(m_val);
    }
    catch (const std::exception& e) {
      SYS_LOG_ERROR(SYS_LOG_ROOT()) << "ConfigVar::toString exception"
        << e.what() << "convert" << typeid(m_val).name() << "to stirng";
    }    
    return "";
  }
  //TODO
  bool fromString(const std::string& val) override {
    try {
      //m_val = LexicalCast<std::string, T>()(val);
      setValue(FromStr()(val));
    }
    catch (const std::exception& e) {
      SYS_LOG_ERROR(SYS_LOG_ROOT()) << "ConfigVar::toString exception"
        << e.what() << "convert string to" << typeid(m_val).name();
    }
    return false;
  } 

  const T getValue() { return m_val; }

  void setValue(const T& val) { 
    if(val== m_val) {
      return;
    }
    for(auto& i : m_cbs) {
      i.second(m_val, val);
    }
    m_val = val; 
  } 
  //TODO 返回类型
  
  //m_cbs
  uint64_t addListener(on_change_cb cb) {
    static uint64_t s_func_id = 0;
    ++s_func_id;
    m_cbs[s_func_id] = cb;
    return s_func_id;
  }

  on_change_cb getListener(uint64_t key) {
    auto it = m_cbs.find(key);
    return it == m_cbs.end() ? nullptr : it->second;
  }

  void delListener(uint64_t key) {
    m_cbs.erase(key);
  }

  void clearListener() {
    m_cbs.clear();
  }
private:
  T m_val;
  //为了保证每个回调函数唯一，使用uint64_t几种进行管理
  std::map<uint64_t, on_change_cb> m_cbs;
};

//config var 的管理类
class Config {
public:
  typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
  
  //根据三元素查找并返回一个对应的configvar ptr 如果map中没有就会自动创建一个
  template<class T>
  static typename ConfigVar<T>::ptr lookUp(const T& default_val, const std::string& name
    , const std::string& des="") {
      auto it = GetDatas().find(name);
      if (it != GetDatas().end()) {
        auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
        if (tmp) {
          SYS_LOG_INFO(SYS_LOG_ROOT()) << "lookup name= " << name << "exists";
          return tmp;
        } else {
          SYS_LOG_INFO(SYS_LOG_ROOT()) << "lookup name= " << name << "exists but type not"
              << TypeToName<T>() << " real_type=" << it->second->getTypeName()
              << " " << it->second->toString();
          return nullptr;
        }
      }
      
      if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678")
          != std::string::npos) {
        SYS_LOG_ERROR(SYS_LOG_ROOT()) << "Invalid config:" << name;
        throw std::invalid_argument(name);
      }

      //没有找到 返回一个新的对象
      typename ConfigVar<T>::ptr v(new ConfigVar<T>(default_val, name, des));
      GetDatas()[name] = v;
      return v;
  }

  //根据名字在map中找到对应的config
  template<class T>
  static typename ConfigVar<T>::ptr lookUp(const std::string& name) {
    auto tmp = GetDatas().find(name);
    if (tmp == GetDatas().end()) {
      return nullptr;
    }
    return std::dynamic_pointer_cast<ConfigVar<T>>(tmp->second);
  }

  static ConfigVarBase::ptr lookUpBase(const std::string& name); 
    
  static void loadFromYaml(const YAML::Node& root);
private:
  static ConfigVarMap& GetDatas() {
    static ConfigVarMap s_datas;
    return s_datas;
  }
};
}

#endif // !__CONFIG_

