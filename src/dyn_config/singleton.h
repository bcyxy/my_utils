#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <stddef.h>

/** 单例模式基类
 * 继承此基类的类成为单例模式
 */
template <typename T>
class SingletonBase {
  public:
    static T& getInstance();

  protected:
    SingletonBase() {}
    ~SingletonBase() {}
    static T *m_pInstance;

  private:
    SingletonBase(const SingletonBase&);
    SingletonBase& operator = (const SingletonBase&);
};

template <class T>
T& SingletonBase<T>::getInstance() {
    // 此处未加锁，会有一些风险；
    if (m_pInstance == NULL) {
        m_pInstance = new T();
    }
    return *m_pInstance;
}

template <typename T>
T* SingletonBase<T>::m_pInstance = NULL;

#endif