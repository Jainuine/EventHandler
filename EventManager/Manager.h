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
#include <mutex>
#include "../Exception/EventException.h"

namespace Manager {

    class Producer;

    class Event 
    {
        public:
            Event ();
            ~Event();
            void sourceSetter(std::weak_ptr<Producer> a_sourceptr);
            std::weak_ptr<Producer> sourceGetter();
            

        private:
            std::weak_ptr<Producer> m_source;
    };

    class Producer : std::enable_shared_from_this<Producer>
    {
        public:
            Producer();
            ~Producer();
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
            std::mutex mtx;
    };

    template <typename T>
    class MonitorChangeProducer : public Producer
    {
        public:
            MonitorChangeProducer(T &a_variableValue)
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
    class ThresholdExceededProducer : public Producer
    {
        public:
            ThresholdExceededProducer(T &a_baseVal , T a_lowerThreshold , T a_upperThreshold)
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

    class TimerExpiredProducer : public Producer
    {
        public:
            TimerExpiredProducer(int a_timerValue);
    };


    class CompositeProducer : public Producer
    {
        public:
            CompositeProducer(std::vector<std::shared_ptr<Manager::Producer>> &a_compositeList);
    };

    Event WaitForEvent(std::shared_ptr<Manager::Producer> a_ProducerPtr);

}