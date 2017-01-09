#include "SubscriberMap.hpp"

//A function to initialize the mapping of all node types
SubscriberMap::SubscriberMap()
{
    _initTimestamp = polysync::getTimestamp();

    _outputMessages[ "ps_radar_targets_msg" ] =
        Subscriber<
                polysync::datamodel::RadarTargetsMessage >::Create;    
    _outputMessages[ "ps_lidar_points_msg" ] =
        Subscriber<
                polysync::datamodel::LidarPointsMessage >::Create;
    _outputMessages[ "ps_lane_model_msg" ] =
        Subscriber<
                polysync::datamodel::LaneModelMessage >::Create;
    _outputMessages[ "ps_objects_msg" ] =
        Subscriber<
                polysync::datamodel::ObjectsMessage >::Create;
    _outputMessages[ "ps_platform_motion_msg"] =
        Subscriber<
                polysync::datamodel::PlatformMotionMessage >::Create;
}
    
//Call of actual bridge type spins up a node with any
//parameters which must be set first
polysync::DataSubscriber* SubscriberMap::newType( std::string nodeType )
{
    return _outputMessages[ nodeType ]( _initTimestamp );
}

void SubscriberMap::printMessages()
{
    std::cout << "Available messages are:\n";

    for(
            _messagesIterator = _outputMessages.begin();
            _messagesIterator != _outputMessages.end();
            _messagesIterator++ )
    {
        std::cout << _messagesIterator->first << "\n";
    }

}
