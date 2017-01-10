#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include <fstream> /* std */
#include <iostream>
#include <sstream>

#include <PolySyncDataModel.hpp> /* PolySync files */
#include <PolySyncDataSubscriber.hpp>
#include <PolySyncApplication.hpp>


template < typename psType > class Subscriber : 
    public polysync::DataSubscriber
{
   
public:
    /**
     * @brief This function creates an instance of the class with a specific
     * node type provided and returns the base class pointer.
     * 
     * @return The base pointer of this class.
     */
    static DataSubscriber * Create( unsigned long int timestamp ) 
    {
        return new Subscriber< psType >( timestamp );
    }

    /**
     * @brief This is the class construction where the timestamp is syncronized
     * in order to provide the same timestamp in each file name created. A new
     * file is opened with the name of the message type and the timestamp passed
     * in with this function.
     *
     * @param [in] A polysync timestamp value in order to append the same time
     * to each export file of different message types.
     */
    Subscriber ( unsigned long int timestamp )
    {
        _initTimestamp = timestamp;

        //Connect the Subscriber
        connectSubscriberMethod< Subscriber >(
                psType::getName(),
                &Subscriber::handleEvents,
                this );

        polysync::Application::getInstance()->attachSubscriber( this );

        //Initialize a CSV with column header dump
        std::string filename = 
            psType::getName() + 
            "_" + 
            std::to_string( _initTimestamp ) + 
            ".csv";

        _outFile.open( filename, std::ios::app | std::ios::binary );
        
        _outFile << std::fixed;

        outputCSVHeader();
    }

    /**
     *@brief This function cleanly closes the CSV file and deletes the stream.
     */
    ~Subscriber()
    {
        _outFile.close();
    }


private:
   
    std::ofstream _outFile;
    
    unsigned long int _initTimestamp;

    /**
     * @brief This function is the handler for incoming messages. Where the
     * message data will be output to a CSV file stream.
     * 
     * @param [in] the message to be converted to CSV.
     */
    void handleEvents( std::shared_ptr< polysync::Message > message );

    /**
     * @brief This function outputs the CSV column header information to the
     * file stream after it has been opened.
     */
    void outputCSVHeader();
 
    /**
     * @brief This function is a general output of the header and
     * sensor_descriptor information.
     *
     * @param [out] The stream to output this data.
     */
    void outputHeaderData( 
            std::stringstream & repeatStream, 
            std::shared_ptr< psType > output )
    {
        //Construct repeating row information
        repeatStream 
            << output->getHeaderType() << ","
            << output->getHeaderTimestamp() << ","
            << output->getHeaderSrcGuid() << ","
            << output->getSensorDescriptorId() << ","
            << output->getSensorDescriptorType() << ","
            << output->getSensorDescriptorTransformParentId() << ","
            << output->getSensorDescriptorTransformTimestamp() << ",";

        auto stack = output->getSensorDescriptorTransformStack();

        if( stack.size() > 0 )
        {
            auto origin = stack.front().getOrigin();
            auto orientation = stack.front().getOrientation();

            repeatStream
                << stack.front().getId() << ","
                << stack.front().getTimestamp() << ","
                << origin[ 0 ] << ","
                << origin[ 1 ] << ","
                << origin[ 2 ] << ","
                << orientation[ 0 ] << ","
                << orientation[ 1 ] << ","
                << orientation[ 2 ] << ","
                << orientation[ 3 ] << ",";
        }
        else
        {
            repeatStream << ",,,,,,,,,";
        }
        
        return;
    }

}; //End Class

#endif // SUBSCRIBER_HPP
