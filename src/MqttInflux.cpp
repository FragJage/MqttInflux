#include <iostream>
#include <algorithm>
#include <cpr/cpr.h>
#ifdef WIN32
#include <WinSock2.h>		// To stop windows.h including winsock.h
#endif
#include "MqttInflux.h"


using namespace std;

MqttInflux::MqttInflux() : MqttDaemon("influx", "MqttInflux")
{
}

MqttInflux::~MqttInflux()
{
    vector<MqttBridge*>::iterator it;

    for(it=m_MqttClients.begin(); it!=m_MqttClients.end(); ++it)
    {
        delete(*it);
    }
    m_MqttClients.clear();
}

void MqttInflux::DaemonConfigure(SimpleIni& iniFile)
{
    LOG_ENTER;

	for (SimpleIni::SectionIterator itSection = iniFile.beginSection(); itSection != iniFile.endSection(); ++itSection)
	{
		if ((*itSection) == "mqtt") continue;
		if ((*itSection) == "log") continue;
		if ((*itSection) == "influx")
        {
            m_InfluxServer = iniFile.GetValue("influx", "server", "http://127.0.0.1:8083");
            m_InfluxDb = iniFile.GetValue("influx", "database", "");
            continue;
        }

        string name    = (*itSection);
        string server  = iniFile.GetValue(name, "server", "");
        string topic   = iniFile.GetValue(name, "topic", "");

        if((server=="")||(topic==""))
        {
            LOG_WARNING(m_Log) << "Keywords server and topic are mandatory in section " << name;
            continue;
        }

        LOG_VERBOSE(m_Log) << "Rule " << name << " : Subscript to " << server << " topic " << topic;
        MqttBridge* pMqttBridge = new MqttBridge(name, server, topic, this);
        m_MqttClients.emplace_back(pMqttBridge);
    }

	LOG_EXIT_OK;
}

bool MqttInflux::IsDecimal(const string& s)
{
    return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !((isdigit(c))||(c=='.')); }) == s.end();
}

void MqttInflux::on_forward(const string& identifier, const string& topic, const string& message)
{
    if(m_bPause) return;

    size_t pos;
    string name(topic);
    LOG_VERBOSE(m_Log) << "Mqtt receive for rule " << identifier << " : " << topic << " => " << message;
    pos = name.find_last_of('/');
    if(pos != string::npos) name = name.substr(pos+1);

    //TO DO a task
    string value = message;
    if(!IsDecimal(value))
       value = "\""+value+"\"";
    LOG_VERBOSE(m_Log) << "Send to Influx : " << m_InfluxServer << "/write?db=" << m_InfluxDb << " => " << name << " value=" << value;
    auto r = cpr::Post(cpr::Url{m_InfluxServer+"/write?db="+m_InfluxDb}, cpr::Body{name+" value="+value});
    if((r.status_code < 200)||(r.status_code > 299))
        LOG_WARNING(m_Log) << "Influx error : Code " << r.status_code << ", message " << r.text;
}

void MqttInflux::on_message(const string& topic, const string& message)
{
	LOG_VERBOSE(m_Log) << "Mqtt receive " << topic << " : " << message;

	string mainTopic = GetMainTopic();
	if (topic.substr(0, mainTopic.length()) != mainTopic)
	{
		LOG_WARNING(m_Log) << "Receive topic not for me (" << mainTopic << ")";
		return;
	}

	if ( (topic.substr(mainTopic.length(), 7) != "command") ||(topic.length() != mainTopic.length() + 7) )
	{
		LOG_WARNING(m_Log) << "Receive topic but not a command (waiting " << mainTopic + "command" << ")";
		return;
	}

	//TO DO service administration

	return;
}

int MqttInflux::DaemonLoop(int argc, char* argv[])
{
	LOG_ENTER;

	Subscribe(GetMainTopic() + "command/#");
	LOG_VERBOSE(m_Log) << "Subscript to : " << GetMainTopic() + "command/#";

	bool bStop = false;
	m_bPause = false;
	while(!bStop)
    {
		int cond = Service::Get()->Wait();
		if (cond == Service::STATUS_CHANGED)
		{
			switch (Service::Get()->GetStatus())
			{
                case Service::StatusKind::PAUSE:
                    m_bPause = true;
                    break;
                case Service::StatusKind::START:
                    m_bPause = false;
                    break;
                case Service::StatusKind::STOP:
                    bStop = true;
                    break;
			}
		}
    }

	LOG_EXIT_OK;
    return 0;
}
