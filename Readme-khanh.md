Hướng dẫn gửi dữ liệu từ ESP32-Client cho team Khánh
==================

Tổng quan
---------

Hôm trước tôi ném cho bọn ông riêng một con server giống kì trước do Server kì này của bọn tôi nó kiểu khác, không dùng được cho bên project của bọn ông.

Tuy nhiên vừa rồi tôi có ngồi làm lại để Server kì này có thể dùng được cho cả Project của bọn tôi lần của bọn ông. Nên bây giờ ông có thể dùng cùng một Server với Mừng.

Ông sẽ chuyển sang Server này để dễ cập nhật các tính năng về sau, đồng thời thay đổi một vài code trong ESP để thích hợp với server này.

Tải xuống và chạy
----------

Tải bản Product của Server [Tại đây](https://github.com/sonnt0201/product)

Nếu ông có `Git` thì dùng `git clone` hoặc không thì tải Zip như thường rồi giải nén.

Mở **terminal** ở thư mục `product` rồi gõ lệnh:

```
./pirserver
```

Nếu server hiện ra bé Nezuko và chữ **Server is listening on port: 8080** tức là chạy OK.

Địa chỉ
---------

Địa chỉ Server sẽ là địa chỉ IPv4 của máy ông với port là 8080, nếu máy ông thay đổi mạng thì tự fix lại cái địa chỉ đấy ở phần code của ESP32 rồi `build` và `upload` lại.

Trước khi chạy server, ông cần gỡ firewall ở PORT 8080 để server có thể nhận được request từ các máy khác bằng cách mở `Terminal` lên và chạy lệnh :

```shell
sudo ufw allow 8080
```

Nhập mật khẩu `sudo` và sau đó PORT 8080 sẽ được mở.

Nên nhớ phải để con ESP và Server ở cùng 1 mạng wifi hoặc 4G.

Các thay đổi trong code ESP
-----


### Phương thức và đường dẫn của HTTP

Tìm đoạn code tạo HTTP Request, trong đó có 1 đoạn có chữ "POST /".

Ông đổi  `POST /` thành `POST /legacy`.


### Cấu trúc dữ liệu của Server


Trong cơ sở dữ liệu của server sẽ gồm 2 **nhóm**, một nhóm là **Mạch của Mừng** có `group_id` là `0`, nhóm còn lại là **Mạch của Khánh** có `group_id` là `1`.

Mạch của Mừng có 5 con PIR đánh số từ 0 đến 4.

Thế nên con PIR của ông (Mạch của Khánh) sẽ được đánh số là 5.

Như vậy ông vào phần code ESP, ở đoạn tạo cái request để gửi đi, ông sẽ để `esp-id=5` thay vì `esp-id=0` như cũ.

### Như vậy phần Request để gửi từ ESP32 sẽ trông như sau

```http
POST /legacy HTTP/1.1
Content-Type: application/x-www-form-urlencoded

esp-id=5&vol=123_456_789_111_222&time=12345678
```

Nhớ phần `vol` phải đủ 100 số ngăn bằng dấu `_`

Nếu gửi thành công thì màn hình `Terminal` bên server sẽ hiện ra đoạn text phía trên.

Một cách để lấy dữ liệu đã ghi
----

Mỗi bản ghi chứa 100 số vol trong 1 giây bắt đầu từ `timestamp`.

Ở đây hướng dẫn ông cách lấy `n` bản ghi gần nhất:

Ở trên máy tính chạy Server, gửi 1 `GET` Request tới địa chỉ sau:

```
http://localhost:8080/api/records/latest?group=1&number=2
```
Thay số 2 ở chỗ `number=2` bằng số Records ông muốn lấy.

Response trả về là 1 `JSON` chứa các records gần nhất thu được.

Ông có thể tự tra Google hoặc con chat AI cách gửi 1 request và lấy response data bằng Python. 

Tìm cách in ra JSON data trả về và xử lý nó.

Một ví dụ về JSON được gửi kèm trong Response:

```JSON
{
    "payload": [
        {
            "pir_id": "5",
            "record_id": "e397d179-3b11-468c-958f-a1c3e47ba699",
            "timestamp": 123456,
            "voltages": [
                123,
                456
            ]
        },
        {
            "pir_id": "5",
            "record_id": "33146e88-ff97-4782-a3cd-5d318355b1c6",
            "timestamp": 123456,
            "voltages": [
                123,
                456
            ]
        }
    ],
    "total": 2
}
```

Nếu vẫn không biết cách lấy dữ liệu, ông có thể cop nguyên cái phần hướng dẫn này vào con chat rồi bảo nó viết code Python hộ, tôi vừa thử rồi, nó làm được đấy 😀.

***
### April 2024 by Thai-Son Nguyen.

🧑‍💻🧑‍💻🧑‍💻 Happy coding !!! 🧑‍💻🧑‍💻🧑‍💻



