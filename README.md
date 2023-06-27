# netreplay

纯 C++ 实现 Http 流量重放工具。

暂不支持 Windows 平台。

## 基本使用

```shell
# 表示将目的端口为 8000 的 http 网络包数据打印到标准输出以及发送到 `http://localhost:8001` http 服务
netreplay --input-cap :8000 --output-stdout --output-http http://localhost:8001
```

## 编译构建

查看 `Makefile` 或 `CMakeLists.txt` 查看依赖的相关库。

使用 Makefile:

```shell
make
```

使用 cmake:

```shell
bash build.sh
```

## 参考

https://github.com/buger/goreplay
