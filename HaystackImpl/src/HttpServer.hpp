//
//  HttpServer.hpp
//  src
//
//  Created by Vishwa on 20/11/2016.
//
//

#include <boost/network/protocol/http/server.hpp>
#include <string>
#include <iostream>
using namespace std;
namespace http = boost::network::http;

struct Store_Handler;
struct Directory_Handler;
typedef http::server<Store_Handler> Store_Server;
typedef http::server<Directory_Handler> Directory_Server;

struct Store_Handler {
    void operator() (Store_Server::request const &request,Store_Server::response &response);
    void log(const char* errCode);
  };

struct Directory_Handler {
      void operator() (Directory_Server::request const &request,Directory_Server::response &response) ;
      void log(const char* errCode);
    };
