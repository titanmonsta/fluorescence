#ifndef FLUO_NET_PACKETS_CLIENTVERSION_HPP
#define FLUO_NET_PACKETS_CLIENTVERSION_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class ClientVersion : public Packet {
public:
    ClientVersion();
    ClientVersion(const UnicodeString& version);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    UnicodeString version_;
};

}
}
}

#endif