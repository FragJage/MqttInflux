#include <iostream>
#include <exception>
#include "MqttInflux.h"

using namespace std;

int main(int argc, char* argv[])
{
    int res = 0;

    try
    {
        MqttInflux mqttInflux;

        Service* pService = Service::Create("MqttInflux", "Log mqtt messages into InfluxDb", &mqttInflux);
        res = pService->Start(argc, argv);
        Service::Destroy();
    }
    catch(const exception &e)
    {
        std::cout << e.what();
    }

    return res;
}
