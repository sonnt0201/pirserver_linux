#include "Filter.hpp"

bool v1(Request request);
bool v2(Request request);

bool filter(Request request)
{
    // priority to newest version;
    if (v2(request))
        return true;
    if (v1(request))
        return true;

    return false;
};

bool v2(Request request)
{

    if (request.method() == POST && request.path() == "/api/v2/video") {

        return true;
    }

    if (request.method() == GET && request.path() == "/api/v2")
    {
        // Check valid number
        std::string strBegin = request.value("begin"), strEnd = request.value("end");
        // std::cout << "begin: " << stringToUInt(strBegin)
        //           << std::endl
        //           << "end: " << stringToUInt(strEnd) << "\n";

        long int begin = stringToUInt(strBegin), end;

        if (strEnd == "")
            end = time(NULL);
        else
            end = stringToUInt(strEnd);

        if (begin == -1 || end == -1 || begin > end)
        {
            // std::cout<<"Filter: Bad request !\n";
            return false;
        }

        return true;
    }

    if (request.method() == GET && request.path() == "/api/v2/range")
    {

        if (request.value("begin") == "" && request.value("end") == "")
            return false;
        if (request.value("begin") != "" && request.value("end") != "")
            return false;

        if (
            request.value("range") != "" && stringToUInt(request.value("range")) == -1)
            return false;

        if (
            request.value("begin") != "" && stringToUInt(request.value("begin")) == -1)
            return false;

        if (
            request.value("end") != "" && stringToUInt(request.value("end")) == -1)
            return false;

        // if (request.value("end"))
        return true;
    }

    if (request.method() == DEL && request.path() == "/api/v2/all") {
        int espID = stringToUInt(request.value("esp-id"));
        if (espID == -1) return false;

        return true;
    }

    //add more endpoints above
   
    // end of filter
    return false;
}

bool v1(Request request)
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

        return true;
    }

    if (request.method() == GET && request.path() == "/api/v1")
    {
        // Check valid number
        std::string strBegin = request.value("begin"), strEnd = request.value("end");
        // std::cout << "begin: " << stringToUInt(strBegin)
        //           << std::endl
        //           << "end: " << stringToUInt(strEnd) << "\n";

        long int begin = stringToUInt(strBegin), end;

        if (strEnd == "")
            end = time(NULL);
        else
            end = stringToUInt(strEnd);

        if (begin == -1 || end == -1 || begin > end)
        {
            // std::cout<<"Filter: Bad request !\n";
            return false;
        }

        return true;
    }

    if (request.method() == GET && request.path() == "/api/v1/range")
    {

        if (request.value("begin") == "" && request.value("end") == "")
            return false;
        if (request.value("begin") != "" && request.value("end") != "")
            return false;

        if (
            request.value("range") != "" && (stringToUInt(request.value("range")) == -1 || stringToUInt(request.value("range")) >= 5000))
            return false;

        if (
            request.value("begin") != "" && stringToUInt(request.value("begin")) == -1)
            return false;

        if (
            request.value("end") != "" && stringToUInt(request.value("end")) == -1)
            return false;

        // if (request.value("end"))
        return true;
    }

    if (request.method() == DEL && request.path() == "api/v1/range")
    {
        if (request.value("begin") == "" || request.value("end") == "" || request.value("esp-id") == "")
            return false;
        int begin = stringToUInt(request.value("begin")),
            end = stringToUInt(request.value("end")),
            espID = stringToUInt(request.value("esp-id"));

        if (begin == -1 || end == -1 || espID == -1)
            return false;
        if (begin > end)
            return false;
        return true;
    }

    if (request.method() == GET && request.path() == "/test")
    {
        return true;
    }

    if (request.method() == DEL && request.path() == "api/v1/all")
    {
        return true;
    }

    // Default:
    return false;
}
