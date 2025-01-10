#include "Manager.h"

void Manager::Event::sourceSetter(std::weak_ptr<EventSource> a_sourceptr)
{
    if(m_source.expired())
    m_source = a_sourceptr;

    else
      CustomException::InvalidAssignment("Assigning EventSource ptr to an existing value") ;
};


void Manager::EventSource::triggerEvent(Event &a_event , bool a_monitorCheckFlag,  bool a_thresholdCheckFlag , bool a_timerExpiredCheckFlag)
{


}

bool Manager::EventSource::hasEvent()
{
    return false;
}
