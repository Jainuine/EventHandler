#include "Manager.h"

void Manager::Event::sourceSetter(std::weak_ptr<EventSource> a_sourceptr)
{
    if(m_source.expired())
    m_source = a_sourceptr;

    else
    throw  CustomException::InvalidAssignment("Assigning EventSource ptr to an existing value") ;
}

std::weak_ptr<Manager::EventSource> Manager::Event::sourceGetter()
{
    if(!m_source.expired())
    return m_source;

    else
    throw CustomException::InvalidFetching("Event source has been not initialised");

}

Manager::Event::Event() 
{
    // Default Constructor for Event Class
    m_source.reset();
}

Manager::Event::~Event()
{
    m_source.~weak_ptr();
}

Manager::EventSource::EventSource()
{
    // Default EventSource for EventSource Class

}

Manager::EventSource::~EventSource()
{
    m_isClosed.store(true);
}

void Manager::EventSource::triggerEvent(Event &a_event, bool a_monitorCheckFlag, bool a_thresholdCheckFlag, bool a_timerExpiredCheckFlag)
{
    a_event.sourceSetter(shared_from_this());
    m_monitorCheckSource = a_monitorCheckFlag;
    m_thresholdCheckSource = a_thresholdCheckFlag;
    m_timerExpiredCheckSource = a_timerExpiredCheckFlag;
    eventQueue.push(a_event);

}

void Manager::EventSource::insertQueue(Event &a_event)
{
    eventQueue.push(a_event);
}


void Manager::EventSource::closeSource()
{
    if(!m_isClosed.load())
    m_isClosed.store(true);

    else 
    throw CustomException::InvalidAssignment("Event Source is already closed");
}


bool Manager::EventSource::hasEvent()
{
    if(eventQueue.empty())
    return true;

    else
    return false;
}

bool Manager::EventSource::isClosed()
{
    if(m_isClosed.load())
    return true;

    else 
    return false;
}

Manager::Event Manager::EventSource::popQueue()
{
    if (hasEvent())
    {
        Event l_eventVariable = eventQueue.front();
        eventQueue.pop();
        return l_eventVariable;
    }

    else 
    throw CustomException::InvalidFetching("Event Queue is empty");

}

Manager::TimerExpiredEventSource::TimerExpiredEventSource(int a_timerValue)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(a_timerValue));

    Manager::Event* l_timerExpiredEventptr = new Event();
    EventSource::triggerEvent(*l_timerExpiredEventptr,false,false,true);
}


Manager::CompositeEventSource::CompositeEventSource()
{
}
