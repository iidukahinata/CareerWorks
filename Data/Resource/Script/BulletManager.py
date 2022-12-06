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
		return 	self.__bulletList.pop(0)

	def Deallocate(self, bullet) :
		self.__bulletList.append(bullet)
		return

	def __CreateNewList(self, num) :

		# get bullet resource
		bulletScript = self.context.GetResource().GetScript("Bullet")
		bulletMesh   = self.context.GetResource().GetMesh("Cube")

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
			bullet.PostInit(self, bulletMesh)

			# add list
			self.__bulletList.append(bullet)

		return
	#================================================