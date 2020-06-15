# Git

注意事项

1. 目录中不能有中文！！
2. 禁止用windows自带记事本。

## 创建仓库

命令：

创建文件夹

```shell
mkdir learngit
```

将文件夹设为repository,初始化仓库

```shell
git init
```

向仓库中添加文件

```shell
git add git.md
```

提交修改

```
git commit -m "comment"
```

## 修改文件



查看文件的状态

```shell
git status
```

查看文件修改部分

```shell
git diff Git.md
```

提交修改

```shell
git commit -m "comment"
```

## 版本回退



查看git历史记录

```shell
git log <--pretty=oneline>
```

回退到以前的版本

​	1.会到上一版本

```shell
git reset  --hard HEAD^
```

​	2.回到上上版本(几个版本几个^符号，当写不过来时，用HEAD~100，回退到前100个版本）

```shell
git reset --hard HEAD^^
```

反悔，撤销回退（后面几位数是commit id前几个数字）

```shell
git reset --hard 1094a
```

历史命令

```shell
git reflog
```

