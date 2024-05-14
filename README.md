[toc]

这是哈尔滨工业大学计科专业软件工程课程的实验一，基于 MIT 的实验修改而来。

使用面向对象的语言（如 Java 等，本人采用 C++）实现指定功能。

# 有向图的创建

给定英文文本，按照要求创建有向图：

- 每个英文单词为一个节点，忽略大小写差异，单词之间以空格或标点隔开
- 所有英文标点按空格处理，所有非字母(A-Z,a-z)字符应当被忽略
- 单词与单词之间以有向边连接，边的权值为两个单词紧挨着出现的次数
  - 如文本中出现了`Hello, World`两次，则从<kbd>Hello</kbd>到<kbd>World</kbd>的边的权值为 2

# 展示要求

代码应当实现以下功能：

- 根据文本生成有向图
- 展示有向图
- 查询两个单词的桥接词
  - 如文本中有<kbd>a piece of</kbd>则<kbd>a</kbd>与<kbd>of</kbd>的桥接词中有<kbd>piece</kbd>
  - 两个单词有多个桥接词的，全部展示
- 生成新文本
  - 根据用户输入的文本，如果出现连续两个出现在文本中的单词，则在中间插入一个桥接词
- 给定两个单词，输出有向图中两个单词的最短路径
- 随机游走
  - 从有向图任意节点开始，随机选边游走，直到遇到没有出度的点、或者遇到一条走过的边

# 代码实现

在此对我的实现方法进行简单说明。

## 有向图的保存与展示

由于本人对 C++图形化并不熟悉，因而无法使用 C++的三方库绘制和展示有向图。绘制有向图，我一般在 Markdown 里通过内置 mermaid 来实现。

万幸，[mermaid-js](https://github.com/mermaid-js)提供了可绘制有向图的 mermaid 语言的[cli 版](https://github.com/mermaid-js/mermaid-cli)，可以生成包括 svg、png 在内的多种图片类型，那我们不妨直接输出为 mermaid 文件，用这个来生成图片。

安装方法如下：

```bash
# mermaid-cli生成图片需要依赖于chromium浏览器
# 直接安装chrome浏览器即可
sudo apt install google-chrome-stable
# 设置环境变量
export PUPPETEER_SKIP_DOWNLOAD=true
# 使用npm安装mermaid-cli
npm install -g @mermaid-js/mermaid-cli
```

生成图片使用以下命令：

```bash
mmdc --puppeteerConfigFile config.json -i out.mmd -o output.svg
```

其中`config.json`文件内容如下：

```json
{
  "executablePath": "/usr/bin/google-chrome"
}
```

生成图片之后，通过浏览器展示图片：

```cpp
void TextGraph::showGraph() {
    // to ensure the robustness of the program,
    // we use fork to open the browser
    if (fork() == 0) {
        system("firefox output.svg");
        exit(0);
    }
}
```

这里为了保证代码主体运行的稳定性，使用`fork`创建子进程来打开浏览器做展示，防止浏览器报错或崩溃而影响主程序的运行。当然，浏览器也可以选用我们刚下载的`google-chrome`。

## 涉及结构与算法

生成有向图的过程中，图的结构我采用类似邻接表的实现，即`map<string, map<string, int>>`，其中外层`map`的键为节点的名称，内层`map`的键为与该节点相连的节点名称，值为边的权值。

我采用了`istringstream`来处理文本，将文本中的单词提取出来，然后根据单词的出现次数来更新图的结构。

查询桥接词的过程比较简单，不赘述；生成新文本，将用户输入的文本解析为单词序列，然后两两之间查询桥接词并插入即可。

计算最短路径，使用并查集（每个节点维护其父节点信息），具体实现不赘述。

# 运行方法

首先，运行配置脚本：

```bash
chmod +x configure
./configure
```

而后编译运行：

```bash
make
./main
```

# 当前不足

虽然代码实现了基本功能，但是还有很多不足之处：

- 终端用户界面疑似过于丑陋，没有使用更好的库来实现
- 在运行过程中，虽然通过指针实现了可以更改选择的文本，但并未在读取文件的时候检查文件是否存在，会因此造成段错误；但由于解析文本的部分被我放在了构造函数里，因而没有办法检查。或许，建图放在单独的函数里会更好？
- etc.
