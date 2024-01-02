C++ Back-End System for PIR
===========

Abstraction
-------------
**This project gives a fast, lightweight and easy-to-maintain way to send and store PIR records from PIR-ESP32 Client, also to provide data via API for other clients:**

- Platform: Linux (recommended for Debian-based systems)
- Language: C++
- Protocol: HTTP

API Reference
-------------
Our API with 2 versions provides multichoice to get data with 
2 formats: JSON and CSV.

Replace **root_url** with the actual server address
### Version 2 (recommended): Get data in CSV format 

| Method  | URL | Parameters | Description |
| --------| --- | -----------| ----------- |
| GET  | **root_url**/api/v2?begin=`{begin}`&end=`{end}` | `begin`: begin timestamp, `end`: end timestamp  | Get records with begin and end timestamp
| GET |  **root_url**/api/v2/range?begin=`{begin}`&range=`{range}`  | `begin`: begin timestamp, `range`: number of records  | Get records with `begin` timestamp and in a specific number |
| GET |  **root_url**/api/v2/range?end=`{end}`&range=`{range}`  | `end`: end timestamp, `range`: number of records  | Get records with `end` timestamp and in a specific number |

### Version 1 (legacy): Add, get (with JSON format) and delete data 

| Method  | URL | Parameters | Description |
| --------| --- | -----------| ----------- |
| GET  |  **root_url**/api/v1?begin=`{begin}`&end=`{end}` | `begin`: begin timestamp, `end`: end timestamp  | Get records with begin and end timestamp
| GET |  **root_url**/api/v1/range?begin=`{begin}`&range=`{range}`  | `begin`: begin timestamp, `range`: number of records  | Get records with `begin` timestamp and in a specific number |
| GET |  **root_url**/api/v1/range?end=`{end}`&range=`{range}`  | `end`: end timestamp, `range`: number of records  | Get records with `end` timestamp and in a specific number |
| GET |  **root_url**/api/v1/count  | NONE  | Get metadata of all the database: begin timestamp, end timestamp and number of records |
| GET |  **root_url**/api/v1/with-id?id=`{id}`  | `id`: id of a specific record  | Get a specific record with `id` |
| GET |  **root_url**/test  | NONE  | Run a test page to check the communication and send fake data (for development environment only) |
| POST |  **root_url**?esp-id=`{esp-id}`&vol=`{vol}`&time=`{time}`  | `esp-id`: id of the esp client, `vol`: a string including voltages, `time`: record timestamp (in second)   | This POST is for ESP-32 device only to send records to server |
| DELETE |  **root_url**/api/v1/all  | NONE  | ⚠️ Be cautious when using, it will delete all records in the database |
| DELETE |  **root_url**/api/v1/range?begin=`begin`&end=`end`&esp-id=`esp-id`  | `begin`, `end`: begin and end timestamp of the records to be deleted, `esp-id`: id of the ESP-32 device to delete its records  | ⚠️ Be cautious when using, it will delete a specific range of records in database |

***
#### January 2023  by Thai-Son Nguyen.

🧑‍💻🧑‍💻🧑‍💻 Happy coding !!! 🧑‍💻🧑‍💻🧑‍💻




