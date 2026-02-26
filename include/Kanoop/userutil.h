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
#endif

#include <QString>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for querying Unix user and group information.
 *
 * POSIX-only methods are guarded by `#ifndef __WIN32` and are unavailable on Windows.
 */
class KANOOP_EXPORT UserUtil
{
public:
#ifndef __WIN32
    /**
     * @brief Look up the numeric GID for a group name.
     * @param name Group name string
     * @return Group ID, or (gid_t)-1 if not found
     */
    static gid_t gidFromName(const QString& name);

    /**
     * @brief Look up the numeric UID for a user name.
     * @param name User name string
     * @return User ID, or (uid_t)-1 if not found
     */
    static uid_t uidFromName(const QString& name);

    /**
     * @brief Return the group name for a numeric GID.
     * @param gid Group ID to look up
     * @return Group name string, or empty string if not found
     */
    static QString nameFromGid(gid_t gid);

    /**
     * @brief Return the user name for a numeric UID.
     * @param uid User ID to look up
     * @return User name string, or empty string if not found
     */
    static QString nameFromUid(uid_t uid);

    /**
     * @brief Return the UID of the process owner.
     * @return UID of the currently running process
     */
    static uid_t currentUser();

    /**
     * @brief Return the full (display) name of the process owner.
     * @return GECOS full name string, or empty string on error
     */
    static QString currentUserFullName();

    /**
     * @brief Test whether a user is a member of a group.
     * @param uid User ID to check
     * @param gid Group ID to check membership in
     * @return true if the user is a member of the group
     */
    static bool isUserMemberOfGroup(uid_t uid, gid_t gid);
#endif

    /**
     * @brief Return the login name of the current user (cross-platform).
     * @return User name string
     */
    static QString currentUserName();
};

#endif // USERUTIL_H
