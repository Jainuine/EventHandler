/*
    Author -  Archit Raj Jain
    Feature - Manager.h class containing all the declaration required for maintaining Events and its sources

*/

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include "../Exception/EventException.h"

namespace Manager {

    class EventSource;

    class Event 
    {
        public:
            void sourceSetter(std::weak_ptr<EventSource> a_sourceptr);


        private:
            std::weak_ptr<EventSource> m_source;
    };

    class EventSource : std::enable_shared_from_this<EventSource>
    {
        public:
            void triggerEvent(Event &a_event , bool a_monitorCheckFlag,  bool a_thresholdCheckFlag , bool a_timerExpiredCheckFlag);
            void insertQueue(Event &a_event);
            bool hasEvent();

        private:
            std::atomic<bool> m_monitorCheckSource{false};
            std::atomic<bool> m_thresholdCheckSource{false};
            std::atomic<bool> m_timerExpiredCheckSource{false};
    };




}