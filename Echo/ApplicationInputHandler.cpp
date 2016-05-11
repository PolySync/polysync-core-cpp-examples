#include "PolySyncDataModel.hpp"
#include "ApplicationInputHandler.hpp"
#include "getopt.h"
#include <algorithm>

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



bool ApplicationInputHandler::optionsParse(const int argc, char *argv[])
{
    bool parsedOptSuccess = true;

    int option_idx = -1;

    int optionArgumentIndex = 0;

    // reset scanner
    optind = 0;

    int fFlag = 0;
    int oFlag = 0;
    int hFlag = 0;
    int HFlag = 0;
    int idx;

    opterr = 0;

    while ( (optionArgumentIndex = getopt( argc, argv, "o:f:hH")) != -1 )
    {
        option_idx = getFlagIndex( (const char) optionArgumentIndex );

        if ( option_idx == -1 )
        {
            cout << "\n\nUsage: invalid options. Usage guide follows." <<endl;

            _getOptHelpFlag = true;
        }

        else if ( option_idx != -1 )
        {
            switch ( optionArgumentIndex )
            {

            case 'f':

                if ( ( *argv[ optind - 1 ] )
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
                    fFlag = 1;

                    _messageName = optarg;

                    _filteredForSingleMessageFlag = true;

                    parsedOptSuccess = true;
                }
                break;

            case 'o':

                if ( ( *argv[ optind - 1 ] )
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
                    oFlag = 1;

                    _userFileName = optarg;

                    _echoMessageToFileFlag = true;

                    parsedOptSuccess = true;
                }
                break;

            case 'h':

                hFlag = 1;

                _getOptHelpFlag = true;

                parsedOptSuccess = true;

                break;

            case 'H':

                HFlag = 1;

                _echoMessageHeadersOnlyFlag = true;

                parsedOptSuccess = true;

                break;

            case '?':

                if ( optopt == 'f' )
                {
                    fFlag = 0;

                    _getOptHelpFlag = true;

                    parsedOptSuccess = false;
                }

                else if ( optopt == 'o' )
                {
                    oFlag = 0;

                    _getOptHelpFlag = true;

                    parsedOptSuccess = false;
                }

            default:

                parsedOptSuccess = true;
            }
        }
    }

    for ( idx = optind; idx < argc; ++idx )
    {
        cout << "\n\nUsage: Non option argument: " << argv[ idx ] <<endl
             << "A usage guide follows." <<endl;

        _getOptHelpFlag = true;
    }

    return parsedOptSuccess;
}


std::string ApplicationInputHandler::getMessageName() const
{
    return _messageName;
}


std::string ApplicationInputHandler::getFileName() const
{
    return _userFileName;
}


bool ApplicationInputHandler::singleMessageWasFiltered() const
{
    return _filteredForSingleMessageFlag;
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
