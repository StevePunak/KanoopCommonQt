#include "userutil.h"


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
