#include <iostream>
#include <string>
#include "Client.h"
#include "Connection.h"
#include "WorldData.h"
#include "BasicPlayer.h"
#include "ClientException.h"
#include "ClientConnector.h"
#include "PropertyStreamError.h"
#include "BadPropertyFormatException.h"
#include "AgentList.h"
#include "fstream"
#include "Vector.h"
#include "Rectangle.h"
#include "Plans.h"
#include <cstdio>
#include <cstdlib>
#include <Types.h>
#include <Shoot-cir.h>
#include <Shoot-gpu.h>
#include <Pass.h>
#include <Clear.h>
#include <Clear-gpu.h>
#include <PenaltyPlan.h>

using namespace std;

Properties read_file_properties(const std::string& filename)
{
    PropertyReader fileReader;
    Properties props;

    try
    {
        /**
         * Read the file, if we can't read it just let the flow
         * throw an exception
         */
        props = fileReader.read(filename);
        return props;

    }
    catch (PropertyStreamError& ex)
    {
        std::cout << "Error reading property file" << std::endl;
    }
    catch (BadPropertyFormatException& ex)
    {
        std::cout << "Error in property format: " << ex.what() << std::endl;
    }

    throw ClientException("Could not read properties file");
}

/*
 *
 * Returns true if the program is supposed to continue
 * false otherwise
 */

bool override_properties_with_cmd(int argc, char ** argv, Properties& props)
{

    if (argc == 1)
    {
        // No properties to read
        return true;
    }
    else if (argc == 2)
    {
        std::string first_arg = argv[1];

        if (first_arg == "-help")
        {
            std::cout << "Available agents parameters: \n"
                    << " -p(ort team) - The team's port\n"
                    << " -n(ick name) - The players nickname\n"
                    << " -r(cID) The agent's robot control id\n"
                    << " -v(tID) the agent's robot control id"
                    << " -t(eamname) the agent team name";

            return false;
        }
        else
        {
            std::cout << "Invalid argument list" << std::endl;
            return false;
        }
    }
    else
    {
        for (int i = 1; i < argc; i += 2)
        {

            /*
             * A parameter is always followed by a value!
             * So read it by pairs
             *
             */
            if (i + 1 >= argc)
            {
                cout << "Missing argument for value " << argv[i] << endl;
                return false;
            }

            std::string arg = argv[i];
            std::string value = argv[i + 1];

            if (arg[0] == '-' && arg.size() > 1)
            {
                try
                {
                    switch (arg[1])
                    {
                        case 'p':
                        {
                            convert_to<int> (value);
                            props.setProperty("PORT_TEAM", value);
                        }
                            break;

                        case 'r':
                        {
                            convert_to<int> (value);
                            props.setProperty("RC_ID", value);
                        }
                            break;

                        case 'v':
                        {
                            convert_to<int> (value);
                            props.setProperty("VT_ID", value);
                        }
                            break;

                        case 't':
                        {
                            props.setProperty("TEAMNAME", value);
                        }
                            break;

                        case 'n':
                        {
                            props.setProperty("NICK_NAME", value);
                        }
                            break;

                        default:
                            cerr << "Error bad argument \"" << arg << "\"" << endl;
                            return false;
                    }
                }
                catch (std::exception& ex)
                {
                    std::cout << "Invalid argument exception: " << ex.what()
                            << "\n";
                    return false;
                }
            }
            else
            {
                std::cout << "Invalid argument" << std::endl;
            }
        }
    }

    return true;
}

mrTeam client_team(const ServerProperties& props, int port)
{
    if (props.GetInt("port_team_yellow") == port)
    {
        return TEAM_YELLOW;
    }
    else if (props.GetInt("port_team_blue") == port)
    {
        return TEAM_BLUE;
    }
    else
    {
        throw ClientException("The connected port is invalid (" + convert_to<
                std::string > (port) + ")");
    }
}

