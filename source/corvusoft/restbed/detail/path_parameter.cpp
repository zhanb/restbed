/*
 * Site: restbed.corvusoft.co.uk
 * Author: Ben Crowhurst
 *
 * Copyright (c) 2013 Restbed Core Development Team and Community Contributors
 *
 * This file is part of Restbed.
 *
 * Restbed is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Restbed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with Restbed.  If not, see <http://www.gnu.org/licenses/>.
 */

//System Includes
#include <vector>
#include <stdexcept>

//Project Includes
#include "restbed/detail/path_parameter.h"
#include "restbed/detail/helpers/string.h"

//External Includes

//System Namespaces
using std::map;
using std::pair;
using std::regex;
using std::string;
using std::vector;
using std::smatch;
using std::make_pair;
using std::regex_search;
using std::invalid_argument;

//Project Namespaces
using restbed::detail::helpers::String;

//External Namespaces

namespace restbed
{
    namespace detail
    {
        regex PathParameter::parse( const string& declaration )
        {
            auto definition = parse_declaration( declaration );

            return regex( definition.second );  
        }

        map< string, string > PathParameter::parse( const string& path, const string& declaration )
        {
            map< string, string > parameters;

            auto declarations = String::split( declaration, '/' );

            auto paths = String::split( path, '/' );

            for ( vector< string >::size_type index = 0; index not_eq declarations.size( ); index++ )
            {
                auto definition = parse_declaration( declarations[ index ] );  

                smatch match;

                if ( not definition.first.empty( ) and regex_search( paths[ index ], match, regex( definition.second ) ) )
                {
                    parameters[ definition.first ] = definition.second;
                }
            }

            return parameters;
        }

        pair< string, string > PathParameter::parse_declaration( const string& declaration )
        {
            string name = String::empty;
            string pattern = ".*";
            string definition = declaration;

            if ( definition.front( ) == '{' )
            {
                if ( definition.back( ) == '}' )
                { 
                    definition = String::trim( definition, "{" );
                    definition = String::trim( definition, "}" ); 
                    
                    auto segments = String::split( definition, ':' );

                    if ( segments.size( ) not_eq 2 )
                    {
                        throw invalid_argument( String::empty );
                    }
                    
                    name = String::trim( segments[ 0 ] );
                    pattern = String::trim( segments[ 1 ] );
                }
            }

            return make_pair( name, pattern );
        }
    }
}
