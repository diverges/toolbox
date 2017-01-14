#pragma once
//===============================================
// Templates.h : class templates
//===============================================

namespace Storm
{
    template <class T>
    class singleton
    {
        T m_OldValue;
        T* m_pGlobalValue;

    public:
        singleton(T newValue, T* globalValue)
        {
            m_pGlobalValue = globalValue;
            m_OldValue = *globalValue;
            *m_pGlobalValue = newValue;
        }

        virtual ~singleton() { *m_pGlobalValue = m_OldValue; }
    };
}
