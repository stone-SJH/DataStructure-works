修正版：
	1. 修正了关键字可以被作为变量名使用的bug
		例如：demo中input then 是正确的输入，而在我的版本中input then会输出syntax error
	2. 修正了命令结束后跟一些其他输入不报错的bug
		例如：demo中list ninfiaemnicean是正确的输入，而在我的版本中会输出syntax error
	3. 修正了run模式下记录的代码中的bug重复报错的bug
		例如：在我的版本中 10 rem3 4
				  20 input4 x
				  30 5
		 		  run
		      这样的代码仅会输出一个syntax error
	4.该修正版本可以通过score测试，另外我还交了一个非修正版本，也可以通过score测试。
	5.注释在本版本代码中有详细注解，而在非修正版本中无。
by stone
2014/11/25
