#include "CSVExport.hpp"

/**
 * @brief This function is main which collects arguments passed in and then
 * proceeds to open a subscriber application. Each subscriber is a templated
 * class with in the SubscriberMap factory constructing one CSV for each
 * message topic. The csv files are names by message type name and a common
 * timestamp at subscriber spin up.
 */
int main( int argc, char *argv[] )
{
    //Load in the Subscriber Factory that builds message types from args
    SubscriberMap callType;

    Parameters setParameters;
    
    //Set the arguments passed in from command line and error out if empty
    //or missing atleast one message type
    if( handleOptions( 
            {argc, argv}, 
            &setParameters ) != 0 )
    {
        return 1;
    }

    if( setParameters.messageType.empty() )
    {
        printMissingMessageType();

        return 0;
    }

    //Subscribe to each message type requested by the user
    std::vector< std::string >::iterator messageTypeIterator;
    std::vector< polysync::DataSubscriber* > subscriberType;

    for( messageTypeIterator = setParameters.messageType.begin();
            messageTypeIterator != setParameters.messageType.end();
            messageTypeIterator++ )
    {
        subscriberType.push_back(callType.newType( 
                    *messageTypeIterator ) );
    }

    //Now that all subscribers are up connect with the polysync bus
    polysync::Application::getInstance()->connectPolySync();

    //Once connection has been left cleanup subscriber classes
    std::vector< polysync::DataSubscriber* >::iterator subscriberIterator; 

    for( subscriberIterator = subscriberType.begin();
            subscriberIterator != subscriberType.end();
            subscriberIterator++ )
    {
        delete *subscriberIterator;
    }

    return 0;
}


int handleOptions( 
        GetOpt && getOpt, 
        Parameters * setParameters )
{
    if( getOpt.getOptions().empty() )
    {
        std::cout << "It appears you have not entered any arguments ";
        
        std::cout << "showing help:\n";
        
        printHelp();
        
        return 1;
    }

    for ( auto option : getOpt.getOptions() )
    {
        //Check for message type as this is required
        if( option.getOptionString() == "t" )
        {
            setParameters->messageType = option.getArguments();
        }
        
        if( option.getOptionString() == "h" )
        {
            printHelp();
            
            return 1;
        }

        if( option.getOptionString() == "m" )
        {
            SubscriberMap messages;
            messages.printMessages();
            return 1;
        }
    }

    return 0;
}


void printHelp()
{
    std::cout << "Message Type is a required option all others are optional\n";
    
    std::cout << "Options: \n";
    
    std::cout << "Flag:     Description:                Default:\n";
    
    std::cout << "-t        Message Type\n"; 
    
    std::cout << "-m        Print available messages\n";

    std::cout << "-h        Prints help\n";

    return;
}


void printMissingMessageType()
{
    std::cout << "It appears a Message Type was not set, ";
    
    std::cout << "make sure to set one or more message type(s) with -t.\n";
        
    SubscriberMap messages;
    messages.printMessages(); 

    return;
}

