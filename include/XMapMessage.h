#ifndef XMAPMESSAGE_H
#define XMAPMESSAGE_H

#include <stdint.h>
#include <string>
#include "Settings.h"



#define XMAPMSG_GETMAP   0x040
#define XMAPMSG_POSTMAP  0x080
#define XMAPMSG_QUERYMAP 0x100
#define XMAPMSG_ANSMAP   0x200

template <typename T>
struct Coordinate
{
    T longitude;
    T latitude;
};

class XMapMessage
{
    public:
        XMapMessage();
        virtual ~XMapMessage();

        int msg_type;
        Coordinate<double> loc;
        struct{
            int x;
            int y;
        }bias;
        int length;
        char data[XMAP_DATASIZE * XMAP_DATASIZE];
        uint64_t version;

        void toJSON(char* dest);
        void fromJSON(char* src);
        void fromJSON(std::string src);

    protected:
    private:
};

#endif // XMAPMESSAGE_H
