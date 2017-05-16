
#include <SmingCore/SmingCore.h>
#include <spiffsconfig.h>
#include "mqtt.h"
#include "telnet.h"

///////////////////////////////////////
///// Telnet Backup command interface
///////////////////////////////////////

TelnetServer telnetServer;
int16_t auth_num_cmds=0;

void telnetCmdNetSettings(String commandLine  ,CommandOutput* commandOutput)
{
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);
	if (auth_num_cmds <= 0)
	{
		commandOutput->println("Prevent Mistakes, give auth token");
		return;
	}
	auth_num_cmds--;
	if (numToken != 3)
	{
		commandOutput->println("Usage set ip|nm|gw|dhcp|wifissid|wifipass|mqttbroker|mqttport|mqttclientid|mqttuser|mqttpass|publishinterval|debounceinterval <value>");
	}
	else if (commandToken[1] == "ip")
	{
		IPAddress newip(commandToken[2]);
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),newip.toString().c_str());
		if (!newip.isNull())
			SpiffsConfig.ip = newip;
	}
	else if (commandToken[1] == "nm")
	{
		IPAddress newip(commandToken[2]);
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),newip.toString().c_str());
		if (!newip.isNull())
			SpiffsConfig.netmask = newip;
	}
	else if (commandToken[1] == "gw")
	{
		IPAddress newip(commandToken[2]);
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),newip.toString().c_str());
		if (!newip.isNull())
			SpiffsConfig.gw = newip;
	}
	else if (commandToken[1] == "wifissid")
	{
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),commandToken[2].c_str());
		SpiffsConfig.wifi_ssid = commandToken[2];
	}
	else if (commandToken[1] == "wifipass")
	{
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),commandToken[2].c_str());
		SpiffsConfig.wifi_pass = commandToken[2];
	}
	else if (commandToken[1] == "mqttbroker")
	{
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),commandToken[2].c_str());
		SpiffsConfig.mqtt_broker = commandToken[2];
	}
	else if (commandToken[1] == "mqttport")
	{
		uint32_t newport = commandToken[2].toInt();
		commandOutput->printf("%s: '%d'\r\n",commandToken[1].c_str(),newport);
		if (newport > 0 && newport < 65536)
			SpiffsConfig.mqtt_port = newport;
	}
	else if (commandToken[1] == "publishinterval")
	{
		uint32_t checkintervall = commandToken[2].toInt();
		commandOutput->printf("%s: '%d'\r\n",commandToken[1].c_str(),checkintervall);
		if (checkintervall >= 1000 && checkintervall <= 600000)
			SpiffsConfig.publish_interval = checkintervall;
	}
	else if (commandToken[1] == "debounceinterval")
	{
		uint32_t debounceintervall = commandToken[2].toInt();
		commandOutput->printf("%s: '%d'\r\n",commandToken[1].c_str(),debounceintervall);
		if (debounceintervall >= 10 && debounceintervall <= 10000)
			SpiffsConfig.debounce_interval = debounceintervall;
	}
	else if (commandToken[1] == "mqttclientid")
	{
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),commandToken[2].c_str());
		SpiffsConfig.mqtt_clientid = commandToken[2];
	}
	else if (commandToken[1] == "mqttuser")
	{
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),commandToken[2].c_str());
		SpiffsConfig.mqtt_user = commandToken[2];
	}
	else if (commandToken[1] == "mqttpass")
	{
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),commandToken[2].c_str());
		SpiffsConfig.mqtt_pass = commandToken[2];
	}
	else if (commandToken[1] == "dhcp")
	{
		SpiffsConfig.enabledhcp = commandToken[2] == "1" || commandToken[2] == "true" || commandToken[2] == "yes" || commandToken[2] == "on";
		commandOutput->printf("%s: '%s'\r\n",commandToken[1].c_str(),(SpiffsConfig.enabledhcp)?"on":"off");
	} else {
		commandOutput->printf("Invalid subcommand. Try %s list\r\n", commandToken[0].c_str());
	}
}

void telnetCmdPrint(String commandLine  ,CommandOutput* commandOutput)
{
	commandOutput->println("You are connecting from: " + telnetServer.getRemoteIp().toString() + ":" + String(telnetServer.getRemotePort()));
	commandOutput->println("== Dumping Configuration ==");
	commandOutput->println("WiFi SSID: " + SpiffsConfig.wifi_ssid + " actual: "+WifiStation.getSSID());
	commandOutput->println("WiFi Pass: " + SpiffsConfig.wifi_pass + " actual: "+WifiStation.getPassword());
	commandOutput->println("Hostname: " + WifiStation.getHostname());
	commandOutput->println("MAC: " + WifiStation.getMAC());
	commandOutput->println("IP: " + SpiffsConfig.ip.toString() + " actual: "+WifiStation.getIP().toString());
	commandOutput->println("NM: " + SpiffsConfig.netmask.toString()+ " actual: "+WifiStation.getNetworkMask().toString());
	commandOutput->println("GW: " + SpiffsConfig.gw.toString()+ " actual: "+WifiStation.getNetworkGateway().toString());
	commandOutput->println((SpiffsConfig.enabledhcp)?"DHCP: on":"DHCP: off");
	commandOutput->println((WifiStation.isEnabledDHCP())?"actual DHCP: on":"DHCP: off");
	commandOutput->println("MQTT Broker: " + SpiffsConfig.mqtt_broker + ":" + String(SpiffsConfig.mqtt_port));
	commandOutput->println("MQTT ClientID: " + SpiffsConfig.mqtt_clientid);
	commandOutput->println("MQTT Login: " + SpiffsConfig.mqtt_user +"/"+ SpiffsConfig.mqtt_pass);
	commandOutput->println("Publish Interval: " + String(SpiffsConfig.publish_interval));
	commandOutput->println("Debounce Interval: " + String(SpiffsConfig.debounce_interval));
}

