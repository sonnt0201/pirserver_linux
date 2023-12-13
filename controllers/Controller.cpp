#include "Controller.hpp"

PIRDB db = PIRDB(DEVELOPMENT);

std::string toJson(std::vector<std::vector<std::string>> vec)
{
    std::string json = "{";
    for (long i = 0; i < vec.size(); i++)
    {
        std::string key = vec[i][0];

        std::string value = vec[i][1];
        // while (value[0] == '0') value.erase(0,1);

        json += '\"' + key + "\": " + value;
        if (i < (vec.size() - 1))
            json += ',';
    }
    json += "}";
    return json; // Add this return statement
};

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
            response.body = "Failed to saving data !";
            response.sendClient(client);
        }
        else
        {

            Response response = Response(200, "text/plain");
            response.body = "Data saved sucessfully";
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
        Response response = Response(200, "application/json");
        int numRows = db.numOfRows();

        // set meta value to json
        std::vector<std::vector<std::string>> metaStr = {{"start-id", std::to_string(std::max(numRows - 1000, 1))}, {"end-id", std::to_string(numRows)}};
        std::string body = " { \"meta\" : " + toJson(metaStr);

        // set main value to json
        body += ", \"payload\" : ";
        body += "[";

        for (int i = std::max(numRows - 1000, 1); i <= numRows; i++)
        {
            std::vector<std::string> data = db.getDataWithID(i);
            std::vector<std::vector<std::string>> str = {{"id", std::to_string(i)}, {"esp_id", data[0]}, {"voltage", data[1]}, {"timestamp", data[2]}};
            body += toJson(str);

            if (i < numRows)
            {
                body += ",";
            }
        }

        body += "] }";
        response.body = body;
        response.sendClient(client);
        return;
    }

    // Get number of row
    if (request.method() == GET && request.path() == "/api/count")
    {
        int count = db.numOfRows();
        if (count >= 0)
        {
            Response response = Response(200, "application/json");
            std::string body = "";
            std::vector<std::vector<std::string>> str = {{"count", std::to_string(count)}};
            body += toJson(str);
            response.body = body;
            response.sendClient(client);
            return;
        }
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
        int begin = stoi(request.value("begin"));
        int end = stoi(request.value("end"));
        Response response = Response(200, "application/json");

        // set meta value to json
        std::vector<std::vector<std::string>> metaStr = {{"start-id", std::to_string(begin)}, {"end-id", std::to_string(end)}};
        std::string body = " { \"meta\" : " + toJson(metaStr);

        // set main value to json
        body += ", \"payload\" : ";
        body += "[";

        

        for (int i = begin; i <= end; i++)
        {
            std::vector<std::string> data = db.getDataWithID(i);
            std::vector<std::vector<std::string>> str = {{"id", std::to_string(i)}, {"esp_id", data[0]}, {"voltage", data[1]}, {"timestamp", data[2]}};
            body += toJson(str);

            if (i < end)
            {
                body += ",";
            }
        }

        body += "] }";
        response.body = body;
        response.sendClient(client);
        return;
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
    Response response = Response(404, "text/html");
    response.setHtmlContent("404.html");
    // printf("%s\n",response.rawText();
    response.sendClient(client);
    return;
}
