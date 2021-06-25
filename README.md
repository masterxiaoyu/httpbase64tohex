# httpserverbase64tohex
简单的base64与16进制互转的http服务器，附测试客户端（支持post，get请求）
linux与windows双版本，windows下为vs2012平台

linux下直接sh mkhttpserver生成testhttpserver可执行文件
ex: ./testhttpclient POST hex 8cac1feafe2affed
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 12

jKwf6v4q/+0=
-----------------------------------------------------
 ./testhttpclient GET hex 8cac1feafe2affed
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 12

jKwf6v4q/+0=
-----------------------------------------------------
 ./testhttpclient POST pem jKwf6v4q/+0=
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 8

0x8c0xac0x1f0xea0xfe0x2a0xff0xed
-----------------------------------------------------
 ./testhttpclient GET pem jKwf6v4q/+0=
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 8

0x8c0xac0x1f0xea0xfe0x2a0xff0xed
-----------------------------------------------------
