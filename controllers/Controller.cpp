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
            response.setPlainContent("Data saved sucessfully") ;
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
        if (strBegin == "") begin = 0; else begin = stringToUInt(strBegin);
        if (strEnd == "") end = time(NULL); else end = stringToUInt(strEnd);


        // Get record
        std::vector<Record> records = db.recordsWithTimestamp(begin, end);
        Json::Value jsonArr= Json::arrayValue;
        

        for (Record& record: records) {
            // std::cout <<record.toJsonString()<<"\n";

            jsonArr.append(record.toJson());
        }
        // std::cout<<jsonArr<<"\n";
        Response response = Response(200, APPLICATION_JSON);
        response.setJsonContent(jsonArr);
        response.sendClient(client);
        return;
    }

    if (request.method() == GET && request.path() == "/api/with-id") {
        int id = stringToUInt(request.value("id"));
        class Record result = db.recordWithID(id);
        
        if (!result.toJson().isNull()) {
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
        int count = db.numOfRows();
        Response response = Response(200, APPLICATION_JSON);
        Json::Value root;
        root["count"] = count;
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
        return;
    }

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

    // return 404
    send404(client);
    return;
}
