# Decoda@k
Lua IDE and Debuger

原项目：https://github.com/unknownworlds/decoda

## 动机
自己觉得Decoda在windows环境下的的调试要好于其他工具，所以用得比较多，但原来的Decoda项目已经没人维护更新很多年了，所以把自己对Decoda的一些修改和更新分享下

## 修改内容
* 增加Lua 5.4版本的支持（由于自己的使用场景，跳过了5.3）
* 增加了自用的一些语法关键字高亮显示（class、switch、case等），不影响官方Lua版本代码的使用
* 增加Project目录管理，可增加子目录，方便项目管理
* 增加StepOut调试功能，方便Step调试时，从当前函数跳出
* 增加自动打开文档功能，下次打开Project时，自动打开上次关闭/退出时已打开的文档
* 优化界面代码逻辑

## 未来打算/可能
* 由于界面元素简单，目前没有打算增加多语言支持
* 增加编辑器utf8或多字节字符支持
* 由于Decoda是通过注入DLL的方式来实现Hook，不适合跨平台移植和远程调试，可能会考虑支持MobDebug

#