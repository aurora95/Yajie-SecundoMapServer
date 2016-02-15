#ifndef APPMAPSERVER_H
#define APPMAPSERVER_H
#include <iostream>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "XMap.h"
#include "XMongoDBService.h"
#include "crow_all.h"
#include "XMapMessage.h"
#include "Settings.h"


class AppMapServer
{
    public:
        AppMapServer(int _port);
        virtual ~AppMapServer();
        void run();
    protected:
    private:
        XMap getMap(double longitude, double latitude);
        std::string postMap(XMapMessage mapmsg);
        int64_t getMapVersion(XMapMessage mapmsg);

        crow::SimpleApp app;
        int port;
        XMongoDBService *dbService;
};

#endif // APPSERVER_H
