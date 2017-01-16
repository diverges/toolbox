#pragma once

namespace AI
{
    template<class entity>
    class State
    {
    public:
        virtual ~State() {}

        // On State Enter
        virtual void Enter(entity& entity) = 0;

        // On Update Tick
        virtual void Execute(entity& entity) = 0;

        // On State Exit
        virtual void Exit(entity& entity) = 0;
    };

    template<class entity>
    class StateMachine
    {
    public:
        StateMachine(entity& owner) :
            m_pOwner(owner),
            m_pCurrentState(nullptr),
            m_pPreviousState(nullptr),
            m_pGlobalState(nullptr)
        {}

        // Initialize
        void SetCurrentState(State<entity>& state)  { m_pCurrentState = state; }
        void SetPreviousState(State<entity>& state) { m_pPreviousState = state; }
        void SetGlobalState(State<entity>& state)   { m_pGlobalState = state; }

        void Update()
        {
            if(m_pGlobalState)  m_pGlobalState->Execute(m_pOwner);
            if(m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
        }

        // Change to a new state, calline exit and enter logic.
        void ChangeState(State<entity>& newState)
        {
            m_pPreviousState = m_pCurrentState;
            m_pCurrentState->Exit(m_pOwner);
            m_pCurrentState = newState;
            m_pCurrentState->Enter(m_pOwner);
        }

        void RevertToPreviousState()
        {
            ChangeState(m_pPreviousState);
        }

        // Accessors
        State<entity>* CurrentState()   const { return m_pCurrentState; }
        State<entity>* PreviousState()  const { return m_pPreviousState; }
        State<entity>* GlobalState()    const { return m_pGlobalState; }

    private:
        entity& m_pOwner;   // reference to owner

        State<entity>*   m_pCurrentState;
        State<entity>*   m_pPreviousState;  // last state agent was in
        State<entity>*   m_pGlobalState;    // state logic called every update
    };
}