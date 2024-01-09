#include "Controller.hpp"

PIRDB db = PIRDB(DEVELOPMENT);

// return TERMINATE | CONTINUE
MAPPER v2(int client, Request request);
MAPPER v1(int client, Request request);

// register multiple api versions here.
void controller(int client, Request request)
{

    // api version 2
    if (v2(client, request) == TERMINATE)
        return;

    // api version 1
    if (v1(client, request) == TERMINATE)
        return;

    send404(client);
}

MAPPER v2(int client, Request request)
{
    // map endpoint
    if (request.method()  == POST && request.path() == "/api/v2/video") {

        std::cout<<"Request raw text: \n"<<request.getText()<<std::endl;

        Response response = Response(200, TEXT_PLAIN);
        response.setPlainContent("Request logged in server. ");
        response.sendClient(client);

        return TERMINATE;
    }

    if (request.method() == GET && request.path() == "/api/v2")
    {

        // Get value
        std::string strBegin = request.value("begin"),
                    strEnd = request.value("end");

        long int begin = 0, end = 0;
        begin = stringToUInt(strBegin);

        if (strEnd == "")
            end = time(NULL);
        else
            end = stringToUInt(strEnd);

        std::cout << begin << " " << end << "\n";
        // Get record
        std::vector<Record> records = db.recordsWithTimestamp(begin, end);
        std::cout << records.size();

        std::vector<std::string> rows = {};

        // TO-DO; push data to the "rows" array
        // Guard for empty array
        if (records.size() > 0)
        {
            rows.push_back(records[0].csvTitleRow());

            for (Record &record : records)
            {
                rows.push_back(record.toCsvRow());
            }

            // set data
        }

        // std::cout<<root<<"\n";
        // std::cout<<jsonArr<<"\n";
        Response response = Response(200, TEXT_CSV);
        response.setCsvContent(rows);

        response.sendClient(client);
        return TERMINATE;
    };

    if (request.method() == GET && request.path() == "/api/v2/range")
    {
        // std::cout<<"on /api/range/ \n";
        std::vector<Record> records = {};
        long int range;
        if (request.value("range") == "")
            range = ROWMAX;
        else
            range = stringToUInt(request.value("range"));
        if ((request.value("begin")) != "")
        {
            long int begin = stringToUInt(request.value("begin"));
            // range = stringToUInt(request.value("range"));
            // std::cout<<"\nbegin "<<begin<<"\n"<<"range "<<range;
            records = db.recordsWithBeginTime(begin, range);
        }

        if ((request.value("end")) != "")
        {
            long int end = stringToUInt(request.value("end"));
            // range = stringToUInt(request.value("range"));
            // std::cout<<"\nend "<<end<<"\n"<<"range "<<range;
            records = db.recordsWithEndTime(end, range);
        }

        // rows init
        std::vector<std::string> rows = {};
        if (records.size() > 0)
            rows.push_back(records[0].csvTitleRow());
        for (auto &record : records)
        {
            rows.push_back(record.toCsvRow());
        }

        // config response and send
        Response response = Response(200, TEXT_CSV);
        response.setCsvContent(rows);
        response.sendClient(client);

        return TERMINATE;
    }

    if (request.method() == DEL && request.path() == "/api/v2/all") {
        int espID = stringToUInt(request.value("esp-id"));

        db.deleteRecords(espID);
        Response response = Response(200, TEXT_PLAIN);
        response.setPlainContent("Records deleted");
        response.sendClient(client);
        
        return TERMINATE;
    }

    // end of v2 mapper
    return CONTINUE;
};

