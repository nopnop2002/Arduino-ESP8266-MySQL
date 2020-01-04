# Arduino-ESP8266-MySQL
Operate MySQL from ESP8266 via HTTP.   
Use [this](https://github.com/mevdschee/php-crud-api) as middleware.

![0001](https://user-images.githubusercontent.com/6020549/71568475-e7fce780-2b0a-11ea-8e91-52a6d268d0cd.jpg)

# Host Side

## Install php-crud-api
```
$ cd $HOME

$ git clone https://github.com/mevdschee/php-crud-api
```

## Create DB and User
```
$ cd php-crud-api/

$ sudo mysql -u root -p
mysql> source tests/fixtures/create_mysql.sql;

mysql> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| php-crud-api       |
| sys                |
+--------------------+
6 rows in set (0.00 sec)


mysql> select Host, User from mysql.user;
+-----------+------------------+
| Host      | User             |
+-----------+------------------+
| localhost | debian-sys-maint |
| localhost | mysql.session    |
| localhost | mysql.sys        |
| localhost | php-crud-api     |
| localhost | root             |
+-----------+------------------+
7 rows in set (0.00 sec)

mysql> exit
```


## Create Tables
```
$ mysql -u php-crud-api -h localhost --password=php-crud-api

mysql> use php-crud-api;
Database changed

mysql> select database();
+--------------+
| database()   |
+--------------+
| php-crud-api |
+--------------+
1 row in set (0.00 sec)


mysql> source tests/fixtures/blog_mysql.sql;

mysql> SHOW TABLES;
+------------------------+
| Tables_in_php-crud-api |
+------------------------+
| barcodes               |
| categories             |
| comments               |
| countries              |
| events                 |
| invisibles             |
| kunsthandvark          |
| nopk                   |
| post_tags              |
| posts                  |
| products               |
| tag_usage              |
| tags                   |
| users                  |
+------------------------+
14 rows in set (0.00 sec)

mysql> select * from posts;
+----+---------+-------------+--------------+
| id | user_id | category_id | content      |
+----+---------+-------------+--------------+
|  1 |       1 |           1 | blog started |
|  2 |       1 |           2 | It works!    |
+----+---------+-------------+--------------+
2 rows in set (0.00 sec)

mysql> exit
```

## Install PHP
```

$ sudo apt install php

$ sudo apt-get install php-mysql

$ php --version
PHP 7.2.24-0ubuntu0.18.04.1 (cli) (built: Oct 28 2019 12:07:07) ( NTS )
Copyright (c) 1997-2018 The PHP Group
Zend Engine v3.2.0, Copyright (c) 1998-2018 Zend Technologies
    with Zend OPcache v7.2.24-0ubuntu0.18.04.1, Copyright (c) 1999-2018, by Zend Technologies
```


## Start Built-in WEB Server
```
$ php -S 0.0.0.0:8080 -t $HOME/php-crud-api
PHP 7.2.24-0ubuntu0.18.04.1 Development Server started at Mon Dec 30 09:21:32 2019
Listening on http://0.0.0.0:8080
Document root is /home/nop/php-crud-api
Press Ctrl-C to quit.
```


## Test php-crud-api
```
$ curl http://localhost:8080/api.php/records/posts/ | python -mjson.tool
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   134  100   134    0     0   8933      0 --:--:-- --:--:-- --:--:--  8933
{
    "records": [
        {
            "category_id": 1,
            "content": "blog started",
            "id": 1,
            "user_id": 1
        },
        {
            "category_id": 2,
            "content": "It works!",
            "id": 2,
            "user_id": 1
        }
    ]
}
```

---

# Arduino Side

## Install ArduinoJson
This library can be installed with the Arduino-IDE library manager.

![ArduinoJson-1](https://user-images.githubusercontent.com/6020549/71568748-04018880-2b0d-11ea-890e-bb396436bc2d.jpg)

## Build Firmware

The following lines need to be changed according to your environment.
```
const char* SSID = "SSID"; // SSID of Wifi router
const char* PASSWORD = "PASSWORD"; // Password of Wifi router
String MYSQL_SERVER_IP = "SERVER_IP"; // IP address of your WEB server
String MYSQL_SERVER_PORT = "SERVER_PORT"; // Port number of your WEB server
```

## List
```
[List from MySQL] - sending request...
##[BASE_URL]## ==> http://192.168.10.43:8080/api.php/records/posts/

##[RESULT]## ==> {"records":[{"id":1,"user_id":1,"category_id":1,"content":"blog started"},{"id":2,"user_id":1,"category_id":2,"content":"It works!"}]}
array.size()=2
id=1
user_id=1
category_id=1
content=blog started

id=2
user_id=1
category_id=2
content=It works!
```

### Read
```
[Read from MySQL] - sending request...
##[BASE_URL]## ==> http://192.168.10.43:8080/api.php/records/posts/

##[RESULT]## ==> {"id":1,"user_id":1,"category_id":1,"content":"blog started"}
array.size()=0
id=1
user_id=1
category_id=1
content=blog started
```

### Create
```

[Create to MySQL] - sending request...
##[BASE_URL]## ==> http://192.168.10.43:8080/api.php/records/posts/

##[RESULT]## ==> 3
Create Success. Addes id =
3

##[RESULT]## ==> {"records":[{"id":1,"user_id":1,"category_id":1,"content":"blog started"},{"id":2,"user_id":1,"category_id":2,"content":"It works!"},{"id":3,"user_id":1,"category_id":3,"content":"Hello World"}]}
array.size()=3
id=1
user_id=1
category_id=1
content=blog started

id=2
user_id=1
category_id=2
content=It works!

id=3
user_id=1
category_id=3
content=Hello World
```

### Update
```

[Update to MySQL] - sending request...
##[BASE_URL]## ==> http://192.168.10.43:8080/api.php/records/posts/

##[RESULT]## ==> {"id":3,"user_id":1,"category_id":3,"content":"Hello World"}
array.size()=0
id=3
user_id=1
category_id=3
content=Hello World

##[RESULT]## ==> 1
Update Success

##[RESULT]## ==> {"id":3,"user_id":1,"category_id":3,"content":"Hello Japan"}
array.size()=0
id=3
user_id=1
category_id=3
content=Hello Japan
```

## Delete
```
[Delete from MySQL] - sending request...
##[BASE_URL]## ==> http://192.168.10.43:8080/api.php/records/posts/

##[RESULT]## ==> {"id":3,"user_id":1,"category_id":3,"content":"Hello Japan"}
array.size()=0
id=3
user_id=1
category_id=3
content=Hello Japan

##[RESULT]## ==> 1
Delete Success

##[RESULT]## ==> {"records":[{"id":1,"user_id":1,"category_id":1,"content":"blog started"},{"id":2,"user_id":1,"category_id":2,"content":"It works!"}]}
array.size()=2
id=1
user_id=1
category_id=1
content=blog started

id=2
user_id=1
category_id=2
content=It works!
```
