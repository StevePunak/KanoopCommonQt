/**
 *  UserUtil
 *
 *  Some static helper methods for Linux users / groups
 *
 *  Stephen Punak, July 08 2019
 */
#ifndef USERUTIL_H
#define USERUTIL_H

#ifndef __WIN32

#include <pwd.h>
#include <grp.h>
#include <QString>
#include "kanoopcommon.h"

class KANOOP_EXPORT UserUtil
{
public:
    static gid_t gidFromName(const QString& name);
    static uid_t uidFromName(const QString& name);

    static QString nameFromGid(gid_t gid);
    static QString nameFromUid(uid_t uid);

    static uid_t currentUser();
    static QString currentUserName();
    static QString currentUserFullName();
    static bool isUserMemberOfGroup(uid_t uid, gid_t gid);
};

#endif

#endif // USERUTIL_H
