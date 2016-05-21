#include <algorithm>
#include <getopt.h>
#include <PolySyncDataModel.hpp>

#include "ApplicationInputHandler.hpp"


using namespace std;

namespace polysync
{

int ApplicationInputHandler::getFlagIndex( const char optret )
{
    for( auto index = 0; index < _optionInputFlags.size(); ++index )
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

    while ( ( optionArgumentIndex = getopt( argc, argv, "t:o:f:hH") ) != -1 )
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
                        _filteredForMessagesFlag = true;

                        _filteredMessageNames.emplace_back( optarg );
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

            case 't':

                if( ( *argv[ optind - 1 ] )
                     && ( *argv[ optind - 1 ] == '-' ) )
                {
                    cout <<"\n\nInvalid usage for option -t run for specific time:\n"
                         <<"-t should be followed by how long you want Echo to run,"
                         << " not by another -option.\n"
                         <<"A usage guide follows." << endl;

                    _getOptHelpFlag = true;
                }
                else if( ( *argv[ optind - 1 ] )
                     && isalpha(  *argv[ optind - 1 ] ) )
                {
                    cout <<"\n\nInvalid usage for option -t run for specific time:\n"
                         << "-t should be followed by a number"
                         <<" representing  how long you want Echo to run.\n"
                         <<"A usage guide follows." << endl;

                    _getOptHelpFlag = true;
                }
                else
                {
                    _echoRunTime =  stoull( optarg );

                    _runTimeSpecifiedFlag = true;
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
                    else if( optopt == 't' )
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


std::vector < std::string > ApplicationInputHandler::getFilteredMessageNames() const
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


bool ApplicationInputHandler::wasRunTimeSpecified () const
{
    return _runTimeSpecifiedFlag;
}


unsigned long long ApplicationInputHandler::getUserRunTime () const
{
    return _echoRunTime;
}


// END polysync::PolySyncGetOpt class


} // end namespace polysync
