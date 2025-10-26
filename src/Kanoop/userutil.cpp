#include "Kanoop/userutil.h"

#ifndef __WIN32
#include "unistd.h"
#else
#include <windows.h>
#endif

#ifndef __WIN32
gid_t UserUtil::gidFromName(const QString &name)
{
    gid_t ret = 0;
    struct group* group;
    group = getgrnam(name.toLatin1().constData());
    if(group != nullptr)
        ret = group->gr_gid;
    return ret;
}

uid_t UserUtil::uidFromName(const QString &name)
{
    uid_t ret = 0;
    struct passwd* passwd;
    passwd = getpwnam(name.toLatin1().constData());
    if(passwd != nullptr)
        ret = passwd->pw_uid;
    return ret;
}

QString UserUtil::nameFromGid(gid_t gid)
{
    QString ret = 0;
    struct group* group;
    group = getgrgid(gid);
    if(group != nullptr)
        ret = group->gr_name;
    return ret;
}

QString UserUtil::nameFromUid(uid_t uid)
{
    QString ret;
    struct passwd* passwd;
    passwd = getpwuid(uid);
    if(passwd != nullptr)
        ret = passwd->pw_name;
    return ret;
}

uid_t UserUtil::currentUser()
{
    return uidFromName(currentUserName());
}

QString UserUtil::currentUserFullName()
{
    QString result;
    struct passwd* passwd;
    passwd = getpwnam(currentUserName().toLatin1().constData());
    if(passwd != nullptr)
        result = passwd->pw_gecos;
    return result;
}

bool UserUtil::isUserMemberOfGroup(uid_t uid, gid_t gid)
{
    struct passwd* pw = getpwuid(uid);
    if(pw == nullptr)
        return false;

    int ngroups = 0;
    getgrouplist(pw->pw_name, pw->pw_gid, nullptr, &ngroups);

    gid_t groups[ngroups];

    getgrouplist(pw->pw_name, pw->pw_gid, groups, &ngroups);

    for(int i = 0;i < ngroups;i++)
        if(groups[i] == gid)
            return true;

    return false;
}

QString UserUtil::currentUserName()
{
    char buf[1024];
    if(getlogin_r(buf, sizeof(buf)) != 0)
        buf[0] = 0;
    return QString(buf);
}

#else

QString UserUtil::currentUserName()
{
#define MAX_UNLEN 512
    QString result;
    char acUserName[MAX_UNLEN + 1];
    DWORD nUserName = sizeof(acUserName);
    if (GetUserNameA(acUserName, &nUserName)) { // Use GetUserNameA for char array
        result = acUserName;
    }
    return result;
}
#endif

