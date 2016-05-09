#include "PolySyncGetOpt.hpp"

using namespace std;
using namespace polysync;

PolySyncGetOpt::PolySyncGetOpt()
{

}


bool PolySyncGetOpt::wasSingleMsgFiltered()
{
    return _filteredForSingleMsgFlag;
}


bool PolySyncGetOpt::wereHeadersRequested()
{
    return _echoMessageHeadersOnlyFlag;
}


bool PolySyncGetOpt::wasFileSpecified()
{
    return _echoMessageToFileFlag;
}


bool PolySyncGetOpt::wasHelpRequested()
{
    return _echoHelpFlag;
}
