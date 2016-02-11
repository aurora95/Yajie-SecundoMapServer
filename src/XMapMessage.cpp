#include "XMapMessage.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <sstream>

using namespace boost::property_tree;
using namespace std;

XMapMessage::XMapMessage()
{
    //ctor
}

XMapMessage::~XMapMessage()
{
    //dtor
}

void XMapMessage::toJSON(char* dest)
{
    ptree pt;
    pt.put<int>("msg_type", msg_type);
    switch(msg_type)
    {
    case XMAPMSG_QUERYMAP:
        {
            ptree ptloc;
            ptloc.put<double>("longitude", loc.longitude);
            ptloc.put<double>("latitude", loc.latitude);
            pt.add_child("loc", ptloc);

            break;
        }
    case XMAPMSG_POSTMAP:
        {
            ptree ptloc;
            ptloc.put<double>("longitude", loc.longitude);
            ptloc.put<double>("latitude", loc.latitude);
            pt.add_child("loc", ptloc);

            pt.put<int>("length", length);

            ptree ptbias;
            ptbias.put<int>("x", bias.x);
            ptbias.put<int>("y", bias.y);
            pt.add_child("bias", ptbias);

            string str_data(data);
            pt.put("data", str_data);

            break;
        }
    case XMAPMSG_GETMAP:
        {
            ptree ptloc;
            ptloc.put<double>("longitude", loc.longitude);
            ptloc.put<double>("latitude", loc.latitude);
            pt.add_child("loc", ptloc);

            break;
        }
    case XMAPMSG_ANSMAP:
        {
            ptree ptloc;
            ptloc.put<double>("longitude", loc.longitude);
            ptloc.put<double>("latitude", loc.latitude);
            pt.add_child("loc", ptloc);

            pt.put("version", version);

            break;
        }
    default:
        break;
    }
    stringstream output_string;
    json_parser::write_json(output_string, pt);
    string json_string = output_string.str();
    strcpy(dest, json_string.c_str());
}


void XMapMessage::fromJSON(char* src)
{
    stringstream jsonstream;
    jsonstream<<src;
    ptree pt;
    try{
        json_parser::read_json(jsonstream, pt);

        msg_type = pt.get<int>("msg_type");
        switch(msg_type)
        {
        case XMAPMSG_GETMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                break;
            }
        case XMAPMSG_POSTMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                ptree ptbias = pt.get_child("bias");
                bias.x = ptbias.get<int>("x");
                bias.y = ptbias.get<int>("y");
                length = ptbias.get<int>("length");
                string str_data = pt.get<string>("data");
                strcpy(data, str_data.c_str());
                break;
            }
        case XMAPMSG_QUERYMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                break;
            }
        case XMAPMSG_ANSMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                version = pt.get<uint64_t>("version");
                break;
            }
        default:
            break;
        }
    }
    catch(ptree_error & e){
        cout<<e.what()<<endl;
    }
}

void XMapMessage::fromJSON(string src)
{
    stringstream jsonstream;
    jsonstream<<src;
    ptree pt;
    try{
        json_parser::read_json(jsonstream, pt);

        msg_type = pt.get<int>("msg_type");
        switch(msg_type)
        {
        case XMAPMSG_GETMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                break;
            }
        case XMAPMSG_POSTMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                ptree ptbias = pt.get_child("bias");
                bias.x = ptbias.get<int>("x");
                bias.y = ptbias.get<int>("y");
                length = ptbias.get<int>("length");
                string str_data = pt.get<string>("data");
                strcpy(data, str_data.c_str());
                break;
            }
        case XMAPMSG_QUERYMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                break;
            }
        case XMAPMSG_ANSMAP:
            {
                ptree ptloc = pt.get_child("loc");
                loc.latitude = ptloc.get<double>("latitude");
                loc.longitude = ptloc.get<double>("longitude");
                version = pt.get<uint64_t>("version");
                break;
            }
        default:
            break;
        }
    }
    catch(ptree_error & e){
        cout<<e.what()<<endl;
    }
}
