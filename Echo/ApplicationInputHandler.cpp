#include <algorithm>
#include <getopt.h>
#include <PolySyncDataModel.hpp>

#include "ApplicationInputHandler.hpp"


ApplicationInputHandler::ApplicationInputHandler()
    :
    _userFileName( {} ),
    _activeMessagesFlag( false ),
    _ignoreSelfFlag( false ),
    _filteredForMessagesFlag( false ),
    _echoMessageHeadersOnlyFlag( false ),
    _echoMessageToFileFlag( false ),
    _echoMessageToFileNoStdOutFlag( false ),
    _getOptHelpFlag( false ),
    _durationSpecifiedFlag( false ),
    _filteredMessageNames( {} )
{
    // empty
}


bool ApplicationInputHandler::optionsParse( const int argc, char * argv[] )
{
    bool parsedOptSuccess = true;

    int optionIndex = -1;

    // reset scanner
    optind = 0;

    opterr = 0;

    while ( ( optionIndex = getopt( argc, argv, "t:o:O:f:hH::a::i::") ) != -1 )
    {
        switch( optionIndex )
        {
            case 'a':

                _activeMessagesFlag = true;

            break;

            case 'i':

                _ignoreSelfFlag = true;

            break;

            case 'f':

                if( ( *argv[ optind - 1 ] )
                     && ( *argv[ optind - 1 ] == '-' ) )
                {
                    std::cout << std::endl << std::endl
                         << "Invalid usage for option -f filter single "
                         << "message type:"
                         << std::endl
                         <<"-f should be followed by a PolySync "
                         << "message type, not by another -option."
                         << std::endl
                         <<"A usage guide follows." << std::endl;

                    _getOptHelpFlag = true;
                }
                else
                {
                    _filteredForMessagesFlag = true;

                    _filteredMessageNames.emplace_back( optarg );
                }

            break;

            case 'o':

                if( ( *argv[ optind - 1 ] )
                     && ( *argv[ optind - 1 ] == '-' ) )
                {
                    std::cout << std::endl << std::endl
                         << "Invalid usage for option -o external file:"
                         << std::endl
                         << "-o should be followed by a filename "
                         << "yourfile.txt, not by another -option."
                         << std::endl << std::endl
                         << "A usage guide follows."
                         << std::endl;

                    _getOptHelpFlag = true;
                }
                else
                {
                    _userFileName = optarg;

                    _echoMessageToFileFlag = true;
                }

                break;

            case 'O':

                if( ( *argv[ optind - 1 ] )
                     && ( *argv[ optind - 1 ] == '-' ) )
                {
                    std::cout << std::endl << std::endl
                         << "Invalid usage for option -O external file:"
                         << std::endl
                         << "-O should be followed by a filename "
                         << "yourfile.txt, not by another -option."
                         << std::endl << std::endl
                         << "A usage guide follows."
                         << std::endl;

                    _getOptHelpFlag = true;
                }
                else
                {
                    _userFileName = optarg;

                    _echoMessageToFileFlag = true;
                    _echoMessageToFileNoStdOutFlag = true;
                }

                break;

            case 't':

            if( ( *argv[ optind - 1 ] )
                 && ( *argv[ optind - 1 ] == '-' ) )
            {
                std::cout << std::endl << std::endl
                     << "Invalid usage for option -t run for specific time:"
                     << std::endl
                     << "-t should be followed by how long you want Echo to"
                     << " run (sec), not by another -option."
                     << std::endl
                     << "A usage guide follows."
                     << std::endl;

                _getOptHelpFlag = true;
            }
            else if( ( *argv[ optind - 1 ] )
                 && isalpha(  *argv[ optind - 1 ] ) )
            {
                std::cout << std::endl << std::endl
                     << "Invalid usage for option -t run for specific time:"
                     << std::endl
                     << "-t should be followed by a number"
                     <<" representing  how long you want Echo to run."
                     << std::endl
                     << "A usage guide follows."
                     << std::endl;

                _getOptHelpFlag = true;
            }
            else
            {
                _duration =  std::stoull( optarg );

                _durationSpecifiedFlag = true;
            }

            break;

            case 'h':

                _getOptHelpFlag = true;

            break;

            case 'H':

                _echoMessageHeadersOnlyFlag = true;

            break;

            default: break;
        }
    }

    return parsedOptSuccess;
}


std::vector< std::string > ApplicationInputHandler::getFilteredMessageNames() const
{
    return _filteredMessageNames;
}


std::string ApplicationInputHandler::getFileName() const
{
    return _userFileName;
}


bool ApplicationInputHandler::messageTypesWereFiltered() const
{
    return _filteredForMessagesFlag;
}


bool ApplicationInputHandler::headersWereRequested() const
{
    return _echoMessageHeadersOnlyFlag;
}


bool ApplicationInputHandler::fileWasSpecified() const
{
    return _echoMessageToFileFlag;
}

bool ApplicationInputHandler::fileWasSpecifiedNoStdOut() const
{
    return _echoMessageToFileNoStdOutFlag;
}

bool ApplicationInputHandler::helpWasRequested() const
{
    return _getOptHelpFlag;
}


bool ApplicationInputHandler::activeTypesWereRequested() const
{
    return _activeMessagesFlag;
}


bool ApplicationInputHandler::durationWasSpecified () const
{
    return _durationSpecifiedFlag;
}


bool ApplicationInputHandler::ignoreSelfWasRequested() const
{
    return _ignoreSelfFlag;
}


unsigned long long ApplicationInputHandler::getUserSpecifiedDuration () const
{
    return _duration;
}
