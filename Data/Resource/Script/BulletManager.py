import Engine
import MonoBehaviour

class BulletManager(MonoBehaviour.MonoBehaviour) :

	#= Override =====================================
	def Start(self) :
		self.__bulletList = []
		self.__numBullet = 0
		self.__CreateNewList(100)
		return
	#================================================

	#= BulletList ===================================
	def Allocate(self) :
		bullet = self.__bulletList[self.__numBullet]
		self.__numBullet += 1
		return bullet

	def __CreateNewList(self, num) :

		# get bullet resource
		bulletScript = self.context.GetResource().GetScript("Bullet")

		for i in range(num) :

			# create gameobject
			gameobject = self.world.CreateGameObject(self.scene)
			gameobject.transform.SetParent(self.transform)

			# init pram
			gameobject.AddComponent("IScript")
			script = gameobject.GetScript()
			script.scriptInstance = bulletScript

			# script postInit
			bullet = script.scriptInstance.GetInstance(script)
			bullet.PostInit(self)

			# add list
			self.__bulletList.append(bullet)

		return
	#================================================