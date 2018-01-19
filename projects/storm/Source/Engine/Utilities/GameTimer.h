#pragma once
#include <stdint.h>
#include <wrl.h>

namespace DX
{
    class GameTimer
    {
    public:
        GameTimer() :
            m_baseTime(0),
            m_pauseTime(0),
            m_stopTime(0),
            m_prevTime(0),
            m_currTime(0),
            m_isStopped(false)
        {
            QueryPerformanceFrequency(&m_qpcFrequency);
            m_secondsPerCount = 1.0 / (double)m_qpcFrequency.QuadPart;
        }

        
        double GetDeltaTime() const         { return m_deltaTime; }
        double GetTotalTime() const
        {
            if (m_isStopped)
            {
                return (
                    ((m_stopTime - m_pauseTime) - m_baseTime) * m_secondsPerCount
                );
            }
            else
            {
                return (
                    (((m_currTime - m_pauseTime) - m_baseTime) * m_secondsPerCount)
                );
            }

        }

        void Tick() 
        {
            if (m_isStopped)
            {
                m_deltaTime = 0.0;
                return;
            }

            // Get the current time
            LARGE_INTEGER currTime;
            QueryPerformanceCounter(&currTime);
            m_currTime = currTime.QuadPart;

            // update delta time
            m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;

            // Update delta time
            m_prevTime = m_currTime;

            // negative check, incase of processor state change
            if (m_deltaTime < 0.0)
            {
                m_deltaTime = 0.0;
            }
        };

        void Reset()
        {
            LARGE_INTEGER currTime;
            QueryPerformanceCounter(&currTime);

            m_baseTime = currTime.QuadPart;
            m_prevTime = currTime.QuadPart;
            m_stopTime = 0;
            m_isStopped = false;
        }

        void Stop()
        {
            if (!m_isStopped)
            {
                LARGE_INTEGER currTime;
                QueryPerformanceCounter(&currTime);

                m_stopTime = currTime.QuadPart;
                m_isStopped = true;

            }
        };
        
        void Start()
        {
            LARGE_INTEGER startTime;
            QueryPerformanceCounter(&startTime);

            // If resuming from a stopped state...
            if (m_isStopped)
            {
                // Count pause time
                m_pauseTime += (startTime.QuadPart - m_stopTime); 

                m_prevTime = startTime.QuadPart;
                m_isStopped = false;
                m_stopTime = 0;
            }
        };


    private:
        // Source timing data uses QPC units.
        LARGE_INTEGER m_qpcFrequency;

        double m_secondsPerCount;
        double m_deltaTime;

        uint64_t m_baseTime;
        uint64_t m_pauseTime;
        uint64_t m_stopTime;
        uint64_t m_prevTime;
        uint64_t m_currTime;

        bool     m_isStopped; 

    };
}