# DaKuaiTou  Esp32-Cam Web遥控视频小车
## 大块头
喜羊羊的坐骑  
喜羊羊与灰太狼 开心方程式  
![](https://i0.hdslb.com/bfs/archive/971cf475f26b255ec3a44d1611262adfb22eb2c6.png)  
现以Esp32-Cam 小车重现  

## 如何使用
复制`wifiConfig.cpp.edit`为`wifiConfig.cpp`并编辑来设置wifi  
修改`camera_pins.h`来设置开发板型号（摄像头只支持OV2640）  
使用`PlatfromIO`编译上传到esp32
连接开发板的ap或者在路由器查看ip访问页面  
使用键盘上的方向键或者虚拟按钮来控制

## 转换index.h
html格式的文件在web目录下  
[gzip的hex转html](https://gchq.github.io/CyberChef/#recipe=From_Hex('Auto')Gunzip())  
[html转gzip的hex](https://gchq.github.io/CyberChef/#recipe=Find_/_Replace(%7B'option':'Regex','string':'%5E%5C%5Cs*'%7D,'',true,false,true,false)Find_/_Replace(%7B'option':'Regex','string':',%20'%7D,',',true,false,true,false)Gzip('Dynamic%20Huffman%20Coding','index.html.gz','',false)To_Hex('0x%20with%20comma',0)) 记得改`.html.gz`的文件名  
挺方便的嘛~
