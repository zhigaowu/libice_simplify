#ifndef __helloI_h__
#define __helloI_h__

#include <hello.h>

namespace greeting
{

class HelloI : public virtual Hello
{
public:

    virtual ::std::string SayHello(::std::string,
                                   const Ice::Current&) override;
};

}

#endif
