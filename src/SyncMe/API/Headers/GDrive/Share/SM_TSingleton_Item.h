#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>

template <typename T>
class SM_TSingleton_Item
{
public:
    static T* inst(void);
    static T* inst(QWidget *parent);
    static void freeInst(void);

private:
    SM_TSingleton_Item(){}
    ~SM_TSingleton_Item(){}
    Q_DISABLE_COPY(SM_TSingleton_Item)

private:
    static T* instObject;
};

template <typename T>
T* SM_TSingleton_Item<T>::instObject = NULL;

template <typename T>
T* SM_TSingleton_Item<T>::inst(void)
{
    if(!instObject) instObject = new T;
    return instObject;
}

template <typename T>
T* SM_TSingleton_Item<T>::inst(QWidget *parent)
{
    if(!instObject) instObject = new T(parent);
    return instObject;
}

template <typename T>
void SM_TSingleton_Item<T>::freeInst(void)
{
    if(instObject) delete instObject;
    instObject = NULL;
}

#endif // SINGLETON_H