MAPPER v1(int client, Request request)
{

    if (request.method() == POST)
    {

        std::string espId = request.value("esp-id"),
                    vol = request.value("vol"),
                    time = request.value("time");

        int rc = db.addData(std::stoi(espId), vol, std::stoi(time));

        // init a response object
        if (rc != SQLITE_DONE)
        {
            Response response = Response(500, "text/plain");
            response.setPlainContent("Failed to saving data !");
            response.sendClient(client);
        }
        else
        {

            Response response = Response(200, "text/plain");
            response.setPlainContent("Data saved sucessfully");
            response.sendClient(client);
        }

        return TERMINATE;
    }

    if (request.method() == GET && request.path() == "/api/v1")
    {

        // Get value
        std::string strBegin = request.value("begin"),
                    strEnd = request.value("end");

        long int begin = 0, end = 0;
        begin = stringToUInt(strBegin);

        if (strEnd == "")
            end = time(NULL);
        else
            end = stringToUInt(strEnd);

        std::cout << begin << " " << end << "\n";
        // Get record
        std::vector<Record> records = db.recordsWithTimestamp(begin, end);
        std::cout << records.size();

        // init json object
        // set meta
        Json::Value root;
        root["meta"]["number_of_records"] = records.size();

        // Guard for empty array
        if (records.size() > 0)
        {

            root["meta"]["time_begin"] = records[0].getTimestamp();
            root["meta"]["time_end"] = records[records.size() - 1].getTimestamp();
            root["payload"] = Json::arrayValue;

            Json::Value recordsArr = Json::arrayValue;

            for (Record &record : records)
            {

                recordsArr.append(record.toJson());
            }

            // set data

            root["payload"] = recordsArr;
        }

        // std::cout<<root<<"\n";
        // std::cout<<jsonArr<<"\n";
        Response response = Response(200, APPLICATION_JSON);
        response.setJsonContent(root);
        response.sendClient(client);
        return TERMINATE;
    }

    if (request.method() == GET && request.path() == "/api/v1/with-id")
    {
        int id = stringToUInt(request.value("id"));
        class Record result = db.recordWithID(id);

        if (!result.toJson().isNull())
        {
            Response response = Response(200, APPLICATION_JSON);
            response.setJsonContent(result.toJson());
            response.sendClient(client);
            return TERMINATE;
        }

        send404(client);
        return TERMINATE;
    }

    // Get number of row
    if (request.method() == GET && request.path() == "/api/v1/count")
    {
        int count = db.numOfRows(), oldest = db.oldestTimestamp(), latest = db.latestTimestamp();
        Response response = Response(200, APPLICATION_JSON);
        Json::Value root;
        root["count"] = count;
        root["oldest_timestamp"] = oldest;
        root["latest_timestamp"] = latest;
        response.setJsonContent(root);
        response.sendClient(client);
        return TERMINATE;
    }

    // test page
    if (request.method() == GET && request.path() == "/test" && db.fileName() == DEVELOPMENT)
    {

        Response response = Response(200, "text/html");
        response.setHtmlContent("test.html");
        // printf("%s\n",response.rawText();
        response.sendClient(client);
        return TERMINATE;
    }
    // std::cout << request.getText();

    if (request.method() == GET && request.path() == "/api/v1/range")
    {
               // std::cout<<"on /api/range/ \n";
        std::vector<Record> records = {};
        long int range;
        if (request.value("range") == "")
            range = ROWMAX;
        else
            range = stringToUInt(request.value("range"));
        if ((request.value("begin")) != "")
        {
            long int begin = stringToUInt(request.value("begin"));
            // range = stringToUInt(request.value("range"));
            // std::cout<<"\nbegin "<<begin<<"\n"<<"range "<<range;
            records = db.recordsWithBeginTime(begin, range);
        }

        if ((request.value("end")) != "")
        {
            long int end = stringToUInt(request.value("end"));
            // range = stringToUInt(request.value("range"));
            // std::cout<<"\nend "<<end<<"\n"<<"range "<<range;
            records = db.recordsWithEndTime(end, range);
        }

        Json::Value root;
        // set meta
        root["meta"]["number_of_records"] = records.size();
        root["meta"]["begin"] = records[0].getTimestamp();
        root["meta"]["end"] = records[records.size() - 1].getTimestamp();

        // set payload
        root["payload"] = Json::arrayValue;

        for (auto &record : records)
        {
            root["payload"].append(record.toJson());
        }

        // config response and send
        Response response = Response(200, APPLICATION_JSON);
        response.setJsonContent(root);
        response.sendClient(client);
      

        return TERMINATE;
    }

    // if (request.method() == GET && request.path() == "/api/")

    if (request.method() == PUT && request.path() == "api/v1/tocsv")
    {
        db.allToCSV();
        return TERMINATE;
    }

    if (request.method() == DEL && request.path() == "api/v1/all")
    {
        db.deleteAllTableContent();
        return TERMINATE;
    }

    if (request.method() == DEL && request.path() == "api/v1/range")
    {
        int espID = stringToUInt(request.value("esp-id"));
        long int begin = stringToUInt(request.value("begin"));
        long int end = stringToUInt(request.value("end"));

        int rc = db.deleteRecords(espID, begin, end);
        Response response = Response(200, TEXT_PLAIN);

        if (rc == SUCCESS)
            response.setPlainContent("Delete resources successfully");
        else
            response.setPlainContent("Failed to delete resource");

        response.sendClient(client);
        return TERMINATE;
    }

    return CONTINUE;
}