int main(int argc, char* argv[])
{
    Properties props;
    std::string host;
    int port;

    try
    {
        /**
         * Read the properties from the configuration files
         */
        props = read_file_properties("config.ini");

        /**
         * If necessary override those properties with any given command-line
         */
        if (!override_properties_with_cmd(argc, argv, props))
        {
            return -1;
        }

        host = props.propertyValue("SERVER_IP"); //ip server ro mide      => propertYValue string mo
        port = convert_to<int> (props.propertyValue("PORT_TEAM")); //port server ro mide

    }
    catch (ClientException& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    ConnectionParameters params;
    params.nickname(props.Get("NICK_NAME"));
    params.teamname(props.Get("TEAMNAME"));
    params.vtId(convert_to<int> (props.Get("VT_ID")));
    params.rcId(convert_to<int> (props.Get("RC_ID")));
    params.movementMode("wheel_velocities");

    int tries = convert_to<int> (props.Get("TRIES"));
    int sleep = convert_to<int> (props.Get("SLEEP"));

    ServerProperties server_props;

    ClientConnector client_connector(host, port);
    client_connector.tries(tries);
    client_connector.retryTime(sleep);

    try
    {
        server_props = client_connector.ConnectToServer(params);
    }
    catch (ClientException& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    mrTeam my_team = client_team(server_props, port);

    WorldData gWorldData(sleep, tries, my_team); /* sleep : meghdar zamani ke ta update beshe bazikon sleep bashe*/
    gWorldData.setConnection(client_connector.getConnection());
    std::cout << "World connection successfully initialized." << std::endl;

    BasicPlayer gPlayer(my_team);
    gPlayer.setConnection(client_connector.getConnection());
    std::cout << "Basic player connection successfully initialized." << std::endl;

    gPlayer.setWorldData(&gWorldData);

    bool quit = false;

    gWorldData.mrRoles[0] = 1 ;

    while (!quit)
    {
        if (gWorldData.updateAll())
        { //avalle har cycle world datamoon ro update mikonim
            if (gWorldData.is_game_on()) //agar bazi dar hale ejra bood
            {
                //				std::cout << "Game is on." << std::endl;
            }
            else if (gWorldData.mrPlayMode() == "warn ending") //age bazi nazdike payad bood
            {
                std::cout << "game near to the end." << std::endl;
            }
            else if (gWorldData.mrPlayMode() == "time over")
            { //age bazi tamoom shode bood
                quit = true; //az bazi kharej mishim
                continue;
            }
            //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Main Cycle>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            if (!gWorldData.i_am_lost())
            {
                vector<int> Opps = gWorldData.mr_found_opponents();
                for (int i = 0; i < Opps.size(); i++)
                    gWorldData.oppPositions[Opps[i] % 5] = gWorldData.basedCenter(gWorldData.opponent(Opps[i]));
                
                vector<int> teammates = gWorldData.mr_found_teammates();
                for (int i = 0; i < teammates.size(); i++)
                    gWorldData.tmmPositions[teammates[i] % 5] = gWorldData.basedCenter(gWorldData.teammate(teammates[i]));


                //there is no need to do this each cycle they are only needed once a half (of the play time)
                gWorldData.oppPole1 = gWorldData.basedCenter(gWorldData.opgoal_pole1());
                gWorldData.oppPole2 = gWorldData.basedCenter(gWorldData.opgoal_pole2());
                gWorldData.ourPole1 = gWorldData.basedCenter(gWorldData.mygoal_pole1());
                gWorldData.ourPole2 = gWorldData.basedCenter(gWorldData.mygoal_pole2());
                
               
                if (gPlayer.can_kick())
                {
                    gpuClear testClear(&gWorldData , &gPlayer);
                    testClear.gpuClearNow();
                }
                else
                {

                }


                /*
                This is when the commands decided above will be actually
                sent over the network. This function should be called
                as only one time per cycle in the end of the main loop
                 */
                gPlayer.flush(); //commande set shode ro be server mifreste
            }
            else
            {
                // Just for the sake of clarity
                // std::cout << "Where I'am?!" << std::endl;
            }
        }
        else
        {
            std::cout << "Didn't receive any update message from server."
                    << std::endl;
        }
    }

    std::cout << "Ending program." << std::endl;
    exit(EXIT_SUCCESS);
}
