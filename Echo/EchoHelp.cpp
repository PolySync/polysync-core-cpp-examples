#include <PolySyncDataModel.hpp>

#include "EchoHelp.hpp"


using namespace std;

namespace polysync
{

std::vector< std::string > EchoHelp::getHelpFlags()
{
    std::vector< std::string > cmdLineFlagsHelp;

        cmdLineFlagsHelp.emplace_back ( "-h" );
        cmdLineFlagsHelp.emplace_back ( "-f <MESSAGE_TYPE>" );
        cmdLineFlagsHelp.emplace_back ( "-H" );
        cmdLineFlagsHelp.emplace_back ( "-o <FILE_NAME>" );
        cmdLineFlagsHelp.emplace_back ( "-t <ECHO_RUNTIME>" );

    return cmdLineFlagsHelp;
}


std::vector< std::string > EchoHelp::getHelpDescriptions()
{
    std::vector< std::string > flagDescriptionsHelp;

    flagDescriptionsHelp.emplace_back
        ( " Show this help message [optional]." );

    flagDescriptionsHelp.emplace_back
        ( " Filter for a single message type OR multiple message types [optional]. \n"
        " Usage Example 1: Single Message: \n"
            " $ polysync-echo -f ps_diagnostic_trace_msg \n\n"
        " Usage Example 2: Multiple Messages: \n"
            " $ polysync-echo -f ps_diagnostic_trace_msg"
            " -f ps_lidar_points_msg \n\n"
        " Usage Example 3: Multiple Messages, Headers Only: \n $ polysync-echo"
              " -f ps_diagnostic_trace_msg -f ps_lidar_points_msg -H \n\n"
        " Usage Example 4: Multiple Messages, Headers Only, and print to file: \n"
              " $ polysync-echo -f ps_diagnostic_trace_msg -H -o yourFileName.txt \n"
              " -f ps_lidar_points_msg \n\n"
        " To see a list of message types: $ polysync-echo -f PolySyncMessageType \n\n"
        " To see sample messages on bus, run Data Generation Tutorial in \n "
            "one Terminal window, and run Echo Utility in a second Terminal window.\n\n"
         " Running $ polysync-echo with no -f filter flags prints ALL messages on bus.");

    flagDescriptionsHelp.emplace_back
        ( " Echo only message headers [optional]. \n"
        " Usage Example 1: echo headers for all message types: \n"
        " $ polysync-echo -H \n"
        " Usage Example 2: echo headers for single message type: \n"
        " $ polysync-echo -f ps_lidar_points_msg -H" );

    flagDescriptionsHelp.emplace_back
        ( " Specify an external output file for printed message data, \n"
        " in addition to standard output [optional]. \n"
        " Usage Example: print only headers for single message type to file: \n"
        " $ polysync-echo -f ps_lidar_points_msg -H -o yourFileName.txt \n\n"
        " Note: Unless you specify a new file each time, each session will \n"
        " append to the end of your specified file.");

    flagDescriptionsHelp.emplace_back
        ( " Specify the amount of time Echo should run for [optional]. \n"
        " Usage Example: Run Echo for 10 seconds: \n"
        " $ polysync-echo -t 10" );

    return flagDescriptionsHelp;
}


void EchoHelp::printHelp(const std::vector< std::string > & helpFlags ,
                             const std::vector< std::string> & helpDescriptions)
{
    cout << "\nPolySync Echo \n"
            "Standard behavior (no options) is to echo all messages "
            "currently on the bus. \n\n";

    cout << "Usage: \n $polysync-echo [options] \n\n";

    for( auto index = 0U; index < getHelpFlags().size(); ++index )
    {
        cout  << helpFlags[ index ] <<endl;
        cout  << helpDescriptions[ index ] <<endl <<endl;
    }
}


void EchoHelp::printEchoHelp()
{
    printHelp( getHelpFlags(), getHelpDescriptions() );
}


// END polysync::EchoHelp class

} // END namespace polysync
