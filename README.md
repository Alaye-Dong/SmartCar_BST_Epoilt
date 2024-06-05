# STC32F12K_BST_Epoilt

2024年江西科技学院蓝色技术工作室智能车队 折线电磁组 BST-磁驭

# 增量式PI的特殊意义

增量式PI不能直接以“增大P响应快，增大I弥补误差”来理解。详情可以看一下这两篇博客。

> [增量式PID的P和I怎么理解（一）_i增大动态性能-CSDN博客](https://blog.csdn.net/fangchenglia/article/details/109518121)
>
> [增量式PID的P和I怎么理解（二）_pi增大不是减少超调量吗?为什么会使超调量增加-CSDN博客](https://blog.csdn.net/fangchenglia/article/details/109534127?spm=1001.2014.3001.5502)
>
> [增量式PID的P和I怎么理解（三）_增量pid (三)-CSDN博客](https://blog.csdn.net/fangchenglia/article/details/109550178?spm=1001.2014.3001.5502)

> 积分不能激活P的潜力，像增域为200的情况，需要选择小P去提高第一下的突增峰值，可以这么说，P为10它的潜力已经被激活完了，就是100多这么大。我为什么说是P=10的潜力是100左右？P系数所对应的潜力值怎么算？请根据我给的不等式（PWM的限幅值>=增域×P系数+初始PWM值，我这里暂且将这个不等式命名为Bright不等式）去算，计算方法：把初始PWM值默认为0，用PWM的限幅值除以P系数即可得到P的潜力（此时P潜力的值=增域，但是P潜力不是增域，因为增域是给的，P潜力是算出来的），999/10=99.9。想要曲线更完美跟上设定值200，P要改小。那么P给多少是最合适的？也请根据Bright不等式计算：现在的情况是初始PWM值等于轮子的当前速度等于0，于是999/200=4.995≈5，所以P给到5。I参数的给定没有公式，自己慢慢调，靠实际效果给。

PWM的限幅值>=增域×P系数+初始PWM值。

> 如果增量式PID是这样子的，我们要如何使用好它呢？
> 就一句话：P需要多大要根据公式——PWM限幅值==增域*P系数+开电机时的初始PWM来定。

所以我们可以根据这个结论来求出电机增量式P的理论响应性最大值。

以下为代码demo。

```c
#include <stdio.h>

/* 计算 P 的最大值 */
double calculate_max_p(double pwm_limit, double increment, double initial_pwm) {
    if (increment == 0) {
        printf("增域 (increment) 不能为零。\n");
        return -1; /* 返回一个错误值 */
    }
  
    return (pwm_limit - initial_pwm) / increment;
}

int main() {
    double pwm_limit = 1000.0;  /* PWM限幅值 */
    double increment = 5.0;     /* 增域 */
    double initial_pwm = 200.0; /* 开电机时的初始PWM */
    double max_p;

    max_p = calculate_max_p(pwm_limit, increment, initial_pwm);
  
    if (max_p != -1) {
        printf("P 的最大值为: %f\n", max_p);
    }
  
    return 0;
}
```

# 模糊PID

[【智能车】模糊PID控制原理详解与代码实现-CSDN博客](https://blog.csdn.net/weixin_45636061/article/details/124996230)

[飞思卡尔智能车----模糊PID算法通俗讲_汽车运动控制模糊pid-CSDN博客](https://blog.csdn.net/weixin_36340979/article/details/79168052)

# 注意

## 编码格式或中文乱码

Keil原因，使用中文注释必须使用GB2312编码，所以当查看本项目代码出现乱码时，请尝试将编码方式调整为GB2312

## 无用设备驱动文件

为节省单片机存储空间，没有使用到的设备驱动文件已经从Keil项目中移出，保存在 `STC32F_Code\Libraries\seekfree_peripheral\Unuse`文件夹中。
若需要重新添加，请在Keil的Manage Project Items中将需要的文件重新添加到项目中。

![1717048084319](image/README/1717048084319.png)

## 符号链接

本项目使用到了 `Keil uVision Assistant` 插件进行开发，但如果 `.uvprj `文件不在打开的一级子文件夹中，插件将无法自动识别到 `.uvprj `文件，每次都需要手动指定，进行重复的操作。所以可以使用[希望打开vscode时自动打开当前目录下所以子目录的keil工程文件 · Issue #39 · github0null/keil-assistant](https://github.com/github0null/keil-assistant/issues/39)中提到的使用符号链接将深层子文件夹中 `STC32F_Code\Project\MDK\EPOILT.uvproj`文件链接到一级子文件夹中，生成的链接文件为 `SmartCar_BST_Epoilt\EPOILT.symlink.uvproj` ，生成的链接文件将同步目标文件的修改。

# 参考资料

感谢以下开源项目和博客教程提供的参考与帮助

## 开源项目

[第十六届智能车竞赛-国家级一等奖-嵌入式控制开源(恩智浦杯/飞思卡尔杯)-全国第14名 🏆](https://github.com/ittuann/Enterprise_E)

[第16届全国大学生智能车大赛-智能视觉组 上海交通大学 AuTop战队 RT1064主控代码](https://github.com/SJTU-AuTop/RT1064-Code)

## 博客教程

[基于简化KNN的模板匹配的智能车电磁元素识别算法_基于knn实现内容匹配-CSDN博客](https://blog.csdn.net/qq_20545625/article/details/126010928)

[增量式PID的P和I怎么理解（一）_i增大动态性能-CSDN博客](https://blog.csdn.net/fangchenglia/article/details/109518121)

[增量式PID的P和I怎么理解（二）_pi增大不是减少超调量吗?为什么会使超调量增加-CSDN博客](https://blog.csdn.net/fangchenglia/article/details/109534127?spm=1001.2014.3001.5502)

# TODO

* [ ] 前馈PID
* [ ] 卡尔曼滤波
* [ ] 模糊PID
