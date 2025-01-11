/*
    Author -  Archit Raj Jain
    Feature - Manager.h class containing all the declaration required for maintaining Events and its sources

*/

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <queue>
#include <chrono>
#include "../Exception/EventException.h"

namespace Manager {

    class EventSource;

    class Event 
    {
        public:
            Event ();
            ~Event();
            void sourceSetter(std::weak_ptr<EventSource> a_sourceptr);
            std::weak_ptr<EventSource> sourceGetter();
            

        private:
            std::weak_ptr<EventSource> m_source;
    };

    class EventSource : std::enable_shared_from_this<EventSource>
    {
        public:
            EventSource();
            ~EventSource();
            void triggerEvent(Event &a_event , bool a_monitorCheckFlag,  bool a_thresholdCheckFlag , bool a_timerExpiredCheckFlag);
            void insertQueue(Event &a_event);
            void closeSource();
            bool hasEvent();
            bool isClosed();
            Event popQueue();

        private:
            std::atomic<bool> m_monitorCheckSource{false};
            std::atomic<bool> m_thresholdCheckSource{false};
            std::atomic<bool> m_timerExpiredCheckSource{false};
            std::atomic<bool> m_isClosed{false};
            std::queue<Event> eventQueue;
    };

    template <typename T>
    class MonitorChangeEventSource : public EventSource
    {
        public:
            MonitorChangeEventSource(T &a_variableValue)
            {
                std::thread([&](){
                    T l_lastValue = a_variableValue;
                    while (!isClosed())
                    {
                        if(l_lastValue != a_variableValue)
                        {
                            Event* l_monitorEventPtr;
                            triggerEvent(l_monitorEventPtr,true,false,false);
                        }     
                    }    
                }).detach();
            }

    };

    template <typename T>
    class ThresholdExceededEventSource : public EventSource
    {
        public:
            ThresholdExceededEventSource(T &a_baseVal , T a_lowerThreshold , T a_upperThreshold)
            {
                std::thread([&]()
                    {
                        while(!isClosed())
                        {
                            if( a_lowerThreshold > a_baseVal || a_baseVal > a_upperThreshold)
                            {
                                Event* l_thresholdEventPtr;
                                triggerEvent(l_thresholdEventPtr,false,true,false);
                            }
                        }

                    }
                ).detach();
            }
    };

    class TimerExpiredEventSource : public EventSource
    {
        public:
            TimerExpiredEventSource(int a_timerValue);
    };


    class CompositeEventSource : public EventSource
    {
        public:
            CompositeEventSource();
    };

}