## Overview
` Be strict in what you send and tolerant in what you receive. `  

This is an implementation of HTTP/1.1 protocol (rfc2616). 
  
Just for self-learning purpose to gain a deep insight into the mechanisims of HTTP servers.


## Enviroment & Dependence
* Cross Platform
* CMake Build System
* Google Test Framework

## Build 
```bash
1. git clone https://github.com/Bitate/HttpServer
2. cd HttpServer
3. git clone https://github.com/google/googletest
4. mkdir build && cd build
5. cmake ..
```


## References
* Percent encoding
  * https://developer.mozilla.org/en-US/docs/Glossary/percent-encoding
  * https://tools.ietf.org/html/rfc3986#section-2.1
  * https://en.wikipedia.org/wiki/Percent-encoding
  * https://developer.twitter.com/en/docs/basics/authentication/oauth-1-0a/percent-encoding-parameters
* Message Content
  * https://stackoverflow.com/questions/18180539/how-to-determine-the-content-type-in-an-http-response
  * https://stackoverflow.com/questions/5661596/do-i-need-a-content-type-header-for-http-get-requests
  * http://httpd.apache.org/docs/2.2/mod/mod_mime.html#addtype
* Tools
  * [Online Uri decode/encode tool](https://meyerweb.com/eric/tools/dencoder/)
  * [RawCap - windows command line tool to capture localhost/loopback IP packets](https://www.netresec.com/?page=RawCap)
  * [Wireshark - cross platform free software to capture IP packets](https://www.wireshark.org/)
* Books
  * [Http: The Definitive Guide (Definitive Guides) 1st Edition by David Gourley](https://www.amazon.com/HTTP-Definitive-Guide-Guides/dp/1565925092/ref=sr_1_1?dchild=1&keywords=http&qid=1591779734&sr=8-1)