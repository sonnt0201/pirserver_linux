#include "Controller.hpp"

PIRDB db = PIRDB(DEVELOPMENT);

void controller(int client, Request request)
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

        return;
    }

    if (request.method() == GET && request.path() == "/")
    {
        Response response = Response(200, "text/html");
        response.setHtmlContent("index.html");
        // printf("%s\n",response.rawText();
        response.sendClient(client);
        return;
    }

    if (request.method() == GET && request.path() == "/api")
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
            root["records"] = Json::arrayValue;

            Json::Value recordsArr = Json::arrayValue;

            for (Record &record : records)
            {
                std::cout << record.toJsonString() << "\n";

                recordsArr.append(record.toJson());
            }

            // set data

            root["records"] = recordsArr;
        }

        // std::cout<<root<<"\n";
        // std::cout<<jsonArr<<"\n";
        Response response = Response(200, APPLICATION_JSON);
        response.setJsonContent(root);
        response.sendClient(client);
        return;
    }

    if (request.method() == GET && request.path() == "/api/with-id")
    {
        int id = stringToUInt(request.value("id"));
        class Record result = db.recordWithID(id);

        if (!result.toJson().isNull())
        {
            Response response = Response(200, APPLICATION_JSON);
            response.setJsonContent(result.toJson());
            response.sendClient(client);
            return;
        }

        send404(client);
        return;
    }

    // Get number of row
    if (request.method() == GET && request.path() == "/api/count")
    {
        int count = db.numOfRows(), oldest = db.oldestTimestamp(), latest = db.latestTimestamp();
        Response response = Response(200, APPLICATION_JSON);
        Json::Value root;
        root["count"] = count;
        root["oldest_timestamp"] = oldest;
        root["latest_timestamp"] = latest;
        response.setJsonContent(root);
        response.sendClient(client);
        return;
    }

    // test page
    if (request.method() == GET && request.path() == "/test" && db.fileName() == DEVELOPMENT)
    {

        Response response = Response(200, "text/html");
        response.setHtmlContent("test.html");
        // printf("%s\n",response.rawText();
        response.sendClient(client);
        return;
    }
    // std::cout << request.getText();

    if (request.method() == GET && request.path() == "/api/range")
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

        return;
    }

    // if (request.method() == GET && request.path() == "/api/")

    if (request.method() == PUT && request.path() == "/tocsv")
    {
        db.allToCSV();
        return;
    }

    if (request.method() == DEL && request.path() == "/all")
    {
        db.deleteAllTableContent();
        return;
    }

    if (request.method() == DEL && request.path() == "/range")
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
        return;
    }
    
    // return 404
    send404(client);
    return;
}
