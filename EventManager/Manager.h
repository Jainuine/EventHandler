/*
    Author -  Archit Raj Jain
    Feature - Manager.h class containing all the declaration required for maintaining Events and its sources

*/

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include "../Exception/EventException.h"

namespace Manager {

    class EventSource;

    class Event {
        public:
            void sourceSetter(std::weak_ptr<EventSource> a_sourceptr);


        private:
            std::weak_ptr<EventSource> m_source;
    };







}