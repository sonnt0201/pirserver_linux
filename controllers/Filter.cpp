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


        return 1;
    }

    if (request.method() == GET && request.path() == "/api/range")
    {
        std::string sBegin = request.value("begin");
        std::string sEnd = request.value("end");
        if (sBegin == "" || sEnd == "")
        {
            std::cout << "null \n";
            return 0;
        }
        if (sBegin[0] == '0' || sEnd[0] == '0')
        {
            std::cout << "Invalid number in request params. \n";
            return 0;
        }
        for (long i = 0; i < sBegin.length(); i++)
            if (sBegin[i] < '0' || sBegin[i] > '9')
                return 0;
        for (long i = 0; i < sEnd.length(); i++)
            if (sEnd[i] < '0' || sEnd[i] > '9')
                return 0;

        int iBegin = std::stoi(sBegin);
        int iEnd = std::stoi(sEnd);

        if (iBegin > iEnd)
            return 0;
        return 1;
    }

    // Default:
    return true;
}