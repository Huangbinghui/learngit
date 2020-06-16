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

## 工作区和暂存区



git add 将文件添加到了暂存区

git commit 将暂存区文件提交到了当前分支

## 管理修改



add前丢弃工作区修改。

```shell
git checkout -- Git.md
```

add后，commit前丢弃修改，撤销暂存区修改

```shell
git reset HEAD Git.md
```

## 删除文件



提交删除

```shell
git rm test.txt

git commit -m "remove test.txt"
```

删错了

```shell
git checkout -- test.txt
```

checkout 其实使用版本库里的版本替换工作区里的版本

## 添加到远程仓库



关联一个远程库要先生成密钥

```shell
ssh-keygen -t rsa -C "youremail@qq.com"
```

github 中添加user/.ssh/id_rsa.pub 内容

github 中创建一个新仓库

```shell
git remote add origin git@github.com:account_name/learngit.git
git push -u origin master

之后提交到远程库
git push origin master
```

## 从远程库克隆



```shell
git clone git@github.com:Huangbinghui/learngit.git
```

## 分支管理



创建dev分支，并切换

```shell
git checkout -b dev
-b 创建并切换，相当于以下两条
git branch dev

git checkout dev
```

切换回分支master

```shell
git checkout master
```

dev 分支合并到当前分支

```shell
git merge dev
```

合并后删除dev分支

```shell
git branch -d dev
```

查看分支

```shell
git branch
```

**switch**  切换分支(创建并切换到新的分支)

```shell
git switch -c dev
```

切换到master分支

```shell
git switch master
```

## 分支管理策略



禁用`Fast forward`

```shell
git merge --no-ff -m "merge with no-ff" dev
```

## bug管理



`stash`功能，可以把当前工作现场“储藏”起来，等以后恢复现场后继续工作

```shell
git stash

git stash list

git stash pop

git stash apply stash@{0}
```

