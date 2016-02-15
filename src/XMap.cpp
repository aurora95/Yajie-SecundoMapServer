#include "XMap.h"
#include "XMongoDBService.h"
#include <string>
#include <sstream>
#include <math.h>
#include <mongo/bson/bson.h>
#include <mongo/client/dbclient.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace mongo;
using namespace boost::property_tree;
using namespace std;

XMap::XMap()
{
    memset(data, 1, sizeof(data));
    version = 0;
    loc.latitude = 0;
    loc.longitude = 0;
}

XMap::XMap(int _id)
{
    mongo::Query q = MONGO_QUERY("_id" << _id);
    std::auto_ptr<mongo::DBClientCursor> p_map = XMongoDBService::GetConnection().query(MAPS_COLLECTION_NAME, q);
    if(p_map->more()){
        mongo::BSONObj obj = p_map->peekFirst();
        std::vector<BSONElement> loc_array= obj.getField("loc").Array();
        loc.longitude = loc_array[0].Double();
        loc.latitude = loc_array[1].Double();
        version = obj.getField("version").Long();
        strcpy(data, obj.getStringField("data"));
    }
    else{
        version = 0;
    }
}

XMap::XMap(Coordinate<double> user_loc)
{
    loc = getRoundedLoc(user_loc);
    mongo::Query q = MONGO_QUERY("loc" << BSON_ARRAY(loc.longitude << loc.latitude));
    std::auto_ptr<mongo::DBClientCursor> p_map = XMongoDBService::GetConnection().query(MAPS_COLLECTION_NAME, q);
    if(p_map->more()){
        mongo::BSONObj obj = p_map->peekFirst();
        version = obj.getField("version").Long();
        strcpy(data, obj.getStringField("data"));
    }
    else{
        version = 0;
        memset(data, 1, sizeof(data));
    }
}

XMap::~XMap()
{
    //dtor
}

Coordinate<double> XMap::getLoc()
{
    return loc;
}

XMap& XMap::setLoc(Coordinate<double> _loc)
{
    loc = getRoundedLoc(_loc);
    return *this;
}

char* XMap::getDataPointer()
{
    return data;
}

void XMap::getData(char* dest)
{
    memcpy(dest, data, sizeof(data));
}

XMap& XMap::setData(char* src)
{
    memcpy(data, src, sizeof(data));
    return *this;
}

XMap& XMap::setData(char* src, int bias_x, int bias_y, int length)
{
    int bias = bias_x * XMAP_DATASIZE + bias_y;
    for(int i = 0; i<length; i++){
        for(int j = 0; j<length; j++){
            data[bias + i*XMAP_DATASIZE + j] = src[i*XMAP_DATASIZE + j];
        }
    }
    return *this;
}

long long XMap::getVersion()
{
    return version;
}

XMap& XMap::setVersion(long long _version)
{
    version = _version;
    return *this;
}

XMap& XMap::save()
{
    try
    {
        if (version != 0)
        {
            XMongoDBService::GetConnection().update(MAPS_COLLECTION_NAME,
                                                    MONGO_QUERY("loc" << BSON_ARRAY(loc.longitude << loc.latitude)),
                                                    BSON("loc" << BSON_ARRAY(loc.longitude << loc.latitude) <<
                                                        "data" << data <<
                                                        "$inc" << BSON("version" << (long long)1)),
                                                    true);
        }
        else
        {
            XMongoDBService::GetConnection().save(MAPS_COLLECTION_NAME, BSON("loc" << BSON_ARRAY(loc.longitude << loc.latitude) <<
                                                        "data" << data <<
                                                        "version" << (long long)1));
        }
    }
    catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }
    return *this;
}

string XMap::toJSON()
{
    ptree pt;
    ptree ptloc;
    ptloc.put<double>("longitude", loc.longitude);
    ptloc.put<double>("latitude", loc.latitude);
    pt.add_child("loc", ptloc);
    pt.put<string>("data", string(data));
    pt.put<long long>("version", version);
    stringstream output_string;
    json_parser::write_json(output_string, pt);
    return output_string.str();
}

XMap& XMap::fromJSON(string src)
{
    stringstream jsonstream;
    jsonstream << src;
    ptree pt;
    try{
        json_parser::read_json(jsonstream, pt);
        ptree ptloc = pt.get_child("loc");
        loc.latitude = ptloc.get<double>("latitude");
        loc.longitude = ptloc.get<double>("longitude");
        string str_data = pt.get<string>("data");
        strcpy(data, str_data.c_str());
        version = pt.get<long long>("version");
    }
    catch(ptree_error & e){
        cout<<e.what()<<endl;
    }
    return *this;
}

Coordinate<double> XMap::getRoundedLoc(Coordinate<double> _loc)
{
    _loc.latitude = std::floor((long double)_loc.latitude * 10000) / 10000;
    _loc.longitude = std::floor((long double)_loc.longitude * 10000) / 10000;
    return _loc;
}
