C++ Back-End System for PIR
===========

Abstraction
-------------
**This is a C++ Back-End System used for collecting records from PIR (Passive infrared sensor) modules and providing PIR data in mutiple formats. Powered by our [HTTP_Module](), this system gives a light-weight, asynchronous and scalable way to receive, store and read PIR data records**

- Platform: `Linux` (recommended for Debian-based systems)
- Language: `C++`
- Protocol: `HTTP`
- Build tool: `CMake`
- Dependencies: [JsonCpp](https://github.com/open-source-parsers/jsoncpp), [SQLite](https://www.sqlite.org/index.html), [Boost::UUID]()

(SQLite and JsonCpp are already amalgamated in source files so they have no need of any additional installation).

Installation
----------

### Installation for end user

#### Prerequisites
We put product version of server for usage purpose on [THIS Github link](https://github.com/sonnt0201/pirserver_product) so you can manually download using `Code > Download Zip` button or download via `Git`.

If you have tend to use `Git`, [get Git](https://git-scm.com/downloads) first, of course, then clone to your own device.

#### Installtion via `Git`
After getting `Git`, create a directory holding the project then just clone the repository:

```shell
mkdir pirserver
cd pirserver
git clone https://github.com/sonnt0201/pirserver_product
```

Redirect to the downloaded folder then run `pirserver`

```shell
cd product
./pirserver
```

### Installation for development purposes.

#### Prerequisites
Our project use [Boost C++ Libraries](https://www.boost.org/) as a third party and [CMake](https://cmake.org/) as the build tool, so get [Boost](https://www.boost.org/), [Git](https://git-scm.com/downloads) and [CMake](https://cmake.org/) first, then clone and setup the project.

Remember to add `Boost` and `CMake` to the `path`.
#### Installation via `Git`

```shell
$ git clone https://github.com/sonnt0201/pirserver_linux
```

(Or contribute to this repository using `fork`)

Delete old build folder then make a new one using CMake:

Open terminal in root directory then run `CMake` command

```shell
$ mkdir build
$ cd build
$ cmake ..
```

Then compile and run the server in a single command

```shell
$ make run
```

API Reference
-------------

PIR Server give some available APIs to end users for easy starting with. If you are a developer, feel free to add your own APIs or any other features, then contribute using `pull request`.  

### Authentication

- The server allows users to read available data without any authentication required. So almost `HTTP GET` methods don't come with any `user token`.

- In the other hand, all request to modify the existed data: `POST`, `PUT` and `DELETE` must have user token as payload.

- User token is a `UUIDv4 String` giving user all privileges to create, update or delete data. Contact the database administrator to get one.

<!-- ### Create new data  -->

### Create new group
***
Endpoint: `POST /new-group`

- Create a new group of PIRs. Request as a JSON including user token and description for new group.

**Request:**
```json
{
    "user_token": String,
    "description": String
}
```

- Response as the new group's ID

**Response:**

```json
{
    "new_group": String
}
```

### Create new PIR
***
Endpoint: `POST /new-pir`

- Create a new PIR of a specified group. Request as JSON including user token, PIR group's ID and description for new PIR device.


**Request:**
```json
{
    "user_token": String,
}


```
- Response as the new PIR ID

**Response:**
```json
{
    "new_group": String
}
    
```

### Create new record of a PIR
***
Endpoint: `POST /new-record`

- Create a new record with PIR's voltage data. 

**Request:**
```json
{
    
    "user_token": String,
    "pir": String,
    "vol": String,
    "timestamp": number

}


```
- Response as `200 OK` if success or `400 Bad Request` if failure.

### Create new record of mutiple PIRs

Endpoint: `POST /new-records`

- Create new records with PIRs' voltage data. 

**Request:**
```json
{
    
    "user_token": String,
    "records": [
        {
            "pir": String,
            "vol": String,
            "timestamp": Number
        },
        ...
    ]

}

```
- Response as `200 OK` if success or `400 Bad Request` if failure.

<!-- ### Read available data -->

### Get PIRs' infor.

Endpoint: `GET /api/pirs/group`.
- Get all PIRs' infor of an specified group.

**Request:**
```json
{
  "group_id": String
}

```
**Response:**
```json
{
  "payload": [
    {
        "pir_id": String,
        "group_id": String,
        "pir_description": String
    },
    ...
  ]
}
```
### Get records data.

Endpoint: `GET /api/records`

- Get records data of all PIRs in a specified group and a range of time. `begin` and `end` are optional and are timestamps in second of begin and end recorded time.

**Request:**
```json
{
  "group_id": String,
  "begin": number,
  "end": number
}

```
**Response:**
```json
{
  "payload": [
    {
        "pir_id": String,
        "group_id": String,
        "pir_description": String
    },
    ...
  ]
}
```
***
### April 2024 by Thai-Son Nguyen.

🧑‍💻🧑‍💻🧑‍💻 Happy coding !!! 🧑‍💻🧑‍💻🧑‍💻




