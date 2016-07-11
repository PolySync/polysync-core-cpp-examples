#ifndef EXAMPLELOGFILE_HPP
#define EXAMPLELOGFILE_HPP

#include <PolySyncLogfile.hpp>

class ExampleLogfile : public polysync::Logfile
{

public:

    ExampleLogfile( polysync::Node & node )
        :
        polysync::Logfile( node )
    {
        // empty
    }

private:

    void readLogfileHandler( const polysync::LogfileAttributes & attributes,
                             ps_msg_type messageType,
                             const polysync::LogfileRecord & record ) override
    {
        // Unused arguments
        static_cast< void >( attributes );

        static_cast< void >( messageType );

        std::cout << "readLogfileHandler" << std::endl;

        std::cout
             << "log record - index: " << record.getIndex() << " - "
             << "size: " << record.getSize() << " bytes - "
             << "RnR timestamp (header.timestamp): " << record.getTimestamp()
             << std::endl;

        auto messageRef = record.getRecordData();

        // get the PolySync message reference
        // datamodel::buildMessage( *_node, record. )->printHeader();

        // release the resources for the
        psync_message_free( _node->getNodeReference(), &messageRef );

        // do something with the data ...
    }

};

#endif // EXAMPLELOGFILE_HPP
