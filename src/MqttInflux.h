#ifndef MQTTINFLUX_H
#define MQTTINFLUX_H

#include <string>
#include <vector>
#include <map>
#include "MqttDaemon.h"
#include "MqttBridge.h"

class MqttInflux : public MqttDaemon, public IForwardMessage
{
    public:
        MqttInflux();
        ~MqttInflux();

		int DaemonLoop(int argc, char* argv[]);
        void on_message(const std::string& topic, const std::string& message);
        void on_forward(const std::string& identifier, const std::string& topic, const std::string& message);

    private:
        bool m_bPause;
        std::string m_InfluxServer;
        std::string m_InfluxDb;
        std::vector<MqttBridge*> m_MqttClients;
		void DaemonConfigure(SimpleIni& iniFile);
		bool IsDecimal(const std::string& s);
};
#endif // MQTTINFLUX_H
