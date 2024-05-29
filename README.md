# STC32F12K_BST_Epoilt

2024年江西科技学院蓝色技术工作室智能车队 折线电磁组 BST-磁驭

# 注意

## 编码格式或中文乱码

Keil原因，使用中文注释必须使用GB2312编码，所以当查看本项目代码出现乱码时，请尝试将编码方式调整为GB2312

## 无用设备驱动文件

为节省存储空间，没有使用到的设备驱动文件已经从Keil项目中移出，保存在STC32F_Code\Libraries\seekfree_peripheral\Unuse文件夹中。
若有重新添加的需要，请在Keil的Manage Project Items中将需要的文件重新添加到项目中。

![1716831537859](image/README/1716831537859.png)

# 参考资料

感谢以下开源项目和博客教程提供的参考与帮助

## 开源项目

[第十六届智能车竞赛-国家级一等奖-嵌入式控制开源(恩智浦杯/飞思卡尔杯)-全国第14名 🏆](https://github.com/ittuann/Enterprise_E)

[第16届全国大学生智能车大赛-智能视觉组 上海交通大学 AuTop战队 RT1064主控代码](https://github.com/SJTU-AuTop/RT1064-Code)

## 博客教程

[基于简化KNN的模板匹配的智能车电磁元素识别算法_基于knn实现内容匹配-CSDN博客](https://blog.csdn.net/qq_20545625/article/details/126010928)

# TODO

* [ ] 前馈PID
* [ ] 卡尔曼滤波
