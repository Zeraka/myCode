'''
工厂模式方法
'''
class LeiFeng():
	def buy_price():
		pass

	def sweep():
		pass

class Student(LeiFeng):
	def buy_price(self):
		print("大学生帮助你买米")

	def sweep(self):
		print("大学生帮助你扫地")

class Volunteer(LeiFeng):
	def buy_price(self):
		print("志愿者帮你买米")

	def sweep(self):
		print("志愿者帮你扫地")


#工厂模式类

#先建立抽象工厂
class LeiFengFactory():
	def create_lei_feng(self):
		pass

class StudentFactory(LeiFengFactory):
	def create_lei_feng(self):
		return Student()

class VolunteerFactory(LeiFengFactory):
	def create_lei_feng(self):
		return Volunteer()

if __name__ == '__main__':
	myFactory = StudentFactory()#只需要一个实例，可以不断调用函数，故使用指针没不用每次都创建

	LeiFeng1 = myFactory.create_lei_feng()
	LeiFeng2 = myFactory.create_lei_feng()
	
	LeiFeng1.buy_price()
	LeiFeng2.sweep()

