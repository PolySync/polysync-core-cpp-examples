#include <algorithm>
#include <getopt.h>
#include <PolySyncDataModel.hpp>

#include "ApplicationInputHandler.hpp"


ApplicationInputHandler::ApplicationInputHandler()
    :
    _userFileName( {} ),
    _activeMessagesFlag( false ),
    _filteredForMessagesFlag( false ),
    _echoMessageHeadersOnlyFlag( false ),
    _echoMessageToFileFlag( false ),
    _getOptHelpFlag( false ),
    _durationSpecifiedFlag( false ),
    _optionInputFlags( { 'f', 'h', 'H', 'o', 't', 'a' } ),
    _filteredMessageNames( {} )
{
    // empty
}

int ApplicationInputHandler::getFlagIndex( const char optret )
{
    for( auto index = 0U; index < _optionInputFlags.size(); ++index )
    {
        if( _optionInputFlags[ index ] == optret )
        {
            return index;
        }
    }

    return -1;
}


bool ApplicationInputHandler::optionsParse( const int argc, char * argv[] )
{
    bool parsedOptSuccess = true;

    int optionIndex = -1;

    int optionArgumentIndex = 0;

    // reset scanner
    optind = 0;

    int index;

    opterr = 0;

    while ( ( optionArgumentIndex =
              getopt( argc, argv, "t:o:f:hH:a") ) != -1 )
    {
        optionIndex =
                getFlagIndex(
                    static_cast< const char >( optionArgumentIndex ) );

        if( optionIndex == -1 )
        {
            std::cout << std::endl << std::endl
                 << "Usage: invalid options. Usage guide follows."
                 << std::endl;

            _getOptHelpFlag = true;
        }
        else if( optionIndex != -1 )
        {
            switch( optionArgumentIndex )
            {
                case 'a':

                    std::cout << "a flag" << std::endl;

                    _activeMessagesFlag = true;

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

                case '?':

                    if( ( optopt == 'f' ) or
                        ( optopt == 'o' ) or
                        ( optopt == 't' ) or
                        ( optopt == 'a' ) )
                    {
                        _getOptHelpFlag = true;

                        parsedOptSuccess = false;
                    }

                default:

                    parsedOptSuccess = true;
            }
        }
    }

    for( index = optind; index < argc; ++index )
    {
        std::cout << std::endl << std::endl
             << "Usage: Non option argument: "
             << argv[ index ]
             << std::endl
             << "A usage guide follows."
             << std::endl;

        _getOptHelpFlag = true;
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


unsigned long long ApplicationInputHandler::getUserSpecifiedDuration () const
{
    return _duration;
}
