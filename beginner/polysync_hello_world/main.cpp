#include <polysync_node.hpp>
#include <polysync_message.hpp>

int main()
{
    polysync::Node myNode( "Hello from Polysync!" );
    myNode.registerListener( MSG_TYPE_OBJECT_STREAM );
    myNode.registerListener( MSG_TYPE_LANE_MODEL );

    while( ! myNode.exitSignal() )
    {
        void* msg = myNode.queuePopSafe();

        if( msg != nullptr )
        {
            ps_msg_type type = polysync::message::getType( msg );

            if( type == MSG_TYPE_OBJECT_STREAM )
            {
                polysync::message::ObjectStream( msg ).print();
            }
            else if( type == MSG_TYPE_LANE_MODEL )
            {
                polysync::message::LaneModel( msg ).print();
            }
        }
    }

    return 0;
}
