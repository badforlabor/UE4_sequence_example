# 简介

- 使用4.24引擎

- 扩展UE4的Sequencer

  







# 例子

打开场景TestSeqEx（在SeqEx插件内），运行游戏，即可测试



## 轨道

### PlayAudio

与Actor绑定的轨道。仅支持设置属性，并不支持属性轨道。



### SwingElevator

通用的轨道。支持设置属性，也支持属性轨道。





# 扩展

假如自己的需求类似SwingElevator，那么

- 将SwingElevator文件夹复制，改名为TraceXXX，
- 将TraceXXX文件夹里面的内容（.h,.cpp文件），执行全局替换，将SwingElevator替换成TrackXXX即可。





