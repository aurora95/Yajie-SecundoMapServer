#include "AppMapServer.h"
#include "crow_all.h"
#include <iostream>
#include <string>
#include <sstream>
#include <ev++.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/tcp.h>

AppMapServer::AppMapServer(int _port)
{
    port = _port;
    XMongoDBService::GetConnection().createIndex(MAPS_COLLECTION_NAME, BSON("loc" << "2d"));

    CROW_ROUTE(app, "/getMap")
        .methods("GET"_method)
        ([this](const crow::request& req){
            auto params = req.url_params;
            auto param_longitude = atof(params.get("longitude"));
            auto param_latitude = atof(params.get("latitude"));
            XMap retMap = this->getMap(param_longitude, param_latitude);
            return retMap.toJSON();
        });

    CROW_ROUTE(app, "/postMap")
        .methods("POST"_method)
        ([this](const crow::request& req){
            XMapMessage mapmsg;
            mapmsg.fromJSON(req.body);
            return postMap(mapmsg);
        });

    CROW_ROUTE(app, "/getVersion")
        .methods("GET"_method)
        ([this](const crow::request& req){
            auto params = req.url_params;
            auto param_longitude = atof(params.get("longitude"));
            auto param_latitude = atof(params.get("latitude"));
            XMap retMap = this->getMap(param_longitude, param_latitude);
            std::stringstream output_string;
            output_string << retMap.getVersion();
            return output_string.str();
        });
}

AppMapServer::~AppMapServer()
{

}

XMap AppMapServer::getMap(double longitude, double latitude)
{
    Coordinate<double> coord;
    coord.latitude = latitude;
    coord.longitude = longitude;
    return XMap(coord);
}

bool AppMapServer::postMap(XMapMessage mapmsg)
{
    if (mapmsg.msg_type != XMAPMSG_POSTMAP)
        return false;
    try{
        XMap xmap(mapmsg.loc);
        xmap.setData(mapmsg.data, mapmsg.bias.x, mapmsg.bias.y, mapmsg.length).save();
        return true;
    }
    catch(const mongo::DBException e){
        std::cout << "caught " << e.what() << std::endl;
        return false;
    }
}

void AppMapServer::run()
{
    app.port(port).multithreaded().run();
}
