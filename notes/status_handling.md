# This file contains my notes about handling of HTTP response status code.

## 100 - Continue
* Common browers will not trigger this status code. This status code is rarely used.

* When client wants to send a huge file, it will send a request with the header **Expect: 100-continue** to expect a response from server with the status **100 - Continue**. The server after receiving the **Expect** header can check the request method, content-length, protocol-version, etc to determine whether it can fulfill client's expectation. Such as responsing with 401 (Unauthorized) or 405 (Method Not Allowed), before the client starts filling the pipes with an unnecessary data.

* A request with the header **Expect**:
```
     PUT /somewhere/fun HTTP/1.1
     Host: origin.example.com
     Content-Type: video/h264
     Content-Length: 1234567890987
     Expect: 100-continue
```

* If the server can satisfy the needs of client, it will send a response with the status code 100 to inform the client that "Dude, I can handle that, just send it :^)". Otherwise, it will send a response with the status code **417 - Expectation Failed** to tell the client that "Sorry, I can't handle that request".