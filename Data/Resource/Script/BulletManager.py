import Engine
import MonoBehaviour

class BulletManager(MonoBehaviour.MonoBehaviour) :

	def Start(self) :
		self.bulletLis = []
		self.__CreateNewList(100)
		return

	def __CreateNewList(self, num) :
		for i in range(num) :
			gameobject = self.world.CreateGameObject(self.scene)
			gameobject.transform.SetParent(self.transform)
			self.bulletLis.append(gameobject)
		return