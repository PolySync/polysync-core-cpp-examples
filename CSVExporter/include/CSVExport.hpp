#ifndef CSVEXPORT_HPP
#define CSVEXPORT_HPP

#include <ostream> /* std */
#include <thread>
#include <vector>
#include <algorithm>
#include <cassert>

#include <PolySyncNode.hpp> /* PolySync files */
#include <PolySyncDataModel.hpp>
#include <GetOpt.hpp>

#include "SubscriberMap.hpp" /* Local files */

/**
 *class Parameters
 * This class is used to pass parameters around for command line arguments.
 */
class Parameters
{
public:
    
    std::vector<std::string> messageType;
};

/**
 * @brief This function is to take in command line arguments and handle them.
 * Arguments go to two locations, the Parameters class used in main and
 * directly set in the TypeMap which is used through out main.
 * 
 * @param getOpt [in] command line arguments from the GetOpt library.
 * 
 * @param setType [out] the TypeMap where it's parameters are set.
 * 
 * @param setParameters [out] the Parameters class used for main parameters.
 */
int handleOptions( 
        GetOpt && getOpt, 
        Parameters * setParameters );

/**
 * @brief This function prints out the command line options.
 */
void printHelp();

/**
 * @brief This function prints out an error message for when the Message type 
 * has not been passed in as an argument.
 */
void printMissingMessageType();

#endif // CSVEXPORT_HPP

