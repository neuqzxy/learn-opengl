# OpenGL_01-概念理解及绘制三角形
[博客地址](https://blog.xinzai.site/2019/06/03/OpenGL-01-%E6%A6%82%E5%BF%B5%E7%90%86%E8%A7%A3%E5%8F%8A%E7%BB%98%E5%88%B6%E4%B8%89%E8%A7%92%E5%BD%A2/)

# Mac环境下配置OpenGL开发环境

## 安装GLFW

[下载GLFW](https://www.glfw.org/download.html)，编译，安装
```shell
cmake . && make && make install
```

## 配置GLAD
GLAD是一个开源的库，它能解决我们上面提到的那个繁琐的问题。GLAD的配置与大多数的开源库有些许的不同，GLAD使用了一个在线服务。在这里我们能够告诉GLAD需要定义的OpenGL版本，并且根据这个版本加载所有相关的OpenGL函数。

打开GLAD的[在线服务](http://glad.dav1d.de/)，将语言(Language)设置为C/C++，在API选项中，选择3.3以上的OpenGL(gl)版本（我们的教程中将使用3.3版本，但更新的版本也能正常工作）。之后将模式(Profile)设置为Core，并且保证生成加载器(Generate a loader)的选项是选中的。现在可以先（暂时）忽略拓展(Extensions)中的内容。都选择完之后，点击生成(Generate)按钮来生成库文件。

GLAD现在应该提供给你了一个zip压缩文件，包含两个头文件目录，和一个glad.c文件。将两个头文件目录（glad和KHR）复制到你的Include文件夹中（或者增加一个额外的项目指向这些目录），并添加glad.c文件到你的工程中。

经过前面的这些步骤之后，你就应该可以将以下的指令加到你的文件顶部了：

```C++
#include <glad/glad.h> 
```

![](http://pub-images.xinzai.site/blog/CG/OpenGL/glad_md.png)
