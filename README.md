# Overview
` Be strict in what you send and tolerant in what you receive. `  

This is an implementation of HTTP/1.1 protocol (rfc2616). 
  
Just for self-learning purpose to gain a deep insight into the mechanisims of HTTP servers.

# Contents
  * [Features](#features)
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)
  * [Examples](#examples)


## Features
* HTTP Methods
  - [x] POST
  - [x] GET
  - [] PUT
  - [] PATCH
  - [] DELETE

- [] HTTPS
- [] DNS


## Requirements
* Linux Or Windows 10
* [Cmake](https://cmake.org/)
* [Googletest](https://github.com/google/googletest)


## How to build? 
```shell
git clone https://github.com/Bitate/HttpServer
cd HttpServer
bash build.sh
```


## Useful links that help me build this project
* WebSocket
  * https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/Writing_WebSocket_servers
* URI
  * https://nodejs.org/api/url.html#url_url
* Percent encoding
  * https://developer.mozilla.org/en-US/docs/Glossary/percent-encoding
  * https://tools.ietf.org/html/rfc3986#section-2.1
  * https://en.wikipedia.org/wiki/Percent-encoding
  * https://developer.twitter.com/en/docs/basics/authentication/oauth-1-0a/percent-encoding-parameters

* Http Status Code
  * https://httpstatuses.com/
  * https://evertpot.com/http/
* Message Content
  * https://stackoverflow.com/questions/18180539/how-to-determine-the-content-type-in-an-http-response
  * https://stackoverflow.com/questions/5661596/do-i-need-a-content-type-header-for-http-get-requests
  * http://httpd.apache.org/docs/2.2/mod/mod_mime.html#addtype
* Tools
  * [Online URI decode/encode tool](https://meyerweb.com/eric/tools/dencoder/)
  * [RawCap - windows command line tool to capture localhost/loopback IP packets](https://www.netresec.com/?page=RawCap)
  * [Database design website](https://dbdiagram.io)
  * [Wireshark - cross platform free software to capture IP packets](https://www.wireshark.org/)
  * [HTTPie](https://github.com/httpie/httpie)
* Books
  * [Http: The Definitive Guide (Definitive Guides) 1st Edition by David Gourley](https://www.amazon.com/HTTP-Definitive-Guide-Guides/dp/1565925092/ref=sr_1_1?dchild=1&keywords=http&qid=1591779734&sr=8-1)
