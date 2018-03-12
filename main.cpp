/// @file     homework2.cpp
/// @brief    using some C++11 features
/// @author   Daria Moroz
/// @date     10-Mar-2018

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

static const char* ipDelimiter = ".";
static const int ipPartsNumber = 4;

typedef unsigned int uint32;

static uint32 formatIp( const std::string& str )
{
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of( ipDelimiter );

    uint32 ip = 0;
    for( int i = ipPartsNumber - 1; i >= 0; --i )
    {
        ip += stoi( str.substr( start, stop - start ) ) << i * 8;

        start = stop + 1;
        stop = str.find_first_of( ipDelimiter, start );
    }

    return ip;
}

static std::string toString( uint32 ip )
{
    std::string ipString;
    for( int i = ipPartsNumber - 1; i >= 0; --i )
    {
        ipString += std::to_string( ( ip >> i * 8 ) & 0xFF );
        ipString += i ? ipDelimiter : "\n";
    }

    return ipString;
}

static void printIps( const std::vector< uint32 >& ips )
{
    std::for_each( ips.begin(), ips.end(),
        []( uint32 ip )
        {
            std::cout << toString( ip );
        }
    );
}

static void filterByFirstByteAndOutput( const std::vector< uint32 >& ips )
{
    std::vector< uint32 > filteredIps;
    auto it = std::copy_if( ips.cbegin(), ips.cend(), std::back_inserter( filteredIps ),
        []( uint32 ip )
        {
            return ( ip >> 24 ) == 0x01;
        }
    );
    printIps( filteredIps );
}

static void filterByFirstAndSecondByteAndOutput( const std::vector< uint32 >& ips )
{
    std::for_each( ips.begin(), ips.end(),
        []( uint32 ip )
        {
            if( ( ip >> 24 ) == 0x2E && ( ( ip >> 16 ) & 0xFF ) == 0x46 )
            {
                std::cout << toString( ip );
            }
        }
    );
}

static void filterByAnyByteAndOutput( const std::vector< uint32 >& ips )
{
    auto filter = []( uint32& ip )
    {
        for( int i = ipPartsNumber - 1; i >= 0; --i )
        {
            if( ( ( ip >> i * 8 ) & 0xFF ) == 0x2E )
            {
                return true;
            }
        }
        return false;
    };

    for( uint32 ip : ips )
    {
        if( filter( ip ) )
        {
            std::cout << toString( ip );
        }
    }
}

int main( int argc, char const *argv[] )
{
    try
    {
        std::vector< uint32 > ips;
        for( std::string line; std::getline( std::cin, line ); )
        {
            ips.push_back( formatIp( line ) );
        }

        // reverse lexicographically sort
        std::sort( ips.rbegin(), ips.rend() );
        printIps( ips );

        filterByFirstByteAndOutput( ips );
        filterByFirstAndSecondByteAndOutput( ips );
        filterByAnyByteAndOutput( ips );
    }
    catch( const std::exception &e )
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}