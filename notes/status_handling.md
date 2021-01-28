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


## 405 - Method Not Allowed
* This does not mean that the request method is not understood/supported by the server. Instead, the server understands/supports the request method, but the request-uri(requested resource) is not allowed to use that request method.

* For example, a PUT request on a read-only resource.

* The following transaction between a client and a server illustrates a situation where the request is attempting to PUT a value to the server, and the server returns 405, which indicates that the PUT method is not allowed for the value.
     * HTTP request:
          ```
               PUT /api/values/1 HTTP/1.1
               Content-type: application/json
               Host: localhost
               Accept: */*
               Content-Length: 12

               "Some Value"
          ```
     * HTTP response:
          ```
               HTTP/1.1 405 Method Not Allowed
               Content-Type: application/json; charset=utf-8
               Date: Wed, 15 May 2013 02:38:57 GMT
               Content-Length: 72

               {"Message":"The requested resource does not support http method 'PUT'."}
          ```
     * In this example, the client sent a valid JSON request to the URI(/api/values/1), but the server returned 405 which indicates that the PUT is not allowed for that URI.

* Real-life examples:
     * https://stackoverflow.com/questions/4904842/error-http-1-0-405-method-not-allowed

* My implementation:
     I mainly focus on whether POST/PUT method is allowed. Namely, wether a request POST/PUT a audio file to the right resource folder which is "resource/audios". If not, return 405. For GET method, other status codes are appropriate.
