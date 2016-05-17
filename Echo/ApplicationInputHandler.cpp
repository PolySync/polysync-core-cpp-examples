#include <algorithm>
#include <getopt.h>
#include <PolySyncDataModel.hpp>

#include "ApplicationInputHandler.hpp"

using namespace std;

namespace polysync
{

int ApplicationInputHandler::getFlagIndex( const char optret )
{
    for ( auto index = 0; index < _optionInputFlags.size(); ++index )
    {
        if( _optionInputFlags[ index ] == optret )
        {
            return index;
        }
    }
    return -1;
}


bool ApplicationInputHandler::optionsParse( const int argc, char *argv[] )
{
    bool parsedOptSuccess = true;

    int optionIndex = -1;

    int optionArgumentIndex = 0;

    // reset scanner
    optind = 0;

    int index;

    opterr = 0;

    int messageFilterCounter = 0;

    while ( ( optionArgumentIndex = getopt( argc, argv, "o:f:hH") ) != -1 )
    {
        optionIndex = getFlagIndex( (const char) optionArgumentIndex );

        if( optionIndex == -1 )
        {
            cout << "\n\nUsage: invalid options. Usage guide follows." <<endl;

            _getOptHelpFlag = true;
        }
        else if( optionIndex != -1 )
        {
            switch( optionArgumentIndex )
            {
                case 'f':

                    if( ( *argv[ optind - 1 ] )
                         && ( *argv[ optind - 1 ] == '-' ) )
                    {
                        cout <<"\n\nInvalid usage for option -f filter single message type:\n"
                             <<"-f should be followed by a PolySync message type,"
                             << " not by another -option.\n"
                             <<"A usage guide follows." << endl;

                        _getOptHelpFlag = true;
                    }
                    else
                    {
                        if( messageFilterCounter == 0 )
                        {
                            _messageName = optarg;

                            _filteredForSingleMessageFlag = true;

                            ++messageFilterCounter;
                        }
                        else
                        {
                            _filteredForMultipleMessagesFlag = true;

                            _multipleFilteredMessageNames.emplace_back( optarg );
                        }
                    }

                break;

                case 'o':

                    if( ( *argv[ optind - 1 ] )
                         && ( *argv[ optind - 1 ] == '-' ) )
                    {
                        cout <<"\n\nInvalid usage for option -o external file:\n"
                             <<"-o should be followed by a filename yourfile.txt,"
                             << " not by another -option.\n"
                             <<"A usage guide follows." << endl;

                        _getOptHelpFlag = true;
                    }
                    else
                    {
                        _userFileName = optarg;

                        _echoMessageToFileFlag = true;
                    }

                    break;

                case 'h':

                    _getOptHelpFlag = true;

                break;

                case 'H':

                    _echoMessageHeadersOnlyFlag = true;

                break;

                case '?':

                    if( optopt == 'f' )
                    {
                        _getOptHelpFlag = true;

                        parsedOptSuccess = false;
                    }
                    else if( optopt == 'o' )
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
        cout << "\n\nUsage: Non option argument: " << argv[ index ] <<endl
             << "A usage guide follows." <<endl;

        _getOptHelpFlag = true;
    }

    return parsedOptSuccess;
}


std::string ApplicationInputHandler::getSingleMessageName() const
{
    return _messageName;
}


std::vector < std::string > ApplicationInputHandler::getMultipleMessageNames() const
{
    return _multipleFilteredMessageNames;
}


std::string ApplicationInputHandler::getFileName() const
{
    return _userFileName;
}


bool ApplicationInputHandler::singleMessageWasFiltered() const
{
    return _filteredForSingleMessageFlag;
}


bool ApplicationInputHandler::multipleMessagesWereFiltered() const
{
    return _filteredForMultipleMessagesFlag;
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


// END polysync::PolySyncGetOpt class


} // end namespace polysync
