/**
 * @file PolySyncEchoHelp.hpp
 * @brief Help Class to augment Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef POLYSYNCECHOHELP_HPP
#define POLYSYNCECHOHELP_HPP

#include <PolySyncDataModel.hpp>

/**
 * @namespace polysync
 */
namespace polysync
{

/**
 * @brief The PolySyncEchoHelp class is a user interaction class to assist
 *        end users with command line arguments/options for Echo utility.
 *
 */
class PolySyncEchoHelp
{
public:
    PolySyncEchoHelp();

    /**
     * @brief Get strings for help page display of cmd line flags.
     * @return std::vector< std::string > - variable contains help descriptions.
     *
     */
    std::vector< std::string > getHelpFlags();

    /**
     * @brief Get strings for help page display of descriptions for cmd line flags.
     * @return std::vector< std::string > - variable contains help descriptions.
     *
     */
    std::vector< std::string > getHelpDescriptions();

    /**
     * @brief Print strings for help page display, append to end of list.
     * @param std::vector< std:: string > - variable containing help string.
     */
    void printHelp( const std::vector< std::string > & helpFlags,
                    const std::vector< std::string > & helpDescriptions);

    /**
     * @brief Wrapper for caller class access to printHelp().
     */
    void printEchoHelp();

};

} /*!< end namespace polysync */

#endif // POLYSYNCECHOHELP_HPP
