#ifndef XMAP_H
#define XMAP_H

#include "Settings.h"
#include "XMapMessage.h"
#include <string>

class XMap
{
    public:
        XMap();
        XMap(int _id);
        XMap(Coordinate<double> user_loc);
        virtual ~XMap();
        Coordinate<double> getLoc();
        XMap& setLoc(Coordinate<double> _loc);
        char* getDataPointer();
        void getData(char* dest);
        XMap& setData(char* src);
        XMap& setData(char* src, int bias_x, int bias_y, int length);
        long long getVersion();
        XMap& setVersion(long long _version);
        XMap& save();

        std::string toJSON();
        XMap& fromJSON(std::string src);

        static Coordinate<double> getRoundedLoc(Coordinate<double> _loc);
    protected:
    private:
        Coordinate<double> loc;
        char data[XMAP_DATASIZE * XMAP_DATASIZE];
        long long version;
};

#endif // XMAP_H
