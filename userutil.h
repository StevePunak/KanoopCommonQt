#ifndef USERUTIL_H
#define USERUTIL_H

#ifndef __WIN32

#include <pwd.h>
#include <grp.h>
#include <QString>

class UserUtil
{
public:
    static gid_t gidFromName(const QString& name);
    static uid_t uidFromName(const QString& name);

    static QString nameFromGid(gid_t gid);
    static QString nameFromUid(uid_t uid);

    static uid_t currentUser();
    static QString currentUserName();
    static bool isUserMemberOfGroup(uid_t uid, gid_t gid);
};

#endif

#endif // USERUTIL_H
