# Toy-12306
This is a train ticket management system, lab 2 for CS1951 (Data structures), SJTU. Simulating the functions of www.12306.cn

文件结构：

`src/main.cpp` 主函数，指令分析器。读入指令并且分配到各个类的相应成员函数中。

`src/UserSystem.h` 用户系统，包含注册、登录、登出、更改和查询用户等功能。

`src/TrainSystem.h` 列车系统，主要处理与车票无关的功能：增加、删除、查询、发布列车。

`src/TicketSystem.h` 车票系统，处理与车票有关的内容：购票、退票并实现候补功能、查询订单、查询直达和中转列车等。

`src/BPT.h` B+ 树实现一个映射（允许一对多），作为数据库运用。

`src/Calendar.h` 实现 `Date Time DateTime` 三个类，表示日期、时间，和日期 + 时间，实现字符串构造、日期时间加减，以及向字符串转换等功能。

`src/vector.h src/map.h` 模拟 STL 中的相应容器。

`src/exceptions.h` 错误类，出现错误时报错，便于调试。

`src/utils.h` 通用头文件，实现封装的字符串类、二分和（归并）排序。

`user.dat train.dat ticket.dat` 分别存储了全部用户、列车、车票的所有信息，采用顺序方式存储（文件前有一个 `int` 表示用户 / 列车 / 车票数）

`UserIndex TrainIndex` 分别为用户名和车次到其在相应文件中位置的映射。

`StationIndex` 为车站名到始发 / 过路 / 终到车在 `train.dat` 中位置的（广义）映射。

`QueueIndex` 为特定日期的列车到候补订单在 `ticket.dat` 中位置的（广义）映射。

------------------------------

几个重要函数的实现：

退票：在 `QueueIndex` 中查找相应列车的候补订单，从前往后枚举有共同区间的订单，检查是否能够补票成功。

查找直达车：对于两个车站，在 `StationIndex` 中查询列车得到两个 `vector`，然后分别用一个指针扫描两个 `vector` 找到共同列车，检查过站顺序，将信息存在新 `vector` 中准备排序（按时间或按价格）

中转查询：对于两端的车站，在 `StationIndex` 中查询列车得到两个 `vector`，直接遍历两个 `vector` 找到共同车站，检查日期是否合法：前序车次到站后必须保证后续列车还有车次在其后到站。并且，还要分类讨论是否要在中转站等待后续列车发出首班车。检查通过后与现有最优解比较，得到最终方案。
