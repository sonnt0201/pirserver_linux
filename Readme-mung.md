# Hướng dẫn gửi HTTP Request cho bên ESP32-Client

**Lưu ý**: Phần này là phần nối giữa phần cứng với phần mềm nên kiểu gì mình cũng phải ngồi với nhau rất nhiều để đảm bảo giao thức nó đúng.

Ông cứ đọc kĩ doc này xong có gì không hiểu thì hỏi luôn, tôi sẽ viết kĩ cho.

Server mới cập nhật cũng sẽ in ra nội dung của các Request nó nhận được.

## Tổng quan

Mình sẽ cần gửi một đoạn HTTP Request text chứa dữ liệu voltage của 5 con PIR cùng lúc.

## Dạng text

Dạng text được gửi đi trông như sau:

```http
POST /api/new-records HTTP/1.1
Content-Type: application/json

{
    "timestamp": 123456799,
    "records": [
        {
            "pir": "0",
            "vol": "456_789_123"
        },

        {
            "pir": "1",
            "vol": "345_678_123"
        },

        {
            "pir": "2",
            "vol": "345_678_123"
        },

        {
            "pir": "3",
            "vol": "345_678_123"
        },

        {
            "pir": "4",
            "vol": "345_678_123"
        },
    ]
}


```

Tôi có sửa lại một chút để mình đẩy cái `timestamp` ra ngoài cùng cho đỡ bị lặp.

Phần body là `JSON string`, giữa các object với nhau xuống dòng hay không xuống dòng ko quan trọng, cứ đúng format thì server tự dịch được. Có thể như trên hoặc có thể viết xuôi toèn toẹt như thế này:

```http
{"timestamp": 123456799, "records": [{"pir": "0", "vol": "456_789_123"}, {"pir": "1", "vol": "345_678_123"}, {"pir": "2", "vol": "345_678_123"}, {"pir": "3", "vol": "345_678_123"}, {"pir": "4", "vol": "345_678_123"}]}
```

đều được cả.

Nhìn chung thư viện `JSON` kiểu gì cũng cung cấp 1 hàm chuyển `JSON` mình đã tạo thành `string` thuần, cứ gọi hàm đấy ra để lấy `string` thuần rồi gán nó vào body là được.

## Phân tích từng phần

### Header

Mấy dòng này là `header`:

```http
POST /api/new-records HTTP/1.1
Content-Type: application/json
```

Phương thức là `POST`, đường dẫn (path) là `/api/new-records` (Khác với kì trước mình làm là `/`).

`Content-Type` thì là `application/json` rồi.

### Body 

Hết `Header`, xuống dòng 2 lần rồi thêm nội dung của `body` vào.

`Body` dạng `JSON`, gồm 2 giá trị ở ngoài cùng là `timestamp` và `records`.

`timestamp` là số. Là thời điểm **bắt đầu** lấy số vol

`records` là một mảng (Array), mỗi phần tử của mảng lại là một object nhỏ bao gồm 2 giá trị là `pir` và `vol`:

```http
{
    "pir": String,
    "vol": String
}
```

"pir" sẽ là id của từng con PIR, mình đánh dâu từ 0 -> 4, lưu ý nó ở dạng `String` nên nhớ đặt là `"pir": "0"`, `"pir": "1"`,... thay vì `"pir": 0`, `"pir": 1`.

"vol" thì như kì trước mình làm, 100 số vol kể từ thời gian `timestamp` ở trên.

## Lưu ý nếu làm mãi không được.

Ban đầu ông cứ đảm bảo là cái ESP của ông kết nối với gửi text tới server, làm sao server nhận được cái request là được. 

Giả sử kết nối với gửi text được rồi đấy mà cái format vẫn bị sai và mãi không sửa được, thì ông cứ viết bừa phần body theo 1 cái format mà ông muốn, miễn trong đấy có data của đủ 5 con PIR và thời gian là được. 

Trong trường hợp đấy, Server về sau sẽ được chỉnh để nương theo cái format mà ông gửi


***
### April 2024 by Thai-Son Nguyen.

🧑‍💻🧑‍💻🧑‍💻 Happy coding !!! 🧑‍💻🧑‍💻🧑‍💻