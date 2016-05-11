#include "ApplicationInputHandler.hpp"
#include <algorithm>

using namespace std;

namespace polysync
{

int ApplicationInputHandler::getOptIdx( const char optret )
{
    if( std::find
            ( std::begin ( _optChars ),
              std::end ( _optChars ),
              optret )
            != std::end ( _optChars ) )
    {
         // success: container end not reached.
    }
    else
    {
        return -1;
    }
}


bool ApplicationInputHandler::optionsParse(const int argc, char *argv[])
{
    bool parsedOptSuccess = true;

    int option_idx = -1;
    int optret = 0;

    // reset scanner
    optind = 0;

    int fFlag = 0;
    int oFlag = 0;
    int hFlag = 0;
    int HFlag = 0;
    int idx;

    opterr = 0;

    while ( (optret = getopt( argc, argv, "o:f:hH")) != -1 )
    {
        option_idx = getOptIdx( (const char) optret );

        if ( option_idx == -1 )
        {
            cout << "\n\nUsage: invalid options. Usage guide follows." <<endl;

            _getOptHelpFlag = true;
        }

        else if ( option_idx != -1 )
        {
            switch ( optret )
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

/*
char * ApplicationInputHandler::getMessageName() const
{
    return _messageName;
}


char * ApplicationInputHandler::getFileName() const
{
    return _userFileName;
}
*/


std::string ApplicationInputHandler::getMessageName() const
{
    return _messageName;
}


std::string ApplicationInputHandler::getFileName() const
{
    return _userFileName;
}


bool ApplicationInputHandler::wasSingleMessageFiltered() const
{
    return _filteredForSingleMessageFlag;
}


bool ApplicationInputHandler::wereHeadersRequested() const
{
    return _echoMessageHeadersOnlyFlag;
}


bool ApplicationInputHandler::wasFileSpecified() const
{
    return _echoMessageToFileFlag;
}


bool ApplicationInputHandler::wasHelpRequested() const
{
    return _getOptHelpFlag;
}


// END polysync::PolySyncGetOpt class


} // end namespace polysync
