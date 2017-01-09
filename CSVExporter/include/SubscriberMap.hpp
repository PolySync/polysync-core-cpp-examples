#ifndef SUBSCRIBERMAP_HPP
#define SUBSCRIBERMAP_HPP

#include <functional> /* std */
#include <map>

#include "Subscriber.hpp" /* PolySync */

/**
 * @class SubscriberMap
 * This class is used to map from string to a message type allowing for command
 * line arguments. This map uses a template class Subscriber.hpp to
 * construct each message type and a Create function which returns the base 
 * pointer [DataSubscriber].
 * 
 * @code
 * 
 * SubscriberMap callType;
 * 
 * auto myMessageType = callType.newType( "ps_radar_targets_msg" );
 *
 * polysync::Application::getInstance()->connectPolySync();
 * 
 * delete myMessageType;
 * 
 * @endcode
 */
class SubscriberMap 
{ 

public:
    SubscriberMap();

    /**
     * @brief This function initializes a node of a specific type from the
     * template class.
     * 
     * @param bridgeType [in] the c type name of message to be constructed
     * 
     * @return The base class pointer of polysync::Node*
     */
    polysync::DataSubscriber* newType(std::string nodeType);

    /**
     * @brief This function prints out the available messages in the map for
     * use with the help output in the arguments.
     */
    void printMessages();

private: 
    
    //The primary map used for calling various node types by string name
    std::map< std::string, std::function< polysync::DataSubscriber*(
            unsigned long int timestamp ) > > _outputMessages;
    std::map< std::string, std::function< polysync::DataSubscriber*(
            unsigned long int timestamp ) > >::iterator _messagesIterator;
    
    unsigned long int _initTimestamp;

};

#endif //SUBSCRIBERMAP_HPP
