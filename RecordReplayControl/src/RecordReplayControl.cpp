#include <iostream>
#include <vector>

#include "polysync_core.h"
#include "psyncDcps.h"
#include "PolySyncRecordReplay.hpp"

using namespace polysync;
using std::cout;
using std::cin;
using std::endl;

static void print_rnr_sessions(
        const LogSessionInfoMap& sessionsInfo )
{
    for( auto sessionInfo : sessionsInfo )
    {
        const std::vector< LogfileDescription >& fileDescriptions =
            sessionInfo.second.getLogfileDescriptions();

        printf( "session Id: %llu - files: %lu\n",
                sessionInfo.second.getId(),
                fileDescriptions.size() );

        for( auto description : fileDescriptions )
        {
            printf( "    name '%s'\n", description.filename.c_str() );
            printf( "    - file_size %lu bytes\n",
                    (unsigned long)description.fileSize );
            printf( "    - duration %.4f seconds\n",
                    (double)description.duration / 1000000.0 );
        }
    }
}

void useReplay( ps_node_ref nodeRef, ps_rnr_session_id sessionId )
{
    ReplaySession replay{ nodeRef };

    replay.load( sessionId );

    replay.start();

    std::string tmp;

    cin >> tmp;

    replay.stop();
}

void useRecord( ps_node_ref nodeRef, ps_rnr_session_id sessionId )
{
    RecordSession record{ nodeRef };

    record.setId( sessionId );

    record.start();

    std::string tmp;

    cin >> tmp;

    record.stop();
}

int main()
{
    ps_node_ref nodeRef = PSYNC_NODE_REF_INVALID;

    if( psync_init( "RnrControl",
            PSYNC_NODE_TYPE_API_USER,
            PSYNC_DEFAULT_DOMAIN,
            PSYNC_SDF_ID_INVALID,
            PSYNC_INIT_FLAG_STDOUT_LOGGING,
            &nodeRef ) != DTC_NONE )
    {
        return 0;
    }

    std::string recordOrReplay;

    ps_rnr_session_id sessionId;

    LogSessionInfoManager& infoManager =
            LogSessionInfoManager::getReference( nodeRef );

    auto availableSessions = infoManager.getLogSessionInfo();

    print_rnr_sessions( availableSessions );

    cout << "Select:\n[1] Replay\n[2] Record\n";

    cin >> recordOrReplay;

    cout << "Enter session id: ";

    cin >> sessionId;

    if( recordOrReplay == "1" )
    {
        useReplay( nodeRef, sessionId );
    }
    else if( recordOrReplay == "2" )
    {
        useRecord( nodeRef, sessionId );
    }

    psync_release( &nodeRef );

    return 0;
}