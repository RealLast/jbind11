#pragma once

namespace jbind11
{
    template <typename T>
    class JBindSingleton
    {
    public:


        static T *getInstance()
        {
            if(mpInstance == nullptr)
            {
                mpInstance = new T();
            }
            std::cout << "Instance " << mpInstance;
          
            return mpInstance;
        }

    protected:
        JBindSingleton()
        {
        
        }


    private:
        static T* mpInstance;
    };


}

template <typename T>
T* jbind11::JBindSingleton<T>::mpInstance = nullptr;

