
#include <helloI.h>

#include "logger/glog_logger.h"

::std::string
greeting::HelloI::SayHello(::std::string name,
                           const Ice::Current& current)
{
    GDEBUG() << "receive greeting from: " << name;
    return ::std::string("hello ") + name;
}
