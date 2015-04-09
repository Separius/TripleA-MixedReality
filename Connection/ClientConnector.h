/*
 * ClientConnector.h
 *
 *  Created on: Oct 24, 2008
 *      Author: brt
 */

#ifndef CLIENTCONNECTOR_H_
#define CLIENTCONNECTOR_H_

#include "Client.h"

#include <string>

class ClientConnector {
public:
	ClientConnector(const std::string& host, int port);

	int tries() const;
	void tries(int tries);

	int retryTime() const;
	void retryTime(int a_time);


	Connection * getConnection();
	ServerProperties ConnectToServer(const ConnectionParameters& client);

	~ClientConnector();

private:

	std::string generateMsgConnectServer(const ConnectionParameters& params);

    Connection * mUdpConnection;
    std::string mHost;
    int mPort;
    int mTries;
    int mTimeout;
    int mRetryTime;
};

#endif /* CLIENTCONNECTOR_H_ */
