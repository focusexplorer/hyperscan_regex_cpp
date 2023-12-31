# hyperscan_regex_cpp
c++ wrapper of hyperscan

## 背景
c++标准库的regex在大批量处理数据时性能非常差。hyperscan是intel开源的正则表达式引擎，性能非常好，但是使用起来不够方便。

这个仓库是hyperscan的c++封装，主要是为了方便使用[hyperscan](https://github.com/intel/hyperscan)的正则表达式引擎。

该封装只有是字符串匹配功能，而没有引入提取功能，如果需要提取功能，可以参考hyperscan的官方例子，或联系本人。

经过封装后的使用方式非常简单
```c++
hsc::SimpleMatch simpleMatch(".*in.*$");
cout<<simpleMatch.match("I am in world")<<endl;
```

如果要同时对一个字符串，验证一个pattern组中各个patter是否符合，可以参考[demo](demo/demo.cpp)的写法。

## 要先编译hyperscan
```bash
git submodule update --init --recursive
cd hyperscan
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=./install -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_CXX_FLAGS='-fPIE -fPIC' -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  ..
make -j 10
cmake --install .
cd ../..
```

## 编译当前库
```bash
mkdir build
cd build
cmake ..
make -j 6
cmake --install . --prefix ./install
cd ..
```


## 编译demo，链接生成的库
```bash
cd demo
mkdir build
cd build
cmake ..
make
./demo
cd ../..
```

可以修改自己需求更改cmake install的目录，demo中都是放在了build/install目录下。

因为代码量不大，用户可以直接把代码复制到自己的仓库中使用。
