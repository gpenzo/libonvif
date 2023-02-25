#include <iostream>
#include "onvifboss.h"

namespace onvif
{

Manager::Manager()
{

}

Manager::~Manager()
{

}

void Manager::startDiscover(std::vector<Data>* devices, std::function<void()> discoverFinished, 
                            std::function<bool(Data&)>getCredential)
{
    std::thread thread(discover, devices, discoverFinished, getCredential);
    thread.detach();
}

void Manager::discover(std::vector<Data>* devices, std::function<void()> discoverFinished,
                        std::function<bool(Data&)>getCredential)
{
    Session session;
    int number_of_devices = broadcast(session);

    for (int i = 0; i < number_of_devices; i++) {
        Data data;
        if (prepareOnvifData(i, session, data)) {
            while (true) {
                if (getCredential(data)) {
                    if (fillRTSP(data) == 0) {
                        getProfile(data);
                        getDeviceInformation(data);
                        devices->push_back(data);
                        std::cout << "log in success" << std::endl;
                        break;
                    }
                } 
                else {
                    break;
                }
            }
        }
    }

    discoverFinished();
}
    
}