void telnetCmdSave(String commandLine  ,CommandOutput* commandOutput)
{
	if (auth_num_cmds <= 0)
	{
		commandOutput->println("Prevent Mistakes, give auth token");
		return;
	}
	auth_num_cmds--;
	commandOutput->println("OK, saving values...");
	SpiffsConfig.save();
}

void telnetCmdLs(String commandLine  ,CommandOutput* commandOutput)
{
	Vector<String> list = fileList();
	for (int i = 0; i < list.count(); i++)
		commandOutput->println(String(fileGetSize(list[i])) + " " + list[i]);
}

void telnetCmdCatFile(String commandLine  ,CommandOutput* commandOutput)
{
	if (auth_num_cmds <= 0)
	{
		commandOutput->println("Prevent Mistakes, give auth token");
		return;
	}
	auth_num_cmds--;
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);

	if (numToken != 2)
	{
		commandOutput->println("Usage: cat <file>");
		return;
	}
	if (fileExist(commandToken[1]))
	{
		commandOutput->println("Contents of "+commandToken[1]);
		commandOutput->println(fileGetContent(commandToken[1]));
	} else {
		commandOutput->println("File '"+commandToken[1]+"' does not exist");
	}
}

void telnetCmdLoad(String commandLine  ,CommandOutput* commandOutput)
{
	commandOutput->printf("OK, reloading values...\r\n");
	SpiffsConfig.load();
}

void telnetCmdReboot(String commandLine  ,CommandOutput* commandOutput)
{
	commandOutput->printf("OK, restarting...\r\n");
	telnetServer.flush();
	telnetServer.close();
	System.restart();
}

/*void telnetAirUpdate(String commandLine  ,CommandOutput* commandOutput)
{
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);

	if (auth_num_cmds <= 0)
	{
		commandOutput->println("Prevent Mistakes, give auth token");
		return;
	}
	auth_num_cmds--;

	if (2 != numToken )
	{
		commandOutput->println("Usage: update <url dir to files>");
		return;
	} else
	{
		if (commandToken[1].length() > 0 && commandToken[1].startsWith("http") && commandToken[1].endsWith("/"))
		{
			commandOutput->println("URL OK: "+commandToken[1]);
		} else
		{
			commandOutput->println("Invalid URL: "+commandToken[1]);
			return;
		}
		commandOutput->println("Updating...");
		OtaUpdate(commandToken[1]+"rom0.bin",commandToken[1]+"rom1.bin",commandToken[1]+"spiff_rom.bin");
	}

}*/

void telnetAuth(String commandLine  ,CommandOutput* commandOutput)
{
	if (commandLine == "auth prevents mistakes "+SpiffsConfig.authtoken)
	{
		auth_num_cmds = 3;
		commandOutput->println("go ahead, use your 3 commands wisely (if you break it, you fix it)");
	} else {
		auth_num_cmds = 0;
		commandOutput->println("no dice");
	}
}

void startTelnetServer()
{
	telnetServer.listen(TELNET_PORT_);
	telnetServer.enableCommand(true);
	//TODO: use encryption and client authentification
#ifdef ENABLE_SSL
	telnetServer.addSslOptions(SSL_SERVER_VERIFY_LATER);
	telnetServer.setSslClientKeyCert(default_private_key, default_private_key_len,
							  default_certificate, default_certificate_len, NULL, true);
	telnetServer.useSsl = true;
#endif
}

void telnetRegisterCmdsWithCommandHandler()
{
	commandHandler.registerCommand(CommandDelegate("set","Change settings","configGroup", telnetCmdNetSettings));
	commandHandler.registerCommand(CommandDelegate("save","Save settings","configGroup", telnetCmdSave));
	commandHandler.registerCommand(CommandDelegate("load","Load settings","configGroup", telnetCmdLoad));
	commandHandler.registerCommand(CommandDelegate("show","Show settings","configGroup", telnetCmdPrint));
	commandHandler.registerCommand(CommandDelegate("ls","List files","configGroup", telnetCmdLs));
	commandHandler.registerCommand(CommandDelegate("cat","Cat file contents","configGroup", telnetCmdCatFile));
	commandHandler.registerCommand(CommandDelegate("restart","restart ESP8266","systemGroup", telnetCmdReboot));
	// commandHandler.registerCommand(CommandDelegate("update","OTA Firmware update","systemGroup", telnetAirUpdate));
	commandHandler.registerCommand(CommandDelegate("auth","auth token","systemGroup", telnetAuth));
}
