//
//  HttpServer.cpp
//  src
//
//  Created by  on 20/11/2016.
//
//

#include "HttpServer.hpp"
#include "Haystack_Store.hpp"
#include "HayStack_Directory.h"
#include <json/json.h>
using namespace Json;
string dataPath;
void Store_Handler::operator() (Store_Server::request const &request,
                Store_Server::connection_ptr connection) {
    std::string ip = source(request);
    std::string strMethod = request.method;

  	HaystackStore* store = HaystackStore::getInstance();


    if(strMethod =="GET")
    {

      std::string token = request.destination.substr (1,request.destination.length()-1);
      int delPos = token.find(",");
      std::string Vid = token.substr(0,delPos);
      std::string ImageID = token.substr(delPos+1,token.length()-1);
      char * data = store->Read(ImageID);
      connection->set_status(Store_Server::connection::ok);
    /*  Store_Server::response_header content_type;
      content_type.name = "Content-Type";
      content_type.value = "application/octet-stream";
      connection->set_headers(content_type);*/
       std::string strdata(data);

      connection->write(strdata);

    }

    else if(strMethod == "PUT")
    {
      store->setDataPath(dataPath);
      std::string body = request.body;
      Reader reader;
      Value root;
      const char* begin = body.c_str();
      const char* end = begin + body.length();
      bool isSuccessful =   reader.parse(begin,end,root);
      std::string token = root.get("token", "empty" ).asString();
      int delPos = token.find(",");
      std::string Vid = token.substr(0,delPos);
      std::string ImageID = token.substr(delPos+1,token.length()-1);
      std::string image = root.get("image", "empty" ).asString();
      if(image=="empty")
      {
             connection->set_status(Store_Server::connection::bad_request);
              connection->write("Invalid request structure");

      }

      store->write(Vid,ImageID,image.c_str(),image.length());
      connection->set_status(Store_Server::connection::ok);
     /* Store_Server::response_header content_type;
      content_type.name = "Content-Type";
      content_type.value = "application/octet-stream";
      connection->set_headers(content_type);*/
      connection->write("write successful!!!");
    }


}
void Store_Handler::log(const char* errCode)
{
  std::cerr<<"Error code:"<<errCode << "\n";
}

void Directory_Handler::operator() (Directory_Server::request const &request,
                 Directory_Server::response &response) {
    std::string ip = source(request);
    std::string strMethod = request.method;
    std::string ImageID = request.destination.substr (1,request.destination.length()-1);

    Directory *dir = Directory::getInstance();

    if(strMethod =="GET")
    {
        std::string reply = dir->GetTokenToRead(ImageID);
        response = Directory_Server::response::stock_reply(
            Directory_Server::response::ok, reply );
    }

    else if(strMethod == "PUT")
    {
      std::string body = request.body;
      Reader reader;
      Value root;
      const char* begin = body.c_str();
      const char* end = begin + body.length();
      bool isSuccessful =   reader.parse(begin,end,root);

      std::string token = root.get("token", "empty" ).asString();
      std::string size = root.get("blobSize", "empty" ).asString();
      unsigned long blobSize = strtoul (size.c_str(), NULL, 0);
      std::string reply = dir->AssignTokenToWrite(token,blobSize);
      response = Directory_Server::response::stock_reply(Directory_Server::response::ok, reply);
    //  store->write(1,1,data,sz);
    }


}
void Directory_Handler::log(const char* errCode)
{
  std::cerr<<"Error code:"<<errCode << "\n";
}

int main(int argc, char * argv[]) {

    if (argc < 4) {
        std::cerr << "Type,address,port,dataPath(for store)" << std::endl;
        return 1;
    }

    try {

      if(strcmp(argv[1],"store")==0)
      {
        Store_Handler store_handler;
        boost::network::utils::thread_pool thread_pool1(2);

        dataPath =  std::string(argv[4]);
        Store_Server server_(argv[2], argv[3], store_handler,thread_pool1);
        cout <<"Store listening...\n";
        server_.run();

      }
      else
      {
        Directory_Handler dir_handler;
        Directory_Server server_(argv[2], argv[3], dir_handler);
        cout <<"Directory listening...\n";
        server_.run();


      }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
