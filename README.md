# Overview

High performance HTTP/1.1 server on Linux.

# Contents
  * [Features](#features)
  * [Benchmark](#becnmark)
  * [Build Environment](#requirements)
  * [How to build?](#how-to-build)

## Features
- [ ] HTTP Compression

## Benchmark
Benchmark is done with the help of [siege](https://www.joedog.org/siege-manual/)

Recent benchmark with terribly bad performance :^)
```text
Transactions:                    538 hits
Availability:                 100.00 %
Elapsed time:                  59.71 secs
Data transferred:              15.94 MB
Response time:                 14.18 secs
Transaction rate:               9.01 trans/sec
Throughput:                     0.27 MB/sec
Concurrency:                  127.80
Successful transactions:         538
Failed transactions:               0
Longest transaction:           19.79
Shortest transaction:           0.17
```

## Build Environment
* Ubuntu 20.04.2 LTS
* GCC 9.3.0
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
* Uri
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
