/**
 * @file EchoHelp.hpp
 * @brief Help Class to augment Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef ECHOHELP_HPP
#define ECHOHELP_HPP


/**
 * @brief The EchoHelp class is a user interaction class to assist
 *        end users with command line arguments/options for Echo utility.
 *
 */
class EchoHelp
{
public:

    /**
     * @brief Get strings for help page display of cmd line flags.
     * @return std::vector< std::string > - variable contains help descriptions.
     *
     */
    std::vector< std::string > getHelpFlags() const;

    /**
     * @brief Get strings for help page display of descriptions for cmd line flags.
     * @return std::vector< std::string > - variable contains help descriptions.
     *
     */
    std::vector< std::string > getHelpDescriptions() const;

    /**
     * @brief Print strings for help page display, append to end of list.
     * @param std::vector< std:: string > - variable containing help string.
     */
    void printHelp(
            const std::vector< std::string > & helpFlags,
            const std::vector< std::string > & helpDescriptions) const;

    /**
     * @brief Wrapper for caller class access to printHelp().
     */
    void printEchoHelp() const;

};


#endif // ECHOHELP_HPP
