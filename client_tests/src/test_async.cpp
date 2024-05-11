/*
 * ==========================================================================
 *
 *       Filename:  test_cases.cpp
 *
 *    Description:  test cases
 *
 *        Version:  1.0
 *        Created:  2024-02-27 16:22:54
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:
 * ==========================================================================
 */

#include <Ice/Ice.h>

#include "hello.h"

#include "logger/glog_logger.h"

#include <memory>
#include <thread>

int test_async(int argc, char **argv)
{
    try
    {
        const Ice::CommunicatorHolder ich(argc, argv);
        auto hello = Ice::checkedCast<greeting::HelloPrx>(ich->stringToProxy("hello:default -h localhost -p 10000"));

        GINFO() << hello->SayHello("server");
    }
    catch(const std::exception& ex)
    {
        GINFO() << ex.what();
        return 1;
    }
    return 0;
}
