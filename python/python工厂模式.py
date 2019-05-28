#encoding=utf-8
#__author__ == "hanyuAdol"

class LeiFeng():
	def buy_rice(self):
		pass
	def sweep(self):
		pass

class Student(LeiFeng):
	def buy_rice(self):
		print("大学生帮你买米")

	def sweep(self):
		print("大学生替你扫地")

class Volunteer(LeiFeng):
	def buy_rice(self):
		print("志愿者帮你买米")

	def sweep(self):
		print("志愿者帮你扫地")

class  LeiFengFactory():

	#@classmethod
	def create_lei_feng(self, str1):
		kv = {
			'大学生':Student(),
			'社区志愿者':Volunteer()
		}

		return kv[str1]

if __name__ == '__main__':


	leifeng1 = LeiFengFactory().create_lei_feng('大学生')
	leifeng1.buy_rice()
	leifeng1.sweep()

	#可以先使用指针，这样就不需要带括号了
	myFactory = LeiFengFactory()
	leifeng2 = myFactory.create_lei_feng("社区志愿者")
	leifeng2.buy_rice()
	leifeng2.sweep()
'''
不加括号的类，就是静态方法，不需要创建实例对象，但是
需要在该方法上写@classmethod提示
这样。
'''