#include "Filter.hpp"

bool filter(Request request)
{

    // request
    if (request.method() == POST)
    {
        //    std::cout<<request.getText()<<"\n";
        // filter goes here
        std::string voltageStr = request.value("vol"),
                    idStr = request.value("esp-id"),
                    timeStr = request.value("time");

        // std::cout << "vol=" << voltageStr << "\n"
        //           << "esp-id=" << idStr << "\n"
        //           << "timeStr=" << timeStr << "\n";

        // check empty string
        if (voltageStr == "" || idStr == "" || timeStr == "")
        {
            std::cout << "null \n";
            return 0;
        }

        // check unsigned
        for (char c : idStr)
            if (c < '0' || c > '9')
                return 0;
        for (char c : timeStr)
            if (c < '0' || c > '9')
                return 0;

        // std::cout<<voltageStr<<std::endl;
        int count = 0;
        for (long i = 0; i < voltageStr.length(); i++)
        {

            // check number and '+'
            if ((voltageStr[i] < '0' || voltageStr[i] > '9') && (voltageStr[i] != SEPARATOR))
            {
                return 0;
            }

            if (voltageStr[i] != SEPARATOR)
                count++;
            else
                count = 0;

            if (count > 4)
                return 0;
        }
    }

    if (request.method() == GET && request.path() == "/api")
    {
        // Check valid number
        std::string strBegin = request.value("begin"), strEnd = request.value("end");
        // std::cout << "begin: " << stringToUInt(strBegin)
        //           << std::endl
        //           << "end: " << stringToUInt(strEnd) << "\n";
      
       
        long int begin = stringToUInt(strBegin), end;

        if (strEnd == "") end = time(NULL); else end = stringToUInt(strEnd);

        if ( begin == -1 || end == -1 || begin > end) {
            // std::cout<<"Filter: Bad request !\n";
            return false;}


        return true;
    }

    
    if (request.method() == GET && request.path() == "/api/range") {

        if (request.value("begin")== "" && request.value("end") == "")return false;
        if (request.value("begin") != "" && request.value("end") != "")return false;

        if (
           request.value("range") != "" 
           && stringToUInt(request.value("range")) == -1
        ) return false;

        if (
           request.value("begin") != "" 
           && stringToUInt(request.value("begin")) == -1
        ) return false;

        if (
           request.value("end") != "" 
           && stringToUInt(request.value("end")) == -1
        ) return false;

        // if (request.value("end"))
        return true;

    }

    if (request.method() == DEL && request.path() == "/range") {
         if (request.value("begin")== "" || request.value("end") == "" || request.value("esp-id") == "")return false;
         int begin = stringToUInt(request.value("begin")),
            end = stringToUInt(request.value("end")),
            espID = stringToUInt(request.value("esp-id"));
        
        if (begin == -1 || end == -1 || espID == -1 ) return false;
        if (begin > end )return false;
        return true;
    }

    // Default:
    return true;
}