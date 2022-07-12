# YuvPrint
Print Chinese characters on YUV graph
# 输入文件
字符串文件的文件名默认为input，YUV文件名默认为c1.yuv
# 输出文件
YUV输出文件名默认为afterc1.yuv
# 实现
读取hzk16点阵字模文件，修改对应Y、U、V层的数据，实现显示文字的效果，具体功能封装在draw_Font_Func函数中
# 注意
输入文件仅支持中文GB2316编码，最大32字符，不支持阿拉伯数字，不支持英文标点符号
Linux可用下述指令将xxx.txt转为本程序可以识别的input文件
iconv -f UTF-8 -t GBK xxx.txt -o input
