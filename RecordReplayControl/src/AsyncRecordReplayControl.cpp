#include <iostream>
#include <vector>

#include "polysync_core.h"
#include "psyncDcps.h"
#include "PolySyncRecordReplay.hpp"

using namespace polysync;
using std::cout;
using std::cin;
using std::endl;

class UserClass : public RecordReplaySubscriber
{
public:

    UserClass( ps_node_ref nodeRef );

    void onLogSessionInfo( const LogSessionInfoMap& sessionsInfo);

private:
};

UserClass::UserClass( ps_node_ref nodeRef ) :
        RecordReplaySubscriber( nodeRef )
{
}

void UserClass::onLogSessionInfo( const LogSessionInfoMap& sessionsInfo)
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

    std::string recordOrReplay, tmp;

    ps_rnr_session_id sessionId;

    LogSessionInfoManager& infoManager =
            LogSessionInfoManager::getReference( nodeRef );

    infoManager.getLogSessionInfo(0);

    UserClass *userInstance = new UserClass{ nodeRef };

    cout << "Select:\n[1] Replay\n[2] Record\n";

    cin >> recordOrReplay;

    cout << "Enter session id: ";

    cin >> sessionId;

    if( recordOrReplay == "1" )
    {
        ReplaySession *replay = new ReplaySession{ nodeRef };

        replay->load( sessionId );

        replay->setAsyncLooping( true );

        replay->startAsync();

        cin >> tmp;

        replay->stopAsync();

        delete replay;
    }
    else if( recordOrReplay == "2" )
    {
        RecordSession *recording = new RecordSession{ nodeRef };

        recording->setId( sessionId );

        recording->startAsync();

        cin >> tmp;

        recording->stopAsync();

        delete recording;
    }

    delete userInstance;

    psync_release( &nodeRef );

    return 0;
}