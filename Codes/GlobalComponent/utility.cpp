#include "utility.h"


#if defined(Q_OS_WIN)

QString Utility::generateUUID()
{
    char buf[64] = { 0 };
            GUID guid;

            if (CoCreateGuid(&guid))
            {
                return QString();
            }

            sprintf(buf,
                "%08X%08X%08X%08X",
                guid.Data1, guid.Data2, guid.Data3, guid.Data4);

            return QString(buf));
}

#elif defined(Q_OS_LINUX)

QString Utility::generateUUID()
{
    char buf[64] = { 0 };

    uuid_t uu;
    uuid_generate( uu );

    int32_t index = 0;
    for (int32_t i = 0; i < 16; i++)
    {
        int32_t len = sprintf(buf + index, "%02X", uu[i]);
        if(len < 0 )
            return QString();
        index += len;
    }

    return QString(buf);
}

#endif

