#ifndef APPMGR_H
#define APPMGR_H

class AppMgr
{
public:
	AppMgr();
	static AppMgr& getInstance();
private:
	~AppMgr();
};

#endif // APPMGR_H
