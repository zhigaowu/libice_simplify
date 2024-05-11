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

#include "helloI.h"

#include "logger/glog_logger.h"

#include <memory>

int test_async(int argc, char **argv)
{
    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        const Ice::CommunicatorHolder ich(argc, argv);
        const Ice::CommunicatorPtr communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->shutdown();
            });

        Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
        adapter->add(std::make_shared<greeting::HelloI>(), Ice::stringToIdentity("hello"));
        adapter->activate();

        communicator->waitForShutdown();
    }
    catch(const std::exception& e)
    {
        GERROR() << e.what();
        return 1;
    }
    return 0;
}
