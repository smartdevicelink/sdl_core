#ifndef SHOW_RPC2_CLASS
#define SHOW_RPC2_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/TextAlignment.h"
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

        std::string getMainField1() const;
        std::string getMainField2() const;
        TextAlignment getTextAlignment() const;
        std::string getStatusBar() const;
        std::string getMediaClock() const;
        std::string getMediaTrack() const;

    private:
        std::string mMainField1;
        std::string mMainField2;
        TextAlignment mAlignment;
        std::string mStatusBar;
        std::string mMediaClock;
        std::string mMediaTrack;

        friend class ShowMarshaller;
    };
}

#endif