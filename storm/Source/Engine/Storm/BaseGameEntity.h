#pragma once

namespace GameCore
{
    // BaseGameEntity :
    //  Base class of all game entities. Each entity must have a unique
    //  identifier and update method.
    class BaseGameEntity
    {
    public:
        BaseGameEntity(int id) 
        {
            SetID(id);
        }
        virtual ~BaseGameEntity() {}

        virtual void Update()=0;

        int ID() const { return m_ID; }
    private:
        int m_ID;                   // Unique Entity ID
        static int m_iNextValidID;  // Updated every time a new entity is instantiated

        void SetID(int val) { m_ID = val; }; // Sets entity ID
    };
}