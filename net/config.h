#ifndef __NOOBNET_CONFIG_
#define __NOOBNET_CONFIG_

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

/**
 * @brief 配置参数模板类型，并保留其参数值
 * @details T 具体的参数类型
 *          FromStr 从string类型转换至模板参数类型的仿函数
 *          ToStr 从模板参数类型转换至string类型的仿函数
 *          std::string yaml格式的字符串
*/
template<class T, class FromStr = LexicalCast<std::string, T>
                , class ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
  typedef RWMutex RWMutexType;
  typedef std::shared_ptr<ConfigVar> ptr;
  typedef std::function<void (const T& old_conf,const T& new_conf)> on_change_cb; 

  /**
   * @brief 构造函数
  */
  ConfigVar(const T& val, const std::string& name, const std::string& des = "")
      :ConfigVarBase(name, des)
      ,m_val(val) {
  }

  /**
   * @brief 获取配置项类型名称
  */
  std::string getTypeName() const override { return TypeToName<T>(); }

  /**
   * @brief     使用字符串转换后的值更改配置项
   * @details   读取配置项的值，并进行类型转换： 
   *            自定义配置项类型 -> std::string
   * @exception e 类型转换失败会抛出异常并打印
  */
  std::string toString() override {
    try {
      //return boost::lexical_cast<std::string>(m_val); 
      RWMutexType::ReadLock lock(m_mutex);
      return ToStr()(m_val);
    }
    catch (const std::exception& e) {
      SYS_LOG_ERROR(SYS_LOG_ROOT()) << "ConfigVar::toString exception"
        << e.what() << "convert" << typeid(m_val).name() << "to stirng";
    }    
    return "";
  }

  /**
   * @brief     使用字符串转换后的值更改配置项
   * @param[in] val string类型的配置值
   * @details   读取string字符串的值，并进行
   *            类型转换 std::string -> T 更改对应配置项
   * @exception e 类型转换失败会抛出异常并打印
  */
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

  /**
   * @brief 获取参数值
  */
  const T getValue() { 
    RWMutexType::ReadLock lock(m_mutex);
    return m_val; 
  }

  /**
   * @brief 更改对应类型的值
   * @details 如果对应的值出现变化则通知对应的注册回调函数
  */
  void setValue(const T& val) { 
    {
      RWMutexType::ReadLock lock(m_mutex);
      if(val== m_val) {
        return;
      }
      for(auto& i : m_cbs) {
        i.second(m_val, val);
      }
    }
    RWMutexType::WriteLock lock(m_mutex);
    m_val = val; 
  } 
  //TODO 返回类型
  
  /**
   * @brief     添加回调函数
   * @param[in] cb 回调函数callback 
   * @return    将cb添加至回调函数模块之后赋予的序列号
  */
  uint64_t addListener(on_change_cb cb) {
    static uint64_t s_func_id = 0;
    RWMutexType::WriteLock lock(m_mutex);
    ++s_func_id;
    m_cbs[s_func_id] = cb;
    return s_func_id;
  }

  /**
   * @brief     根据序号值获取指定的回调函数
   * @param[in] key 回调函数的唯一的序列号 
   * @return    key序号对应的回调函数
  */
  on_change_cb getListener(uint64_t key) {
    RWMutexType::ReadLock lock(m_mutex);
    auto it = m_cbs.find(key);
    return it == m_cbs.end() ? nullptr : it->second;
  }

  /**
   * @brief     根据序号值删除指定的回调函数
   * @param[in] key 回调函数的唯一的序列号 
  */
  void delListener(uint64_t key) {
    RWMutexType::WriteLock lock(m_mutex);
    m_cbs.erase(key);
  }

  /**
   * @brief 清理所有的回调函数
  */
  void clearListener() {
    RWMutexType::WriteLock lock(m_mutex);
    m_cbs.clear();
  }
private:
  T m_val;
  //为了保证每个回调函数唯一，使用uint64_t集中进行管理
  std::map<uint64_t, on_change_cb> m_cbs;
  RWMutexType m_mutex;
};

//config var 的管理类
class Config {
public:
  typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
  typedef RWMutex RWMutexType;
  
  /**
   * @brief      根据名字查找/创建对应的配置项
   * @param[in]  default_val 配置项的默认值
   * @param[in]  name 配置项的名称
   * @param[in]  des 配置项的描述
   * @details    根据配置项的名称查找相应的配置项
   *             若不存在该配置项则使用默认值default_val创建
   * @return     返回对应的配置参数，若名称存在但格式不匹配则返回nullptr
   * @exception  若存在[abcdefghijklmnopqrstuvwxyz._012345678]
   *             抛出异常invalid_argument
  */
  template<class T>
  static typename ConfigVar<T>::ptr LookUp(const T& default_val, const std::string& name
    , const std::string& des="") {
      RWMutexType::WriteLock lock(GetLock());
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

  /**
   * @brief 根据名字找到对应的配置项
  */
  template<class T>
  static typename ConfigVar<T>::ptr LookUp(const std::string& name) {
    RWMutexType::ReadLock lock(GetLock());
    auto tmp = GetDatas().find(name);
    if (tmp == GetDatas().end()) {
      return nullptr;
    }
    return std::dynamic_pointer_cast<ConfigVar<T>>(tmp->second);
  }

  /**
   * @brief     查找配置参数，返回配置参数的基类
   * @param[in] name 配置参数的名字
  */
  static ConfigVarBase::ptr LookUpBase(const std::string& name); 

  /**
   * @brief 使用YAML::Node初始化配置文件
  */ 
  static void LoadFromYaml(const YAML::Node& root);

  /**
   * @brief     从配置目录path中加载配置项
   * @param[in] path 配置文件所在的目录 
  */
  static void LoadFromConfDir(const std::string& path, bool force);
  
  /**
   * @brief     遍历配置模块内的所有配置项
   * @param[in] cb 配置项的回调函数
  */
  static void Visit(std::function<void(ConfigVarBase::ptr)> cb);
private:
  /**
   * @brief 获得所有的配置项
  */
  static ConfigVarMap& GetDatas() {
    static ConfigVarMap s_datas;
    return s_datas;
  }

  /**
   * @brief 配置项的读写锁
  */
  static RWMutexType& GetLock() {
    static RWMutexType s_mutex;
    return s_mutex;
  }
};
}

#endif // !__NOOBNET_CONFIG_

