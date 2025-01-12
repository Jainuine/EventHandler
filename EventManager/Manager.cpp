#include "Manager.h"

void Manager::Event::sourceSetter(std::weak_ptr<Producer> a_sourceptr)
{
    if(m_source.expired())
    m_source = a_sourceptr;

    else
    throw  CustomException::InvalidAssignment("Assigning Producer ptr to an existing value") ;
}

std::weak_ptr<Manager::Producer> Manager::Event::sourceGetter()
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

Manager::Producer::Producer()
{
    // Default Producer for Producer Class

}

Manager::Producer::~Producer()
{
    m_isClosed.store(true);
}

void Manager::Producer::triggerEvent(Event &a_event, bool a_monitorCheckFlag, bool a_thresholdCheckFlag, bool a_timerExpiredCheckFlag)
{
    std::lock_guard<std::mutex> lock(mtx);

    a_event.sourceSetter(shared_from_this());
    m_monitorCheckSource = a_monitorCheckFlag;
    m_thresholdCheckSource = a_thresholdCheckFlag;
    m_timerExpiredCheckSource = a_timerExpiredCheckFlag;
    eventQueue.push(a_event);

}

void Manager::Producer::insertQueue(Event &a_event)
{
    std::lock_guard<std::mutex> lock(mtx);
    eventQueue.push(a_event);
}


void Manager::Producer::closeSource()
{
    std::lock_guard<std::mutex> lock(mtx);
    if(!m_isClosed.load())
    m_isClosed.store(true);

    else 
    throw CustomException::InvalidAssignment("Event Source is already closed");
}


bool Manager::Producer::hasEvent()
{
    if(eventQueue.empty())
    return true;

    else
    return false;
}

bool Manager::Producer::isClosed()
{
    if(m_isClosed.load())
    return true;

    else 
    return false;
}

Manager::Event Manager::Producer::popQueue()
{

    std::lock_guard<std::mutex> lock(mtx);
    if (hasEvent())
    {
        Event l_eventVariable = eventQueue.front();
        eventQueue.pop();
        return l_eventVariable;
    }

    else 
    throw CustomException::InvalidFetching("Event Queue is empty");

}

Manager::TimerExpiredProducer::TimerExpiredProducer(int a_timerValue)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(a_timerValue));

    Manager::Event* l_timerExpiredEventptr = new Event();
    Producer::triggerEvent(*l_timerExpiredEventptr,false,false,true);
}


Manager::CompositeProducer::CompositeProducer(std::vector<std::shared_ptr<Manager::Producer>> &a_compositeList)
{
    std::thread([&]()
        {
            while (!isClosed())
            {
                for (auto &l_Producer : a_compositeList)
                {
                    if(l_Producer->hasEvent())
                    {
                        Event l_compositeEvent = l_Producer->popQueue();
                        insertQueue(l_compositeEvent);
                    }
                }
            }
        }
    ).detach();
}

Manager::Event Manager::WaitForEvent(std::shared_ptr<Manager::Producer> a_ProducerPtr)
{
    while(!a_ProducerPtr->isClosed())
    {
        if(a_ProducerPtr->hasEvent())
        return a_ProducerPtr->popQueue();
    }

    throw CustomException::InvalidFetching("Producer is already Closed");
}
