#ifndef SHOW_RPC2_CLASS
#define SHOW_RPC2_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/ALRPCObjects/TextAlignment.h"
#include <string>

namespace RPC2Communication
{
    class Show : public RPC2Request
    {
    public:
        Show();
        ~Show();

        void setMainField1(const std::string & str);
        void setMainField2(const std::string & str);
        void setTextAlignment(const TextAlignment& s);
        void setStatusBar(const std::string& s);
        void setMediaClock(const std::string& s);
        void setMediaTrack(const std::string& str);

        const std::string * getMainField1() const;
        const std::string * getMainField2() const;
        const TextAlignment * getTextAlignment() const;
        const std::string * getStatusBar() const;
        const std::string * getMediaClock() const;
        const std::string * getMediaTrack() const;

    private:
        std::string * mMainField1;
        std::string * mMainField2;
        TextAlignment * mAlignment;
        std::string * mStatusBar;
        std::string * mMediaClock;
        std::string * mMediaTrack;

        friend class ShowMarshaller;
    };
}

#endif